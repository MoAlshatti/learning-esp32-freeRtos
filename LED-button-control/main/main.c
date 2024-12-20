#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "esp_mac.h"
#define BUTTON 26
#define LED 17

static const char* TAG = "LED-Button";


TaskHandle_t led_on_handler;
TaskHandle_t button_pushed_handler;


void led_config(void){
    gpio_reset_pin(LED);
    gpio_set_direction(LED,GPIO_MODE_INPUT_OUTPUT); // configured as input output for gpio_get_level function to work properly

}

void button_config(gpio_config_t * bt){
    bt->mode = GPIO_MODE_INPUT;
    bt->pull_up_en = GPIO_PULLUP_ENABLE;
    bt->pull_down_en = GPIO_PULLDOWN_DISABLE;
    bt->pin_bit_mask = 1ULL << BUTTON;
    bt->intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(bt);
}

void IRAM_ATTR button_ISR(void *arg){
    xTaskResumeFromISR(button_pushed_handler);  // better to use task notification to synchornise with ISR (according to Freertos website)
    portYIELD_FROM_ISR(pdTRUE);
}

void led_on (void *args){
    int state;
    while (1){
        vTaskSuspend(NULL);
        state = gpio_get_level(LED);
        ESP_LOGI(TAG,"level is %d\n",state);
        switch(state){
            case 0 : 
                gpio_set_level(LED,1);
                break;
            case 1 : 
                gpio_set_level(LED,0);
                break;
        }
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

void button_pushed(void *args){
    while (1){
        vTaskSuspend(NULL);
        vTaskResume(led_on_handler);
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    gpio_config_t button_settings;
    button_config(&button_settings);
    led_config();

    xTaskCreate(led_on,"LED",4069,NULL,4,&led_on_handler);
    xTaskCreate(button_pushed,"Button",2048,NULL,4,&button_pushed_handler);

    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    gpio_isr_handler_add(BUTTON,button_ISR,NULL);

    while (1){
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}