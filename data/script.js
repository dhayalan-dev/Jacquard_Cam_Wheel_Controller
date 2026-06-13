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

        const startButton = document.getElementById("start-button");

        const loadButton = document.getElementById("load-button");

        const stateElement = document.getElementById("state");

        const abortButton = document.getElementById("abort-button");

        if(data.state === "RUNNING")
        {
            abortButton.disabled = false;
        }
        else
        {
            abortButton.disabled = true;
        }

        if(data.state === "RUNNING")
        {
            loadButton.disabled = true;
        }
        else
        {
            loadButton.disabled = false;
        }

        // Enable Start button only when machine can start
        if(
            data.state === "READY" ||
            data.state === "WAIT_FOR_NEXT_WHEEL"
        )
        {
            startButton.disabled = false;
        }
        else
        {
            startButton.disabled = true;
        }

        // State color modification
        switch(data.state)
        {
            case "RUNNING":
                stateElement.style.color = "#22c55e";
                break;

            case "READY":
                stateElement.style.color = "#3b82f6";
                break;

            case "WAIT_FOR_NEXT_WHEEL":
                stateElement.style.color = "#f59e0b";
                break;

            case "COMPLETED":
                stateElement.style.color = "#8b5cf6";
                break;

            case "ABORTED":
                stateElement.style.color = "#ef4444";
                break;

            case "ERROR":
                stateElement.style.color = "#ef4444";
                break;

            default:
                stateElement.style.color = "#f1f5f9";
                break;
        }

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

async function startMachine()
{
    try
    {
        const response =
            await fetch(
                "/start"
            );

        if(!response.ok)
        {
            throw new Error(
                "Failed to start machine"
            );
        }

        updateStatus();
    }
    catch(error)
    {
        console.error(
            error
        );
    }
}

async function abortMachine()
{
    try
    {
        const response =
            await fetch(
                "/abort"
            );

        if(!response.ok)
        {
            throw new Error(
                "Failed to abort machine"
            );
        }

        updateStatus();
    }
    catch(error)
    {
        console.error(
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