#include <stdio.h>
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "freertos/task.h"
static const char* TAG = "task_notification";

TaskHandle_t t1 = NULL;
TaskHandle_t t2 = NULL;

void task1(void *arg){
    uint32_t recieved;
    while (1){
        vTaskDelay(500/portTICK_PERIOD_MS); // for some reason it hard resets if the delay is after the notifyGive call!
        xTaskNotifyGive(t2);    // increments notification value by 1
        xTaskNotifyWait(0,0,&recieved,portMAX_DELAY);
        ESP_LOGI(TAG,"task1 recieved value %lu\n",recieved);
    }
}

void task2(void *arg){
    uint32_t recieved;
    while (1){
        recieved = ulTaskNotifyTake(pdFALSE,portMAX_DELAY); //setting it to pdTRUE clears the value when done, setting to pdFALSE decrements it by 1
        ESP_LOGI(TAG,"task2 recieved value %lu \n",recieved);
        vTaskDelay(2000/portTICK_PERIOD_MS);    // set to 500 to see how it recieves immedietly after task 1 sends,
    }
}

void task3(void *arg){
    uint32_t value = 0;
    while (1){
        vTaskDelay(1000/portTICK_PERIOD_MS);
        xTaskNotify(t1,value,eSetValueWithoutOverwrite);   //sending value to task 1, overwriting any existing value
        value = (value + 1) % 31;
    }
}


void app_main(void)
{
    xTaskCreate(task1,"task1",4000,NULL,2,&t1);
    xTaskCreate(task2,"task2",4000,NULL,1,&t2);
    xTaskCreate(task3,"task3",4000,NULL,1,NULL);

    while (1){
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }

}