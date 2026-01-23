#include "d_main.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_psram.h"

#include "platform/platform_fs.h"
#include "platform/platform_input.h"
#include "platform/platform_lcd.h"

static const char *TAG = "doom_app";

static void platform_init(void)
{
    esp_err_t psram_err = esp_psram_init();
    if (psram_err != ESP_OK)
    {
        ESP_LOGW(TAG, "PSRAM init failed: %s", esp_err_to_name(psram_err));
    }

    platform_lcd_init();
    platform_fs_init();
    platform_input_init();
}

void app_main(void)
{
    platform_init();
    D_DoomMain();
}
