let currentPattern = null;

function parsePattern(bytes)
{
    const wheels = [];

    const BYTES_PER_WHEEL = 9;
    const NO_OF_WHEELS = 18;
    const STEPS_PER_WHEEL = 72;

    for(let wheel = 0; wheel < NO_OF_WHEELS; wheel++)
    {
        const wheelBits = [];

        const wheelOffset = wheel * BYTES_PER_WHEEL;

        for(let byteIndex = 0;
            byteIndex < BYTES_PER_WHEEL;
            byteIndex++)
        {
            const value =
                bytes[wheelOffset + byteIndex];

            for(let bitIndex = 0;
                bitIndex < 8;
                bitIndex++)
            {
                wheelBits.push(
                    (value >> (7 - bitIndex)) & 0x01
                );
            }
        }

        // Trim extra bits from last byte
        wheelBits.length = STEPS_PER_WHEEL;

        wheels.push(wheelBits);
    }

    const repeat =
        bytes[bytes.length - 1];

    return {
        wheels: wheels,
        repeat: repeat
    };
}

function renderPattern()
{
    if(currentPattern === null)
    {
        return;
    }

    const container =
        document.getElementById(
            "pattern-preview"
        );

    container.innerHTML = "";

    currentPattern.wheels.forEach(
        (wheel, index) =>
        {
            const row =
                document.createElement("div");

            row.className =
                "wheel-row";

            const wheelNumber =
                String(index + 1)
                .padStart(2, '0');

            row.innerHTML =
                "<strong>Wheel "
                + wheelNumber
                + ":</strong> "
                + wheel.map(
                    bit => bit ? "█" : "."
                ).join("");

            container.appendChild(row);
        }
    );
}

async function loadPatternBinary() 
{
    try
    {
        const response = await fetch("/pattern");
        const buffer = await response.arrayBuffer();
        const bytes = new Uint8Array(buffer); 

        console.log(
            "Pattern size:", buffer.byteLength, "bytes"
        );

        currentPattern = parsePattern(bytes);

        renderPattern();
    }
    catch(error)
    {
        console.error(
            "Failed to load pattern",
            error
        );
    }
}

async function updateStatus()
{
    try
    {
        const response = await fetch("/status");

        const data = await response.json();

        document.getElementById("state").innerText =
            "State : " + data.state;

        document.getElementById("pattern").innerText =
            "Pattern : " + data.patternName;

        document.getElementById("wheel").innerText =
            "Wheel : " + data.wheel + "/" + data.totalWheels;

        document.getElementById("step").innerText =
            "Step : " + data.step + "/" + data.totalSteps;

        document.getElementById("progress").innerText =
            "Progress : " + data.progress + "%";
    }
    catch(error)
    {
        console.error(
            "Failed to update status",
            error
        );
    }
}

async function loadPatterns()
{
    try
    {
        const response = await fetch("/patterns");

        const patterns = await response.json();

        const select = document.getElementById("patterns");

        select.innerHTML = "";

        patterns.forEach(pattern =>
        {
            const option = document.createElement("option");

            option.value = pattern.id;
            option.text = pattern.name;

            select.appendChild(option);
        });
    }
    catch(error)
    {
        console.error(
            "Failed to load patterns",
            error
        );
    }
}

async function loadPattern()
{
    try
    {
        const select = document.getElementById("patterns");

        const patternId = select.value;

        const response = await fetch(
                            "/select?pattern=" +
                            patternId
                        );

        if(!response.ok)
        {
            throw new Error(
                "Pattern selection failed"
            );
        }

        await loadPatternBinary();

        updateStatus();
    }
    catch(error)
    {
        console.error(
            "Failed to load pattern",
            error
        );
    }
}

window.onload = async function()
{
    await loadPatterns();
    await loadPatternBinary();

    updateStatus();

    setInterval(
        updateStatus,
        1000
    );
};