#include "lab2_app.h"

#include "main.h"
#include "task1_uart_calc.h"
#include "task2_i2c.h"
#include "task34_spi_pwm.h"

#include <stdio.h>
#include <string.h>

#define RX_LINE_LENGTH 64U
#define SAMPLE_INTERVAL_MS 500U

typedef enum {
  APP_MENU,
  APP_TASK1_NUMBERS,
  APP_TASK1_OPERATION,
  APP_TASK2_INPUT,
  APP_TASK3_RUNNING,
  APP_TASK4_RUNNING
} AppState;

static UART_HandleTypeDef *uart;
static I2C_HandleTypeDef *i2c;
static SPI_HandleTypeDef *spi;
static TIM_HandleTypeDef *tim_led;
static TIM_HandleTypeDef *tim_servo;

static uint8_t rx_byte;
static char rx_line[RX_LINE_LENGTH];
static volatile uint8_t rx_length;
static volatile uint8_t line_ready;
static AppState state;
static uint8_t task1_values[3];
static uint32_t last_sample_tick;

static void WriteText(const char *text)
{
  HAL_UART_Transmit(uart, (uint8_t *)text, (uint16_t)strlen(text), 100U);
}

static void ShowMenu(void)
{
  WriteText("\r\nLab 2 task menu\r\n"
            "1 - UART calculator\r\n"
            "2 - I2C Arduino exchange\r\n"
            "3 - SPI analog value to LED PWM\r\n"
            "4 - SPI analog value to servo PWM\r\n"
            "Select task: ");
  state = APP_MENU;
}

static void HandleTask1Numbers(const char *line)
{
  if (Task1_ParseNumbers(line, task1_values)) {
    WriteText("Operation [A=add, M=multiply]: ");
    state = APP_TASK1_OPERATION;
  } else {
    WriteText("Enter three 2-digit integers separated by spaces: ");
  }
}

static void HandleTask1Operation(const char *line)
{
  char output[96];
  uint32_t result;

  if (!Task1_Calculate(task1_values, line[0], &result)) {
    WriteText("Operation [A=add, M=multiply]: ");
    return;
  }

  snprintf(output, sizeof(output),
           "Result: %lu\r\n", (unsigned long)result);
  WriteText(output);
  ShowMenu();
}

static void HandleTask2(const char *line)
{
  unsigned int speed;
  unsigned int data;
  Task2_Result result = {0U};
  char output[112];
  HAL_StatusTypeDef status;

  if ((sscanf(line, "%u %u", &speed, &data) != 2)
      || (speed > 3U) || (data > 1U)) {
    WriteText("Enter speed 0..3 and data 0..1: ");
    return;
  }

  status = Task2_Exchange(i2c, (uint8_t)speed, (uint8_t)data, &result);
  if (status != HAL_OK) {
    snprintf(output, sizeof(output),
             "I2C transaction failed (HAL status %u).\r\n",
             (unsigned int)status);
  } else if (data == 0U) {
    snprintf(output, sizeof(output),
             "Command 0x%02X, register 0x00, analog value: %u\r\n",
             (unsigned int)(0x80U + speed), result.analog_value);
  } else {
    snprintf(output, sizeof(output),
             "Command 0x%02X, register 0x01, text: %s\r\n",
             (unsigned int)(0x80U + speed), result.text);
  }
  WriteText(output);
  ShowMenu();
}

static void HandleLine(const char *line)
{
  /* A completed UART line advances the selected task state. */
  if ((state == APP_TASK3_RUNNING) || (state == APP_TASK4_RUNNING)) {
    if ((line[0] == 'M') || (line[0] == 'm')) {
      ShowMenu();
    }
    return;
  }

  switch (state) {
    case APP_MENU:
      if (line[0] == '1') {
        WriteText("Enter three 2-digit integers separated by spaces: ");
        state = APP_TASK1_NUMBERS;
      } else if (line[0] == '2') {
        WriteText("Enter speed 0..3 and data 0..1: ");
        state = APP_TASK2_INPUT;
      } else if (line[0] == '3') {
        WriteText("Task 3 running; enter M to return to the menu.\r\n");
        state = APP_TASK3_RUNNING;
        last_sample_tick = 0U;
      } else if (line[0] == '4') {
        WriteText("Task 4 running; enter M to return to the menu.\r\n");
        state = APP_TASK4_RUNNING;
        last_sample_tick = 0U;
      } else {
        WriteText("Select task 1, 2, 3, or 4: ");
      }
      break;

    case APP_TASK1_NUMBERS:
      HandleTask1Numbers(line);
      break;

    case APP_TASK1_OPERATION:
      HandleTask1Operation(line);
      break;

    case APP_TASK2_INPUT:
      HandleTask2(line);
      break;

    default:
      break;
  }
}

static void UpdateSpiTask(void)
{
  char output[112];
  uint16_t analog_value;
  uint32_t mapped_value;
  uint32_t now = HAL_GetTick();

  if ((state != APP_TASK3_RUNNING) && (state != APP_TASK4_RUNNING)) {
    return;
  }
  /* Periodic sampling leaves UART reception responsive between readings. */
  if ((now - last_sample_tick) < SAMPLE_INTERVAL_MS) {
    return;
  }
  last_sample_tick = now;

  if (Task34_ReadAnalog(spi, &analog_value) != HAL_OK) {
    WriteText("SPI transaction failed.\r\n");
    return;
  }

  if (state == APP_TASK3_RUNNING) {
    mapped_value = Task3_SetLedDuty(tim_led, analog_value);
    snprintf(output, sizeof(output),
             "ADC: %u, LED duty: %lu.%lu%%\r\n",
             analog_value,
             (unsigned long)(mapped_value / 10U),
             (unsigned long)(mapped_value % 10U));
  } else {
    mapped_value = Task4_SetServoPosition(tim_servo, analog_value);
    snprintf(output, sizeof(output),
             "ADC: %u, servo pulse: %lu us, angle: %lu deg\r\n",
             analog_value,
             (unsigned long)mapped_value,
             (unsigned long)(((uint32_t)analog_value * 180U) / 1023U));
  }
  WriteText(output);
}

void Lab2_AppInit(UART_HandleTypeDef *huart,
                  I2C_HandleTypeDef *hi2c,
                  SPI_HandleTypeDef *hspi,
                  TIM_HandleTypeDef *htim_led,
                  TIM_HandleTypeDef *htim_servo)
{
  uart = huart;
  i2c = hi2c;
  spi = hspi;
  tim_led = htim_led;
  tim_servo = htim_servo;

  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
  HAL_TIM_PWM_Start(tim_led, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(tim_servo, TIM_CHANNEL_1);
  __HAL_TIM_SET_COMPARE(tim_led, TIM_CHANNEL_1, 0U);
  __HAL_TIM_SET_COMPARE(tim_servo, TIM_CHANNEL_1, 1500U);

  ShowMenu();
  HAL_UART_Receive_IT(uart, &rx_byte, 1U);
}

void Lab2_AppProcess(void)
{
  char line[RX_LINE_LENGTH];

  if (line_ready != 0U) {
    strcpy(line, rx_line);
    line_ready = 0U;
    HandleLine(line);
  }
  UpdateSpiTask();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == uart) {
    /* Build one line a byte at a time and parse it outside the interrupt. */
    if ((rx_byte == '\r') || (rx_byte == '\n')) {
      if ((rx_length > 0U) && (line_ready == 0U)) {
        rx_line[rx_length] = '\0';
        rx_length = 0U;
        line_ready = 1U;
      }
    } else if ((line_ready == 0U) && (rx_length < (RX_LINE_LENGTH - 1U))) {
      rx_line[rx_length] = (char)rx_byte;
      rx_length++;
    }
    HAL_UART_Receive_IT(uart, &rx_byte, 1U);
  }
}
