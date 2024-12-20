#include <stdio.h>
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/task.h"

#define BUTTON 26
static const char* TAG = "button_config";

TaskHandle_t button_press_h;

void IRAM_ATTR interrupt_h (void *args){    //ISR function
    xTaskResumeFromISR(button_press_h);
    portYIELD_FROM_ISR(pdTRUE);     //forces the scheduler to check if a task with higher priority has been awakened (and directly goes there)
}

void button_pressed (void *args){
    while (1){
        vTaskSuspend(NULL); //task suspends itself, only an interrupt resumes it (when button is pressed)
        ESP_LOGI(TAG,"Button pressed!!\n");
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
    
}

void app_main(void)
{
    gpio_config_t button_conf;
    button_conf.mode = GPIO_MODE_INPUT;
    button_conf.pin_bit_mask = 1ULL << BUTTON;    // left shifting 1 as unsigned long int, bascially sets the Button pin as 1
    button_conf.pull_up_en = GPIO_PULLUP_ENABLE; // only enable pull up when the button is connected to GND and not VCC
    button_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;    // enable pull down if the button is connected to VCC 
    button_conf.intr_type = GPIO_INTR_NEGEDGE;  // trigger the interrupt upon a falling edge (since pullup is enabled the button is low only upon a button press)
    gpio_config(&button_conf);

    xTaskCreate(button_pressed,"task",4000,NULL,4,&button_press_h);

    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);   // ESP_INTR_FLAG_IRAM ensures the interrupt runs from IRAM (for max speed min latency)
    gpio_isr_handler_add(BUTTON,interrupt_h,NULL);

    while (1){
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
