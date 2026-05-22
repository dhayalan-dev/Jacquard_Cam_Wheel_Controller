#include "patterns.h"

#include <SPIFFS.h>

bool initPatternFS()
{
    if(!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS Mount Failed");
        return false;
    }

    Serial.println("SPIFFS Mounted");

    return true;
}

bool loadPattern(
    const char *filename,
    pattern_t &pattern
)
{
    File file = SPIFFS.open(filename, "rb");

    if(!file)
    {
        Serial.println("Pattern Open Failed");
        return false;
    }

    size_t bytesRead =
        file.read(
            (uint8_t*)pattern.rows,
            sizeof(pattern.rows)
        );

    file.close();

    if(bytesRead != sizeof(pattern.rows))
    {
        Serial.println("Pattern Read Error");
        return false;
    }

    return true;
}

bool getCell(
    const pattern_t &pattern,
    uint8_t row,
    uint8_t col
)
{
    return (
        pattern.rows[row]
        >> (17 - col)
    ) & 0x01;
}