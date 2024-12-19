#include <stdio.h>
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_task.h"

static const char* TAG = "tasks";


void task1(void *args){
    while(1){
        ESP_LOGI(TAG,"task 1\n");
        vTaskDelay(1000/portTICK_PERIOD_MS);    //delay is needed to give the freeRtos scheduler time for other tasks to run
    }
}

void task2(void *args){
    while (1){
        ESP_LOGI(TAG,"task2 \n");
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}



void app_main(void)

{
    TaskHandle_t t1 = NULL; // not used in this example code
    TaskHandle_t t2 = NULL; // not used in this example code
    
    //Params: the function for the task, task name, stack size, params, handler(used to delete the task)
    xTaskCreate(task1,"Task 1",2048,NULL,1,&t1);
    xTaskCreate(task2,"Task 2",2048,NULL,1,&t2);

    while (1){
        vTaskDelay(1000/portTICK_PERIOD_MS);    
    }
}