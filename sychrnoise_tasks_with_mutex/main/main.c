#include <stdio.h>
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include <inttypes.h>

static const char* TAG = "mutex_prog";

SemaphoreHandle_t mutex = NULL; //creating a global mutex

void Task1 (void *arg){
    uint32_t *shared_res = (uint32_t*)arg;

    while (1){
        xSemaphoreTake(mutex,portMAX_DELAY);    //portMAX_DELAY is to tell it to wait indefinitely for the mutex to be free then take it
        *shared_res = *shared_res + 1;
        ESP_LOGI(TAG,"task 1, num %lu\n",*shared_res);
        xSemaphoreGive(mutex);
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
    
}

void Task2 (void *arg){
    uint32_t *shared_res = (uint32_t*)arg;
    while(1){
        xSemaphoreTake(mutex,portMAX_DELAY);
        *shared_res = *shared_res + 1;
        ESP_LOGI(TAG,"task 2, num %lu\n",*shared_res);
        xSemaphoreGive(mutex);
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
    
}

void app_main(void)
{
    uint32_t *shared_resource = (uint32_t*)malloc(sizeof(uint32_t));
    *shared_resource = 0;
    mutex = xSemaphoreCreateMutex(); //initalizing the mutex

    TaskHandle_t t1 = NULL;
    TaskHandle_t t2 = NULL;

    xTaskCreate(Task1,"Task1",3000,(void*)shared_resource,1,&t1);
    xTaskCreate(Task2,"Task2",3000,(void*)shared_resource,1,&t2);

    while(1){
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}   