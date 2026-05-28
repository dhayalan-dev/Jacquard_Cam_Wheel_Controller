#include "lcd_display.h"

// LCD instance
static LiquidCrystal lcd(
    LCD_RS_PIN,
    LCD_ENABLE_PIN,
    LCD_D4_PIN,
    LCD_D5_PIN,
    LCD_D6_PIN,
    LCD_D7_PIN
);

bool lcdDisplayInit(void)
{
    lcd.begin(LCD_COLS, LCD_ROWS);
    lcd.clear();
    return true;
}


void lcdDisplayClear(void)
{
    lcd.clear();
}

bool lcdDisplayPrint(
    uint8_t col,
    uint8_t row,
    const char* txt
)
{
    lcd.setCursor(col, row);
    lcd.print(txt);
    return true;
}