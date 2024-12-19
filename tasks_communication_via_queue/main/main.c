#include <stdio.h>
#include <esp_mac.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

static const char* TAG = "queues";

#define QUEUE_LENGTH 5

QueueHandle_t queue;

void Task_produce(void *arg){
    uint32_t value = 0;
    while (1){
        xQueueSend(queue,&value,500/portTICK_PERIOD_MS);    // the queue sends the value by value not by reference.
        value = (value +1)%100; //sending values from 0 to 99 then reset
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

void Task_consume(void *arg){
    uint32_t recieved_value;
    while (1){
        xQueueReceive(queue,&recieved_value,500/portTICK_PERIOD_MS);
        ESP_LOGI(TAG,"\t recieved value is:%lu\n",recieved_value);
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    queue = xQueueCreate(QUEUE_LENGTH,sizeof(uint32_t));

    TaskHandle_t t_produce = NULL;
    TaskHandle_t t_consume = NULL;

    xTaskCreate(Task_produce,"producer",2048,NULL,1,&t_produce);
    xTaskCreate(Task_consume,"consumer",2048,NULL,1,&t_consume);

    while(1){
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}