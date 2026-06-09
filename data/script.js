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

window.onload = function()
{
    loadPatterns();
    updateStatus();

    setInterval(
        updateStatus,
        1000
    );
};