# Embedded Programming Principles

## Register field updates

A peripheral register often contains several independent fields. A field is updated by clearing its mask and then OR-ing the desired encoded value into the same bit positions:

```c
register_value = (register_value & ~field_mask) | encoded_value;
```

This preserves unrelated bits. A single-bit input is isolated with an AND mask and normalized to a Boolean value by comparing the result with zero.

## GPIO input and output

The input data register (`IDR`) reflects the sampled logic level on GPIO pins. The output data register (`ODR`) holds output states. Bitwise masks allow one pin to be read or changed without disturbing the other pins on the port.

## Timer period calculation

For an STM32 general-purpose timer, the update interval is

```text
T_update = (PSC + 1) * (ARR + 1) / f_timer
```

Choosing a convenient counter tick first makes the period calculation direct. At 32 MHz, `PSC = 31999` gives a 1 kHz counter tick; `ARR = 1499` then gives a 1.5 s update interval.

## Interrupt structure

External interrupts capture asynchronous button events, while timer update interrupts perform periodic work. Application state determines which action an event causes. Interrupt callbacks should perform the required state transition directly and remain short.

Mechanical push-buttons produce several transitions around one press. A short time gate after an accepted edge is sufficient for this lab; a 30 ms interval suppresses the contact bounce while preserving normal button interaction.

## Serial communication

UART transfers text without a shared clock, so both endpoints use the same baud rate and frame format. USART2 is configured for 115200 bit/s, eight data bits, no parity, and one stop bit. A line-oriented protocol collects characters until a terminator arrives; a fixed-length protocol arms interrupt reception for the required number of bytes. The completion callback can notify a task to process the received frame.

I2C uses a shared clock and a seven-bit slave address. STM32 HAL expects the address shifted left by one bit, so slave address `0x55` is supplied as `0x55 << 1`. Multi-byte integers are reconstructed in the byte order defined by the slave protocol.

SPI transfers one received byte for every transmitted byte. A register-style slave may require one transfer to select a value and another transfer to clock that value back to the master. Chip select remains active for the complete register transaction.

## PWM mapping

For edge-aligned PWM, the output frequency and duty cycle are

```text
f_PWM = f_timer / ((PSC + 1) * (ARR + 1))
duty  = CCR / (ARR + 1)
```

A 10-bit sample in the range 0 to 1023 can be mapped to a timer compare value with integer arithmetic:

```text
CCR = sample * (ARR + 1) / 1023
```

Servo control uses a 50 Hz period and a pulse width within the actuator's command range. With a 1 MHz timer counter, compare values correspond directly to pulse widths in microseconds.

## RTOS scheduling and memory

CMSIS-RTOS2 thread priorities determine which ready task runs. An interrupt can set a thread flag to wake a task, and the task blocks on that flag while waiting for work. Queue operations copy complete values between tasks. A queue's capacity is its number of items; its width is the number of bytes per item.

CMSIS-RTOS2 `osThreadAttr_t.stack_size` is expressed in bytes. A local array of 150 four-byte integers occupies 600 bytes of its task stack. The allocated stack also holds local variables, function calls, and saved context. `osThreadGetStackSpace()` reports the minimum untouched stack space observed since task creation.

For the Cortex-M3 FreeRTOS port, SVC starts the first task at priority 0. PendSV and SysTick run at the lowest interrupt priority. Interrupts that call RTOS APIs use a numerical priority at or above `configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY`. Use SysTick for the RTOS and a separate hardware timer for the HAL tick.

Lab 3 supplies `SysTick_Handler` in the interrupt source to call `xPortSysTickHandler`. Its two application threads are created in `Task1_Start` or `Task2_Start`; the Keil target selects the implementation through `LAB3_TASK`.

## Mutexes and semaphores

A mutex gives one task ownership of a critical section. Place the complete read-modify-write operation between acquisition and release. When a UART line describes that update, keeping its transmission inside the same section preserves the order of the displayed values. A zero timeout requests the mutex once; the task performs the operation when the returned status is `osOK`. Periodic delays follow the release.

A semaphore counts tokens without assigning an owner. A binary semaphore can transfer a completion event from an interrupt to a waiting task. Create it with maximum count one and initial count zero, then release it in the peripheral completion callback. The task acquires the token and processes the completed transfer. A counting semaphore represents several available resources or pending events. FreeRTOS mutexes support priority inheritance, while semaphores have no owner to inherit a waiting task's priority.

API details are maintained in the [Arm CMSIS-RTOS2 mutex documentation](https://arm-software.github.io/CMSIS_6/latest/RTOS2/group__CMSIS__RTOS__MutexMgmt.html) and [semaphore documentation](https://arm-software.github.io/CMSIS_6/latest/RTOS2/group__CMSIS__RTOS__SemaphoreMgmt.html).
