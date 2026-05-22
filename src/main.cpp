#include <Arduino.h>

#include "patterns.h"
#include "design.h"

pattern_t currentTopPattern;
pattern_t currentBottomPattern;

void printPattern(  
    const pattern_t &pattern
)
{
  for(int r = 0; r < PATTERN_SIZE; r++)
  {
    for(int c = 0; c < PATTERN_SIZE; c++)
    {
      Serial.print(getCell(pattern, r, c));
    }

    Serial.println();
  }
}

void setup()
{
    Serial.begin(115200);

    if(!initPatternFS()) return;

    int selectedDesign = 0;

    const design_t &currentDesign =
        designs[selectedDesign];

    Serial.println();
    Serial.println(currentDesign.name);

    if(loadPattern(
        currentDesign.topPatternFile,
        currentTopPattern
    ))
    {
        Serial.println();
        Serial.println("TOP PATTERN");

        printPattern(currentTopPattern);
    }

    if(loadPattern(
        currentDesign.bottomPatternFile,
        currentBottomPattern
    ))
    {
        Serial.println();
        Serial.println("BOTTOM PATTERN");

        printPattern(currentBottomPattern);
    }
}

void loop()
{

}