# Lab 2: Communication and PWM

Lab 2 implements four tasks in one STM32L073RZT6 firmware project:

1. Receive three two-digit integers through USART2 and report their sum or product.
2. Use two UART selections to control the Arduino LED interval and choose either its two-byte analog value or four-byte text response over I2C.
3. Read the Arduino analog value over SPI and map it to an LED PWM duty cycle.
4. Read the same SPI value and map it to a 50 Hz servo pulse.

## Project structure

- `Firmware/Lab2/Lab2.ioc`: STM32CubeMX configuration
- `Firmware/Lab2/MDK-ARM/Lab2.uvprojx`: Keil MDK-ARM project
- `Firmware/Lab2/Lab2/Inc` and `Firmware/Lab2/Lab2/Src`: independently written application modules
- `Lab2_Supplemental-Material`: supplied Arduino slave programs
- `report`: LaTeX report, figures, metadata, and final PDF

## Serial interface

Open the ST-LINK virtual COM port at 115200 bit/s, 8 data bits, no parity, and 1 stop bit. The menu offers tasks 1 to 4. Task 2 accepts `<speed> <data>`: speed selections 0 to 3 send commands `0x80` to `0x83`, while data selections 0 and 1 request the analog value and `RTCA`, respectively. Enter `M` to leave a continuously sampled SPI task.

## Connections

I2C uses PB8/SCL and PB9/SDA. SPI uses PA5/SCK, PA6/MISO, PA7/MOSI, and PB6/CS. PA0 carries the LED PWM output and PB13 carries the servo PWM output. Complete pin mapping is recorded in `../../docs/hardware.md`.
