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

