# Hardware Background

## Microcontroller

The projects target the STM32L073RZT6, an ultra-low-power STM32L0 microcontroller built around a 32 MHz Arm Cortex-M0+ core. The device provides 192 KB of ECC-protected Flash memory, 20 KB of RAM, and 6 KB of ECC-protected data EEPROM.

## Lab board mapping

Lab 1 uses the NUCLEO-L073RZ board configuration:

| Function | MCU signal | Board item | Configuration |
|---|---|---|---|
| User LED | PA5 | LD2, green | Push-pull output, active high |
| User button | PC13 | B1, blue | GPIO external interrupt on falling edge |
| Blink interval | TIM2 update | Internal peripheral | 32 MHz timer clock, 1.5 s update period |
| Debug | PA13 / PA14 | ST-LINK | SWDIO / SWCLK |

The system clock is generated from HSI16 through the PLL with multiplication by 4 and division by 2, producing a 32 MHz SYSCLK. AHB, APB1, and APB2 use a divider of 1.

## Development tools

- STM32CubeMX for pin, clock, interrupt, and peripheral configuration
- Keil MDK-ARM 5 for compilation and debugging
- STM32CubeL0 HAL and CMSIS device support

