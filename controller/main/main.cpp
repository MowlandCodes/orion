#include "esp_log.h"

static const char *TAG = "MAIN";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Hello from app_main");
    ESP_LOGI(TAG, "Hello from app_main2");
}
