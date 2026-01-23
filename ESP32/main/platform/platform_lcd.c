#include "platform_lcd.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_check.h"
#include "esp_lcd_panel_io.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LCD_SPI_HOST SPI2_HOST
#define LCD_H_RES 320
#define LCD_V_RES 240
#define LCD_GAME_HEIGHT 200
#define LCD_V_PAD ((LCD_V_RES - LCD_GAME_HEIGHT) / 2)

#define LCD_GPIO_CS 10
#define LCD_GPIO_CLK 12
#define LCD_GPIO_D0 11
#define LCD_GPIO_D1 13
#define LCD_GPIO_D2 14
#define LCD_GPIO_D3 15
#define LCD_GPIO_RST 16
#define LCD_GPIO_BL 1

#define LCD_PCLK_HZ 40000000

#define LCD_CMD_SWRESET 0x01
#define LCD_CMD_SLPOUT 0x11
#define LCD_CMD_DISPON 0x29
#define LCD_CMD_CASET 0x2A
#define LCD_CMD_RASET 0x2B
#define LCD_CMD_RAMWR 0x2C
#define LCD_CMD_MADCTL 0x36
#define LCD_CMD_COLMOD 0x3A

static const char *TAG = "platform_lcd";
static esp_lcd_panel_io_handle_t lcd_io;
static uint16_t black_band[LCD_H_RES * LCD_V_PAD];

static esp_err_t lcd_send_cmd(uint8_t cmd, const void *param, size_t param_size)
{
    if (!lcd_io)
    {
        return ESP_ERR_INVALID_STATE;
    }
    return esp_lcd_panel_io_tx_param(lcd_io, cmd, param, param_size);
}

static void lcd_set_window(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    uint8_t data[4];

    data[0] = (uint8_t)(x_start >> 8);
    data[1] = (uint8_t)(x_start & 0xFF);
    data[2] = (uint8_t)(x_end >> 8);
    data[3] = (uint8_t)(x_end & 0xFF);
    ESP_ERROR_CHECK(lcd_send_cmd(LCD_CMD_CASET, data, sizeof(data)));

    data[0] = (uint8_t)(y_start >> 8);
    data[1] = (uint8_t)(y_start & 0xFF);
    data[2] = (uint8_t)(y_end >> 8);
    data[3] = (uint8_t)(y_end & 0xFF);
    ESP_ERROR_CHECK(lcd_send_cmd(LCD_CMD_RASET, data, sizeof(data)));
}

static void lcd_send_color(const uint16_t *pixels, size_t pixel_count)
{
    if (!lcd_io || pixel_count == 0)
    {
        return;
    }
    esp_lcd_panel_io_tx_color(lcd_io, LCD_CMD_RAMWR, pixels, pixel_count * sizeof(uint16_t));
}

void platform_lcd_present_rgb565(const uint16_t *framebuffer,
                                 int width,
                                 int height,
                                 int stride)
{
    if (!framebuffer || width <= 0 || height <= 0 || stride <= 0)
    {
        return;
    }

    const int target_width = LCD_H_RES;
    const int target_height = LCD_V_RES;
    const int pad = LCD_V_PAD;

    if (width != target_width || height != LCD_GAME_HEIGHT)
    {
        ESP_LOGW(TAG, "Unexpected framebuffer size %dx%d", width, height);
    }

    if (pad > 0)
    {
        lcd_set_window(0, 0, target_width - 1, pad - 1);
        lcd_send_color(black_band, (size_t)target_width * pad);
    }

    lcd_set_window(0, pad, target_width - 1, pad + height - 1);
    if (stride == width)
    {
        lcd_send_color(framebuffer, (size_t)width * height);
    }
    else
    {
        for (int y = 0; y < height; ++y)
        {
            lcd_send_color(framebuffer + y * stride, (size_t)width);
        }
    }

    if (pad > 0)
    {
        lcd_set_window(0, pad + height, target_width - 1, target_height - 1);
        lcd_send_color(black_band, (size_t)target_width * pad);
    }
}

void platform_lcd_init(void)
{
    esp_err_t err;
    spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_GPIO_CLK,
        .data0_io_num = LCD_GPIO_D0,
        .data1_io_num = LCD_GPIO_D1,
        .data2_io_num = LCD_GPIO_D2,
        .data3_io_num = LCD_GPIO_D3,
        .max_transfer_sz = LCD_H_RES * LCD_V_RES * sizeof(uint16_t),
        .flags = SPICOMMON_BUSFLAG_QUAD,
    };

    err = spi_bus_initialize(LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(err);

    esp_lcd_panel_io_qspi_config_t io_config = {
        .cs_gpio_num = LCD_GPIO_CS,
        .pclk_hz = LCD_PCLK_HZ,
        .trans_queue_depth = 4,
        .user_ctx = NULL,
        .flags = {
            .quad_mode = 1,
        },
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_qspi(LCD_SPI_HOST, &io_config, &lcd_io));

    gpio_config_t bl_cfg = {
        .pin_bit_mask = 1ULL << LCD_GPIO_BL,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&bl_cfg));
    gpio_set_level(LCD_GPIO_BL, 0);

    if (LCD_GPIO_RST >= 0)
    {
        gpio_config_t rst_cfg = {
            .pin_bit_mask = 1ULL << LCD_GPIO_RST,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ESP_ERROR_CHECK(gpio_config(&rst_cfg));
        gpio_set_level(LCD_GPIO_RST, 0);
        vTaskDelay(pdMS_TO_TICKS(20));
        gpio_set_level(LCD_GPIO_RST, 1);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    else
    {
        ESP_ERROR_CHECK(lcd_send_cmd(LCD_CMD_SWRESET, NULL, 0));
        vTaskDelay(pdMS_TO_TICKS(120));
    }

    ESP_ERROR_CHECK(lcd_send_cmd(LCD_CMD_SLPOUT, NULL, 0));
    vTaskDelay(pdMS_TO_TICKS(120));

    uint8_t colmod = 0x55;
    ESP_ERROR_CHECK(lcd_send_cmd(LCD_CMD_COLMOD, &colmod, 1));

    uint8_t madctl = 0x00;
    ESP_ERROR_CHECK(lcd_send_cmd(LCD_CMD_MADCTL, &madctl, 1));

    ESP_ERROR_CHECK(lcd_send_cmd(LCD_CMD_DISPON, NULL, 0));
    vTaskDelay(pdMS_TO_TICKS(20));

    gpio_set_level(LCD_GPIO_BL, 1);
}
