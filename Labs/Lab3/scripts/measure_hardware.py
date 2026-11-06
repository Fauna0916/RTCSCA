"""Record Task 2 UART replies and LED transitions through the ST-LINK probe."""

import argparse
import csv
import json
import time
from pathlib import Path

import serial
from elftools.elf.elffile import ELFFile
from pyocd.core.helpers import ConnectHelper


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="COM6")
    args = parser.parse_args()
    lab = Path(__file__).resolve().parents[1]
    data = lab / "report" / "data"
    data.mkdir(exist_ok=True)
    elf = lab / "Firmware/Lab3/MDK-ARM/Lab3_Task2/Lab3_Task2.axf"
    with elf.open("rb") as source:
        symbols = ELFFile(source).get_section_by_name(".symtab")
        stack_address = symbols.get_symbol_by_name("task2_stack_free")[0]["st_value"]

    edges = []
    transcript = []
    with ConnectHelper.session_with_chosen_probe(
        target_override="cortex_m", connect_mode="attach",
        options={"frequency": 4000000},
    ) as session, serial.Serial(args.port, 115200, timeout=0) as uart:
        target = session.target
        for interval in (1000, 500, 250):
            uart.reset_input_buffer()
            command = f"-DELAY {interval:04d}"
            transcript.append(f"TX: {command}\n")
            previous = (target.read32(0x4001080C) >> 5) & 1
            started = time.perf_counter()
            uart.write(command.encode("ascii"))
            received = bytearray()
            transitions = 0
            while transitions < 12:
                before = time.perf_counter()
                level = (target.read32(0x4001080C) >> 5) & 1
                sampled = (before + time.perf_counter()) / 2 - started
                received.extend(uart.read(uart.in_waiting))
                if level != previous:
                    transitions += 1
                    edges.append((interval, transitions, sampled, level))
                    previous = level
                time.sleep(0.001)
            transcript.append(received.decode("ascii"))
            print(command, received.decode("ascii").strip(), sep="\n", flush=True)
        stack_free = target.read32(stack_address)

    with (data / "led_edges.csv").open("w", newline="", encoding="utf-8") as output:
        writer = csv.writer(output)
        writer.writerow(("command_ms", "edge", "time_s", "level"))
        writer.writerows(edges)
    (data / "uart.txt").write_text("".join(transcript), encoding="utf-8")
    (data / "stack.json").write_text(json.dumps({
        "allocated_bytes": 1536, "minimum_unused_bytes": stack_free,
        "array_bytes": 600,
    }, indent=2) + "\n", encoding="utf-8")
    print(f"Minimum unused LED task stack: {stack_free} bytes")


if __name__ == "__main__":
    main()
