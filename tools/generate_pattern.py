import struct
from pathlib import Path

STEPS_PER_WHEEL = 72
NO_OF_WHEELS = 18

BYTES_PER_WHEEL = (
    (STEPS_PER_WHEEL + 7) // 8
)

REPEAT_COUNT = 2

BASE_DIR = (
    Path(__file__)
    .resolve()
    .parent
    .parent
)

SOURCE_DIR = (
    BASE_DIR /
    "pattern_sources"
)

OUTPUT_DIR = (
    BASE_DIR /
    "data" /
    "patterns"
)

OUTPUT_DIR.mkdir(
    parents=True,
    exist_ok=True
)

for txt_file in SOURCE_DIR.glob("*.txt"):

    print(f"Reading {txt_file.name}")

    wheels = []

    with open(txt_file, "r") as file:

        lines = [
            line.strip()
            for line in file
            if line.strip()
        ]

    if len(lines) != NO_OF_WHEELS:

        print(
            f"Invalid wheel count "
            f"in {txt_file.name}"
        )

        continue

    valid = True

    for line in lines:

        if len(line) != STEPS_PER_WHEEL:

            print(
                f"Invalid step count "
                f"in {txt_file.name}"
            )

            valid = False

            break

        if any(
            c not in "01"
            for c in line
        ):

            print(
                f"Invalid character "
                f"in {txt_file.name}"
            )

            valid = False

            break

        wheel_bytes = bytearray(
            BYTES_PER_WHEEL
        )

        for step, bit in enumerate(line):

            if bit == '1':

                byte_index = step // 8

                bit_index = step % 8

                wheel_bytes[byte_index] |= (
                    1 << (7 - bit_index)
                )

        wheels.append(wheel_bytes)

    if not valid:
        continue

    output_file = (
        OUTPUT_DIR /
        f"{txt_file.stem}.bin"
    )

    with open(output_file, "wb") as file:

        for wheel in wheels:

            file.write(wheel)

        file.write(
            struct.pack(
                "B",
                REPEAT_COUNT
            )
        )

    print(
        f"Generated: "
        f"{output_file}"
    )