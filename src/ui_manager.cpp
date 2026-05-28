#include "ui_manager.h"
#include "lcd_display.h"
#include "keypad_input.h"

/* ============================================================
 * PATTERN LIST
 * ============================================================
 *
 * This array stores the names of all available patterns.
 *
 * Each index represents:
 * - Pattern ID
 * - Menu position
 *
 * Example:
 * patternList[0] -> "Heart"
 * patternList[1] -> "Star"
 */

static const char *patternList[] = 
{
    "Heart",
    "Star",
    "Water",
    "Fire",
    "Sun"
};

/* ============================================================
 * PATTERN COUNT
 * ============================================================
 *
 * Calculates total number of patterns automatically.
 *
 * sizeof(patternList)
 *     -> Total array size in bytes
 *
 * sizeof(patternList[0])
 *     -> Size of one element
 *
 * Total patterns =
 * total array size / single element size
 */

static const uint8_t patternCount =
    sizeof(patternList) / sizeof(patternList[0]);

/* ============================================================
 * UI STATE VARIABLES
 * ============================================================
 */

/*
 * Stores the current UI state.
 *
 * Example:
 * - Pattern selection screen
 * - Running screen
 * - Error screen
 */
static ui_state_e currentState =
    UI_STATE_PATTERN_SELECT;

/*
 * Stores the currently selected pattern index.
 *
 * Example:
 * currentSelection = 2
 * -> "Water" selected
 */
static uint8_t currentSelection = 0;

/*
 * Stores the first visible item in LCD.
 *
 * Since LCD can only show:
 * - 2 rows
 *
 * displayOffset determines which patterns
 * are currently visible.
 *
 * Example:
 *
 * displayOffset = 0
 * Shows:
 * Heart
 * Star
 *
 * displayOffset = 2
 * Shows:
 * Water
 * Fire
 */
static uint8_t displayOffset = 0;

/* ============================================================
 * PRIVATE FUNCTIONS
 * ============================================================
 */

/**
 * @brief Renders the pattern selection menu on LCD.
 *
 * This function:
 * - Clears LCD
 * - Displays two visible pattern names
 * - Displays cursor ('>') for selected pattern
 */
static void _uiRenderPatternMenu()
{
    /* Clear LCD before drawing new content */

    lcdDisplayClear();

    /*
     * LCD has only 2 rows.
     *
     * So loop through:
     * row 0
     * row 1
     */
    for(uint8_t row = 0; row < 2; row++)
    {
        /*
         * Determine which pattern should
         * be displayed on this row.
         *
         * Example:
         * displayOffset = 2
         *
         * row 0 -> pattern 2
         * row 1 -> pattern 3
         */
        uint8_t patternIndex =
            displayOffset + row;

        /*
         * Prevent accessing outside array.
         */
        if(patternIndex >= patternCount)
        {
            continue;
        }

        /*
         * Print cursor character.
         *
         * If current pattern is selected:
         * print ">"
         *
         * Otherwise:
         * print blank space
         */
        lcdDisplayPrint(
            0,
            row,
            (patternIndex == currentSelection)
            ? ">"
            : " "
        );

        /*
         * Print pattern name.
         *
         * Column = 1 because:
         * column 0 used for cursor.
         */
        lcdDisplayPrint(
            1,
            row,
            patternList[patternIndex]
        );
    }
}

/* ============================================================
 * PUBLIC FUNCTIONS
 * ============================================================
 */

bool uiManagerInit(void)
{
    _uiRenderPatternMenu();

    return true;
}


void uiManagerUpdate(void)
{
    /*
     * Read keypad action.
     *
     * Example:
     * - UP
     * - DOWN
     * - SELECT
     */
    keypad_action_e action =
        keypadRead();

    /*
     * Check current UI state.
     */
    switch(currentState)
    {
        /* ====================================================
         * PATTERN SELECTION SCREEN
         * ====================================================
         */
        case UI_STATE_PATTERN_SELECT:
        {
            /*
             * Handle keypad actions.
             */
            switch(action)
            {
                /* ============================================
                 * MOVE CURSOR UP
                 * ============================================
                 */
                case KEYPAD_ACTION_UP:
                {
                    /*
                     * Prevent going above first pattern.
                     */
                    if(currentSelection > 0)
                    {
                        /*
                         * Move selection upward.
                         */
                        currentSelection--;

                        /*
                         * If selection moved above
                         * current visible window,
                         * scroll menu upward.
                         */
                        if(currentSelection <
                           displayOffset)
                        {
                            displayOffset--;
                        }

                        /*
                         * Redraw menu.
                         */
                        _uiRenderPatternMenu();
                    }

                    break;
                }

                /* ============================================
                 * MOVE CURSOR DOWN
                 * ============================================
                 */
                case KEYPAD_ACTION_DOWN:
                {
                    /*
                     * Prevent going beyond last pattern.
                     */
                    if(currentSelection < (patternCount - 1))
                    {
                        /*
                         * Move selection downward.
                         */
                        currentSelection++;

                        /*
                         * If selection moves beyond
                         * visible rows,
                         * scroll display downward.
                         *
                         * LCD has 2 visible rows.
                         */
                        if(currentSelection >= (displayOffset + 2))
                        {
                            displayOffset++;
                        }

                        _uiRenderPatternMenu();
                    }

                    break;
                }

                /* ============================================
                 * SELECT CURRENT PATTERN
                 * ============================================
                 */
                case KEYPAD_ACTION_SELECT:
                {
                    /*
                     * Change UI state.
                     */
                    currentState =
                        UI_STATE_RUNNING;

                    /*
                     * Clear LCD.
                     */
                    lcdDisplayClear();

                    /*
                     * Display startup message.
                     */
                    lcdDisplayPrint(
                        0,
                        0,
                        "START"
                    );

                    /*
                     * Display selected pattern name.
                     */
                    lcdDisplayPrint(
                        0,
                        1,
                        patternList[currentSelection]
                    );

                    break;
                }

                default:
                    break;
            }

            break;
        }

        /* ====================================================
         * MACHINE RUNNING SCREEN
         * ====================================================
         */
        case UI_STATE_RUNNING:
        {
            /*
             * Future:
             * - Show progress
             * - Show wheel number
             * - Show step number
             */
            break;
        }

        /* ====================================================
         * ERROR SCREEN
         * ====================================================
         */
        case UI_STATE_ERROR:
        {
            /*
             * Future:
             * - Display error messages
             * - Recovery options
             */
            break;
        }

        default:
            break;
    }
}

/**
 * @brief Returns currently selected pattern index.
 *
 * Example:
 * return 2 -> "Water"
 */
uint8_t uiManagerGetSelectedPattern()
{
    return currentSelection;
}