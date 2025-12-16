#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "Hello from app_main");
    ESP_LOGI(TAG, "Hello from app_main2");
}
