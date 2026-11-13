# Lab 4: Mutexes and Semaphores

Task 1 creates three Normal-priority tasks that increment a shared counter through one mutex. Each task attempts `osMutexAcquire(counterMutexHandle, 0)`, increments and transmits while it owns the mutex, releases it, and delays for 1000 ticks. The serial output identifies the task and the updated counter, for example `Task 2: 7`.

Task 2 presents the differences between mutexes and semaphores and explains interrupt-to-task completion signalling in the report.

## Project files

- `Firmware/Lab4/Lab4.ioc`: CubeMX configuration, including the three tasks and shared mutex.
- `Firmware/Lab4/MDK-ARM/Lab4.uvprojx`: Keil project with the `Lab4` target.
- `Firmware/Lab4/Lab4/Inc` and `Src`: application interface, counter task, and serial output.
- `report/main.tex` and `report/Lab4_report.pdf`: report source and final PDF.

Build the `Lab4` target and download through ST-LINK. Open its virtual COM port at 115200 bit/s, 8 data bits, no parity, and 1 stop bit. The application starts printing counter updates after reset. All three tasks use the same entry function with different task identifiers.

Hardware testing recorded 30 consecutive counter values, from 33 to 62, in 15 seconds. Tasks 1, 2, and 3 contributed 8, 7, and 15 updates, respectively. The recording is in `report/data/task1_uart.txt`, and the report includes a live serial-terminal screenshot.

The source implementation is selected over CubeMX's weak entry function by the linker. Shared hardware and RTOS details are maintained in the repository-level `docs` directory.
