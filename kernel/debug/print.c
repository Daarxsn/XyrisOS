#include "print.h"

#include "../graphics/font.h"

#define DEBUG_LEFT_MARGIN 20
#define DEBUG_TOP_MARGIN  20
#define DEBUG_LINE_HEIGHT 12

#define DEBUG_TEXT_COLOR 0xFFFFFF

static int cursor_x = DEBUG_LEFT_MARGIN;
static int cursor_y = DEBUG_TOP_MARGIN;

void debug_print_init(void)
{
    cursor_x = DEBUG_LEFT_MARGIN;
    cursor_y = DEBUG_TOP_MARGIN;
}

void debug_set_cursor(int x, int y)
{
    cursor_x = x;
    cursor_y = y;
}

void debug_print(const char *text)
{
    if (!text)
        return;

    font_draw_string(
        cursor_x,
        cursor_y,
        text,
        DEBUG_TEXT_COLOR
    );

    while (*text)
    {
        if (*text == '\n')
        {
            cursor_x = DEBUG_LEFT_MARGIN;
            cursor_y += DEBUG_LINE_HEIGHT;
        }
        else
        {
            cursor_x += FONT_WIDTH;
        }

        text++;
    }
}

void debug_print_line(const char *text)
{
    debug_print(text);

    cursor_x = DEBUG_LEFT_MARGIN;
    cursor_y += DEBUG_LINE_HEIGHT;
}