# Lab 1 — Registers, GPIO, and Timer Interrupts

The project targets the STM32L073RZT6 on a NUCLEO-L073RZ board and uses a 32 MHz system clock.

## Task mapping

| Task | Result |
|---|---|
| 1 | Stepwise GPIOB pin 5 register configuration is presented in the report. |
| 2 | The GPIO input-expression evaluation is explained with a worked example. |
| 3 | `Lab1/Src/task3_polling.c` reads the blue button through `GPIOC->IDR` and controls LD2 through `GPIOA->ODR`. |
| 4 | `Lab1/Src/lab1_app.c` implements button EXTI control and TIM2-based 1.5 s LED blinking without delays. |

## Project layout

```text
Firmware/Lab1/
├── Inc/                 STM32CubeMX-generated headers
├── Src/                 STM32CubeMX-generated source
├── Drivers/             STM32CubeL0 HAL and CMSIS
├── Lab1/Inc/            independently written Lab 1 headers
├── Lab1/Src/            independently written Lab 1 source
├── MDK-ARM/             Keil project
└── Lab1.ioc             STM32CubeMX configuration
```

Open `Firmware/Lab1/Lab1.ioc` in STM32CubeMX to inspect the configuration, or open `Firmware/Lab1/MDK-ARM/Lab1.uvprojx` in Keil MDK-ARM 5 to build the project.

