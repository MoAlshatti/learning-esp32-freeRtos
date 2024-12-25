// NOTE: this example is largely inspired by esp-idf own wifi example, but simpler and more documented (look at readme)

#include <stdio.h>
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_log.h"

static const char *TAG = "WIFI";
#define SSID "SSID"  //change to the ssid
#define PASS "PASS"  //change to the pass
#define MAX_TRIES 5

EventGroupHandle_t event_group;
#define success_bit BIT0
#define failure_bit BIT1
uint16_t tries = 0;

void event_handler(void* handler_arg,esp_event_base_t event_base, int32_t event_id, void* event_data){
   

   if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
      esp_wifi_connect();
   }
   else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){   // if connection fails try again until tries == MAX_TRIES
     if(tries < MAX_TRIES){
         ESP_LOGI(TAG,"retrying..\n");
         tries ++;
         esp_wifi_connect();
      }
      else {
         xEventGroupSetBits(event_group,failure_bit); //if tries >= MAX_TRIES set the failure bit
      }
   }
   else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){ // when we have an ip address, set success bit and reset tries
      tries = 0;
      xEventGroupSetBits(event_group,success_bit); 
   }

}


void wifi_connect(void){


   event_group = xEventGroupCreate();
   
   esp_event_loop_create_default();
   esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID, event_handler, NULL);
   esp_event_handler_register(IP_EVENT,ESP_EVENT_ANY_ID, event_handler, NULL);

   esp_netif_init();
   esp_netif_create_default_wifi_sta();

   wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
   esp_wifi_init(&wifi_init_config);   
   esp_wifi_set_mode(WIFI_MODE_STA);

   wifi_config_t wifi_config = { 
      .sta = {
         .ssid = SSID,
         .password = PASS
      },
   };

   esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
   esp_wifi_start();

   EventBits_t event_bits = xEventGroupWaitBits(event_group, success_bit | failure_bit, pdFALSE, pdFALSE, portMAX_DELAY);   
   /*xEventGroupWaitBits returns the value of the bits when a bit (or more) of an event we are waiting for is set, or the block time is done */

   if((event_bits & success_bit) != 0){
      ESP_LOGI(TAG,"connected to WIFI...\n \n \n");
   }
   else if ((event_bits & failure_bit) != 0){
      ESP_LOGI(TAG,"failed to connect \n \n \n");
   }

   esp_event_handler_unregister(WIFI_EVENT,ESP_EVENT_ANY_ID,event_handler);
   esp_event_handler_unregister(IP_EVENT,ESP_EVENT_ANY_ID,event_handler);
   vEventGroupDelete(event_group);
}


void app_main(void)
{
   nvs_flash_init();
   wifi_connect();
  
  
}