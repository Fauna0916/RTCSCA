# Lab 4: Mutexes and Semaphores

Task 1 creates three Normal-priority tasks that increment a shared counter through one mutex. Each task attempts `osMutexAcquire(counterMutexHandle, 0)`, increments and transmits while it owns the mutex, releases it, and delays for 1000 ticks. The serial output identifies the task and the updated counter, for example `Task 2: 7`.

Task 2 uses four tasks and a counting semaphore with an initial and maximum count of two. Task 1 shifts the previous buffer value and writes values from 0 to 9; tasks 2, 3, and 4 read the previous value. Each successful acquisition holds its permit for 50 ticks, so two overlapping accesses can be observed. UART output gives the operation, value, and number of active permit holders. A separate mutex serialises each UART line. The report also compares mutexes and semaphores and explains interrupt-to-task completion signalling.

## Project files

- `Firmware/Lab4/Lab4.ioc`: CubeMX peripheral configuration and Task 1's three tasks and shared mutex.
- `Firmware/Lab4/MDK-ARM/Lab4.uvprojx`: Keil project with `Task1` and `Task2` targets.
- `Firmware/Lab4/Lab4/Inc` and `Src`: application interface, counter task, and serial output.
- `report/main.tex` and `report/Lab4_report.pdf`: report source and final PDF.

Build the selected target and download through ST-LINK. The targets define `LAB4_TASK=1` and `LAB4_TASK=2` and produce `Lab4_Task1/Lab4_Task1.hex` and `Lab4_Task2/Lab4_Task2.hex` under `MDK-ARM`. Open the virtual COM port at 115200 bit/s, 8 data bits, no parity, and 1 stop bit. Each application starts printing after reset. Task 2's semaphore and four tasks are created by `Task2_Start()` in `Lab4/Src/task2.c`.

Hardware testing recorded 30 consecutive counter values, from 33 to 62, in 15 seconds. Tasks 1, 2, and 3 contributed 8, 7, and 15 updates, respectively. The recording is in `report/data/task1_uart.txt`, and the report includes a live serial-terminal screenshot.

For Task 1, the source implementation is selected over CubeMX's weak entry function by the linker. The startup selection is inside CubeMX user-code regions. Shared hardware and RTOS details are maintained in the repository-level `docs` directory.
