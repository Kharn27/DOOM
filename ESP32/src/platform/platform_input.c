#include "platform_input.h"

#include "doomdef.h"

#define GPIO_BTN_UP 0
#define GPIO_BTN_DOWN 1
#define GPIO_BTN_LEFT 2
#define GPIO_BTN_RIGHT 3
#define GPIO_BTN_FIRE 4
#define GPIO_BTN_USE 5
#define GPIO_BTN_STRAFE 6
#define GPIO_BTN_SPEED 7

typedef struct
{
    int gpio;
    int keycode;
    byte buttons;
    char forwardmove;
    char sidemove;
    short angleturn;
} platform_input_mapping_t;

static const platform_input_mapping_t platform_input_mappings[] = {
    {GPIO_BTN_UP, KEY_UPARROW, 0, 1, 0, 0},
    {GPIO_BTN_DOWN, KEY_DOWNARROW, 0, -1, 0, 0},
    {GPIO_BTN_LEFT, KEY_LEFTARROW, 0, 0, 0, 1},
    {GPIO_BTN_RIGHT, KEY_RIGHTARROW, 0, 0, 0, -1},
    {GPIO_BTN_FIRE, KEY_RCTRL, BT_ATTACK, 0, 0, 0},
    {GPIO_BTN_USE, ' ', BT_USE, 0, 0, 0},
    {GPIO_BTN_STRAFE, KEY_RALT, 0, 0, 0, 0},
    {GPIO_BTN_SPEED, KEY_RSHIFT, 0, 0, 0, 0},
};

int platform_input_read(event_t *event, ticcmd_t *ticcmd)
{
    (void)event;
    (void)ticcmd;
    (void)platform_input_mappings;

    // TODO: Poll GPIOs and translate to event_t/ticcmd_t using
    // platform_input_mappings.
    return 0;
}

void platform_input_init(void)
{
    // TODO: Configure GPIOs for button inputs.
}
