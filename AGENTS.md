# Repository Instructions

## Project scope

This repository contains independent course laboratory projects for Real-Time Computer Systems and Computer Architecture. Implement the requested tasks directly and follow any task-specific method stated in the laboratory brief. Explanatory teaching material in a brief is background rather than an additional deliverable.

## Hardware and toolchain

- Target microcontroller: STM32L073RZT6 with a 32 MHz Arm Cortex-M0+ core, 192 KB ECC Flash, 20 KB RAM, and 6 KB ECC EEPROM.
- Development board: NUCLEO-L073RZ.
- User LED: LD2 on PA5, push-pull output, active high.
- User button: B1 on PC13, configured for a falling-edge external interrupt when interrupts are required.
- Debug interface: ST-LINK over PA13/PA14 SWD.
- System clock: 32 MHz, generated from HSI16 through the PLL; AHB, APB1, and APB2 use a divider of one.
- Project tools: STM32CubeMX and Keil MDK-ARM 5, including the VS Code Keil extension where convenient.
- Firmware libraries: STM32CubeL0 HAL and CMSIS.

Detailed reference material is maintained in `docs/hardware.md` and `docs/embedded-principles.md`.

## Engineering approach

- Create a separate STM32CubeMX/Keil project under `Labs/Labx/` for every laboratory.
- Inside each generated firmware project, keep independently written code in `Labx/Inc/` and `Labx/Src/`, and add these paths to the Keil project.
- Keep implementations proportional to a course laboratory task. Do not add speculative fallback chains, redundant validation layers, extensive logging, experiment ledgers, or defensive-only code.
- Add mechanisms that directly support the required behavior. A short push-button debounce interval is appropriate; 30 ms is the project default.
- Use normal embedded-C structure: cohesive functions, clear interfaces, and separate `.c` and `.h` files when they improve organization.
- Keep interrupt callbacks short and focused on the required state transition.
- Do not claim physical measurements or photographed results until hardware testing is available.
- Preserve STM32CubeMX user-code regions and keep independently written application logic separate from generated files.

## Reports

- Write reports in LaTeX using formal academic terminology.
- Use `Abstract`, `Introduction`, task-numbered result sections, and `Conclusion`.
- Present the result of each task and the overall laboratory outcome. Do not include build output as a report result.
- Do not repeat common hardware, toolchain, workflow, or repository constraints in the report or on its title page.
- Do not include a `References` section unless a later laboratory explicitly requires one.
- Omit incidental programming-language facts and internal development commentary, such as which standard header supplies a basic C type.
- Describe the work as a completed laboratory implementation. Keep temporary access limitations and other process constraints out of the report.
- Use Python plotting libraries such as seaborn and matplotlib for useful figures. When a required physical photograph cannot yet be taken, insert a clearly labelled placeholder indicating the image to add later.
- Keep final PDFs in `output/pdf/` and editable report sources inside the corresponding laboratory directory.

## Version control

- Use Git throughout development.
- Commit meaningful intermediate states so that each stage can be reverted independently.
- Keep commits focused; do not combine unrelated generated project, firmware, figure, and report changes without a clear reason.
- Push completed stages to the configured GitHub repository.
