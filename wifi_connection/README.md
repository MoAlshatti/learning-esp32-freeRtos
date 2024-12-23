### Event groups

First lets talk about how event groups work. We define an event handle, and then create an event group using the **xEventGroupCreate** function which returns the event handle. Now we want the event group to signal an event, we do that by creating bit variables, bit1_var = BIT0; and bit2_var = BIT1; . Now Lets say we have a function that waits for an event then does something, so it calls the **xEventGroupWaitBits** which takes the following parameters <br>
1-**event group handle**<br>
2-**bit value we want to wait on**: so if we have two bits (potentially more), we OR them together, so under the hood it looks like (0001 | 0010) = 0011 .<br>
3- **whether to clear on exit**: if set to pdTRUE, all bits are cleared<br>
4- **whether to wait for all bits**: if set to pdTRUE, the function will wait for all bits before continuing, if set to pdFALSE, any change to any bit will make it continue.<br>
5- **wait time in ticks** <br>
Now we need to send the bit signal from somewhere to signal an event to the wait function, so we call the **xEventGroupSetBits** function, which takes an event group handle, and bit variable i.e. bit1_var . <br><br>

### Event loops and Default event loops

**Event loops** are seperate from event groups. Event loops are like interrupts but on software level. As in you create a loop by calling **esp_event_loop_create** and give it a loop handle and loop arguments, and you create event handler functions for each event (or one handler for more than one event), then you register the handler to the loop using the **esp_event_handler_register_with** function, which takes the loop handler, the event and the handler function among other things. To post events to the loop we use **esp_event_post_to** function. The loop then monitors the events and whenever an event occurs it queues the handler function. **Note**: There are more details on declaring event bases and event IDs that I wont go through here.<br>
**Note**: for more details about event base and event ID, https://tinyurl.com/yvpjhyu7
<br><br>

**Default event loops** are the same but for system events, like WIFI. since there is only one default loop, the API functions dont need a loop handler as an argument, other than that they are the same, so we have **esp_event_loop_create_default**, **esp_event_handler_register**, and **esp_event_post**. We will use pre-existing event bases and IDs from the libraries.



### NVS 

what is nvs and why do we need to initialize it? nvs stands for non volatile storage, wtf does that mean? well, it basically means that the storage retains the value even after a poweroff, unlike RAM for instance. So **esp_err_t nvs_flash_init** just initializes the NVS partition in memory, for more details https://tinyurl.com/4up8t7uk .


### Connecting to WIFI

**esp_netif_init** function: initializes IP/TCP stack.<br>
**esp_netif_create_default_wifi_sta** function: The API creates esp_netif object with default WiFi station config, attaches the netif to wifi and registers wifi handlers to the default event loop (taken from the func description). This is why we don't need to post any event to the default loop in our case, its abstracted away from us. <br>
**esp_wifi_init** function: Initialize WiFi Allocate resource for WiFi driver, such as WiFi control structure, RX/TX buffer, WiFi NVS structure etc. This WiFi also starts WiFi task (taken from func description). This function takes a pointer to a **wifi_init_config_t** struct. <br>


