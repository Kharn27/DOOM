#include "i_net.h"

#include "d_net.h"
#include "doomstat.h"

extern doomcom_t *doomcom;

void I_InitNetwork(void)
{
    // No network support for ESP32 v1.
    doomcom = NULL;
    netgame = false;
}

void I_NetCmd(void)
{
}
