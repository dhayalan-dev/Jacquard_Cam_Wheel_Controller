#include "web_server.h"
#include "pattern_registry.h"
#include "machine_controller.h"
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

    server.sendHeader("Location", "/");

    server.send(302, "text/plain", "");
}

static void handleRoot()
{
    String html;

    // HTML Start
    html += "<html>";
    html += "<head>";
    html += "<title>Jacquard Controller</title>";
    html += "</head>";

    html += "<body>";

    html += "<h1>Jacquard Controller</h1>";

    html += "<p>System Online</p>";

    html += "<h2>Status</h2>";
    switch(machineControllerGetState())
    {
        case MACHINE_STATE_IDLE:
            html += "<p>IDLE</p>";
            break;

        case MACHINE_STATE_RUNNING:
            html += "<p>RUNNING</p>";
            break;

        case MACHINE_STATE_WAIT_FOR_NEXT_WHEEL:
            html += "<p>WAITING FOR WHEEL CHANGE</p>";
            break;

        case MACHINE_STATE_COMPLETED:
            html += "<p>COMPLETED</p>";
            break;

        default:
            html += "<p>ERROR</p>";
            break;
    }

    const pattern_entry_t *selectedPattern = patternRegistryGet(selectedPatternIndex);
    html += "<p>Current Pattern : ";
    if(selectedPattern != nullptr)
    {
        html += selectedPattern->name;
    }
    else
    {
        html += "None";
    }
    html += "</p>";

    html += "<h2> Available Patterns</h2>";
    html += "<form action='/select' method='GET'>";
    html += "<select name='pattern'>";
    for(uint8_t i = 0; i < patternRegistryGetCount(); i++)
    {
        const pattern_entry_t *pattern = patternRegistryGet(i);

        html += "<option value='";
        html += String(i);              // Converting Index to string
        html += "'";

        if( i == selectedPatternIndex)
        {
            html += " selected";
        }
        html += ">";
        html += pattern->name;
        html += "</option>";
    }
    html += "</select>";

    html += "<br><br>";

    html += "<input "
            "type='submit' "
            "value='Load Pattern'>";
    html += "</form>";

    html += "</body>";
    html += "</html>";
    // HTML End

    server.send(
        200,
        "text/html",
        html
    );
}

/**
 * ===================Public Functions===================
 */

bool webServerInit()
{
    server.on("/", handleRoot);
    server.on("/select", handlePatternSelect);

    server.begin();
    Serial.println("Web Server Started");

    return true;
}

void webServerUpdate()
{
    server.handleClient();
}