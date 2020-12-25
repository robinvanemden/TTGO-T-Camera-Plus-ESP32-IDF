#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_attr.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "driver/gpio.h"

// screen
#include "st7789.h"

// camera
#include "app_camera.h"
#include "esp_camera.h"

#define INTERVAL 400
#define WAIT vTaskDelay(INTERVAL)

camera_fb_t *fb = NULL;

const int pixel_size = sizeof(uint16_t);
const int screen_width = 240;
const int screen_height = 240;
const int screen_left = 40 * pixel_size;
const int camera_width = 320;

EXT_RAM_ATTR uint16_t colors[240];

void CamToScreen(TFT_t *dev, int width, int height)
{
	fb = esp_camera_fb_get();
	for (int row = 0; row < screen_height; row++){
		memcpy(colors, fb->buf + 1 + screen_left + (row * pixel_size * camera_width), pixel_size * screen_width);
		lcdDrawMultiPixels(dev, 0, row, screen_width, colors);
	}
	esp_camera_fb_return(fb);
	fb = NULL;
}

void Run(void *pvParameters)
{
	TFT_t dev;
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO, CONFIG_BL_GPIO);
	lcdInit(&dev, CONFIG_WIDTH, CONFIG_HEIGHT, CONFIG_OFFSETX, CONFIG_OFFSETY);

	while (1)
	{
		CamToScreen(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
	}

	// never reach
	while (1)
	{
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

void app_main(void)
{
	app_camera_init();
	xTaskCreate(Run, "Run", 1024 * 6, NULL, 2, NULL);
}
