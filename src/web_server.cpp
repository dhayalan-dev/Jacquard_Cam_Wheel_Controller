#include "web_server.h"
#include "pattern_registry.h"
#include "machine_controller.h"
#include <SPIFFS.h>
#include <WebServer.h>

static WebServer server(80);

static uint8_t selectedPatternIndex = 0;

/**
 * ===================Private Functions===================
 */

/**
 * @brief Handle pattern selection requests from the web client.
 */
static void handlePatternSelect()
{
    // Check if the "pattern" argument is present in the request
    if(server.hasArg("pattern"))
    {
        uint8_t requestedIndex = server.arg("pattern").toInt();

        // Validate the requested index and update the selected pattern index
        if(requestedIndex < patternRegistryGetCount())
        {
            selectedPatternIndex = requestedIndex;
        }
        else
        {
            Serial.print("Invalid pattern index received: ");
            Serial.println(requestedIndex);
            server.send(400, "text/plain", "Invalid pattern index");
            return;
        }

        // Load the selected pattern into the machine controller
        const pattern_entry_t *pattern = patternRegistryGet(selectedPatternIndex);

        if(pattern != nullptr)
        {
            machineControllerLoadPattern(pattern->filename);

            Serial.print("Loaded: ");
            Serial.println(pattern->name);
        }

        Serial.print("Selected Pattern: ");
        Serial.println(selectedPatternIndex);
    }

    server.send(200, "text/plain", "Pattern loaded");
}

static void handleRoot()
{
    File file = SPIFFS.open(
        "/index.html",
        FILE_READ
    );

    if (!file)
    {
        server.send(
            404, "text/plain", "index.html not found"
        );

        return;
    }

    server.streamFile(file, "text/html");
    file.close();
}

static void handleStatus()
{
    /* Start of JSON Data 
     * JSON structure : 
     * {
     *  state:IDLE/RUNNING/WAIT_FOR_NEXT_WHEEL/COMPLETED/ERROR,
     *  wheel:currentWheelIndex,
     *  step:currentStepIndex,
     *  progress:currentProgressPercentage,
     *  patternIndex:selectedPatternIndex,
     *  patternName:selectedPatternName
     * } 
     */
    String json = "{";

    /* Current state */
    json += "\"state\":\"";

    switch (machineControllerGetState())
    {
        case MACHINE_STATE_IDLE:
            json += "IDLE";
            break;

        case MACHINE_STATE_READY:
            json += "READY";
            break;
        
        case MACHINE_STATE_RUNNING:
            json += "RUNNING";
            break;
        
        case MACHINE_STATE_WAIT_FOR_NEXT_WHEEL:
            json += "WAIT_FOR_NEXT_WHEEL";
            break;

        case MACHINE_STATE_ABORTED:
            json += "ABORT";
            break;
        
        case MACHINE_STATE_COMPLETED:
            json += "COMPLETED";
            break;
        
        case MACHINE_STATE_ERROR:
            json += "ERROR";
            break;
    }

    json += "\",";

    /* Current wheel data */
    json += "\"wheel\":";
    json += String(machineControllerGetWheelIndex()); json += ",";

    /* Current step data */
    json += "\"step\":";
    json += String(machineControllerGetStepIndex()); json += ",";

    /* Progress */
    json += "\"progress\":";
    json += String(machineControllerGetProgress()); json += ",";

    /* Selected Pattern Index*/
    json += "\"patternIndex\":";
    json += String(selectedPatternIndex); json += ",";

    /* Selected Pattern Name */
    json += "\"patternName\":";
    const pattern_entry_t *selectedPattern = patternRegistryGet(selectedPatternIndex);
    if (selectedPattern != nullptr)
    {
        json += "\""; json += selectedPattern->name; json += "\"";
    }
    else
    {
        json += "\"Unknown\"";
    }

    json += "}";
    /* End of JSON Data */

    server.send(
        200, "application/json", json
    );
}

static void handlePatterns()
{
    String json = "[";

    for(uint8_t i = 0; i < patternRegistryGetCount(); i++)
    {
        const pattern_entry_t *pattern = patternRegistryGet(i);

        if(i > 0)
        {
            json += ",";
        }

        json += "{"; json += "\"id\":"; json += String(i); json += ",";

        json += "\"name\":\""; json += pattern->name; json += "\""; json += "}";
    }

    json += "]";

    server.send(
        200, "application/json", json
    );
}

static void handleCss()
{
    File file = SPIFFS.open(
        "/style.css",
        FILE_READ
    );

    if(!file)
    {
        server.send(
            404,
            "text/plain",
            "style.css not found"
        );
        return;
    }

    server.streamFile(
        file,
        "text/css"
    );

    file.close();
}

static void handleJs()
{
    File file = SPIFFS.open(
        "/script.js",
        FILE_READ
    );

    if(!file)
    {
        server.send(
            404,
            "text/plain",
            "script.js not found"
        );
        return;
    }

    server.streamFile(
        file,
        "application/javascript"
    );

    file.close();
}

static void handlePatternFile()
{
    const char* filename = machineControllerGetCurrentPatternFile();

    if(filename == nullptr || strlen(filename) == 0)
    {
        server.send(404, "/text/plain", "No pattern loaded");
        return;
    }

    File file = SPIFFS.open(filename, FILE_READ);

    if(!file)
    {
        server.send(404, "text/plain", "Pattern file not found");
        return;
    }

    server.streamFile(file, "application/octet-stream");

    file.close();
}

static void handleStart()
{
    if(machineControllerStart())
    {
        Serial.println("start received from web");
        server.send(
            200,
            "text/plain",
            "Machine Started"
        );
    }
    else
    {
        server.send(
            400,
            "text/plain",
            "Machine Not Ready"
        );
    }
}

static void handleAbort()
{
    machineControllerAbort();

    server.send(
        200,
        "text/plain",
        "Aborted"
    );
}

/**
 * ===================Public Functions===================
 */

bool webServerInit()
{
    server.on("/",          handleRoot);
    server.on("/style.css", handleCss);
    server.on("/script.js", handleJs);
    server.on("/status",    handleStatus);
    server.on("/select",    handlePatternSelect);
    server.on("/patterns",  handlePatterns);
    server.on("/pattern", HTTP_GET, handlePatternFile);
    server.on("/start", handleStart);
    server.on("/abort", handleAbort);

    server.begin();
    Serial.println("Web Server Started");

    return true;
}

void webServerUpdate()
{
    server.handleClient();
}