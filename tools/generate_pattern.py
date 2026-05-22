import struct
from pathlib import Path

SOURCE_DIR = Path("../pattern_sources")
OUTPUT_DIR = Path("../data/patterns")

OUTPUT_DIR.mkdir(
    parents=True,
    exist_ok=True
)

for txt_file in SOURCE_DIR.glob("*.txt"):

    rows = []

    with open(txt_file, "r") as file:

        for line in file:

            line = line.strip()

            if len(line) != 18:
                continue

            value = int(line, 2)

            rows.append(value)

    if len(rows) != 18:

        print(f"Skipping {txt_file.name}")
        print("Invalid row count")

        continue

    output_file = (
        OUTPUT_DIR /
        f"{txt_file.stem}.bin"
    )

    with open(output_file, "wb") as file:

        for row in rows:

            file.write(
                struct.pack("<I", row)
            )

    print(f"Generated: {output_file.name}")