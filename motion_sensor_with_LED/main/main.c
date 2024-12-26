#include <stdio.h>
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED 17
#define SENSOR 22
static const char *TAG = "sensor";

TaskHandle_t led_task_handle;

void IRAM_ATTR sensor_intr(void *args){
    vTaskNotifyGiveFromISR(led_task_handle,0);
    portYIELD_FROM_ISR(pdTRUE);
}

void led_task(void *args){
    uint16_t state = 0;
    while(1){
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        state = gpio_get_level(LED);
        if (state){
            gpio_set_level(LED,0);
        }
        else {
            gpio_set_level(LED,1);
        }
        ESP_LOGI(TAG,"recieved an interrupt, state changed from %d\n\n",state);
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

void sensor_init(void){
    gpio_reset_pin(SENSOR);
    gpio_config_t sensor_config = {.mode = GPIO_MODE_INPUT,
                                   .intr_type = GPIO_INTR_POSEDGE,
                                   .pin_bit_mask = 1ULL << SENSOR, 
                                   .pull_up_en = GPIO_PULLUP_DISABLE, 
                                   .pull_down_en = GPIO_PULLDOWN_DISABLE};

    gpio_config(&sensor_config);
}

void led_init(void){
    gpio_reset_pin(LED);
    gpio_set_direction(LED,GPIO_MODE_INPUT_OUTPUT);
}

void app_main(void)
{
    sensor_init();
    led_init();

    xTaskCreate(led_task,"led task",5000,NULL,3,&led_task_handle);

    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    gpio_isr_handler_add(SENSOR,sensor_intr,NULL);

    while (1){
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }

}