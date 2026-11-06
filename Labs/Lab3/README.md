# Lab 3: Tasks, Priorities, and Queues

The project contains two Keil targets for the NUCLEO-F103RB:

| Target | Behavior |
|---|---|
| `Task1` | B1 lights LD2 while held and sends `Car is slowing down` through USART2. The brake task has High priority and the message task has Normal priority. |
| `Task2` | An eleven-byte UART command sets the LED toggle interval. Three queues carry the interval, the count of ten-toggle groups, and the maximum of 150 random integers. |

Open `Firmware/Lab3/MDK-ARM/Lab3.uvprojx`, choose a target, build, and download through ST-LINK. The CubeMX configuration is `Firmware/Lab3/Lab3.ioc`. Application code is in `Firmware/Lab3/Lab3/Inc` and `Src`.

For Task 2, open the virtual COM port at 115200 bit/s, 8N1 and send exactly `-DELAY 1000`, `-DELAY 0500`, or `-DELAY 0250` with the terminal's line ending set to None. The terminal displays the echoed command, `Max: ...`, and `Count: ...`. Each interval is the time between consecutive LED toggles; the complete on/off period is twice that interval.

`scripts/measure_hardware.py` records UART replies, PA5 output transitions through ST-LINK, and the LED task stack watermark. Run it with Task 2 loaded and the terminal port closed:

```powershell
python Labs/Lab3/scripts/measure_hardware.py --port COM6
```

It uses pyserial, pyocd, and pyelftools. Results are stored in `report/data`; `report/generate_figures.py` creates the plots used by the LaTeX report. Shared hardware and RTOS details are maintained in the repository's `docs` directory.
