### Event group

First lets talkl about how events work. We define an event handle, and then create an event group using the **xEventGroupCreate** function which returns the event handle. Now the we want the event group to signal an event, we do that by creating bit variables bit1_var = BIT0; and bit2_var = BIT1; . Now Lets say we have a function that waits for an event, then does something, so it calls the **xEventGroupWaitBits** which takes the following parameters <br>
1-**event group handle**<br>
2-**bit value we want to wait on**: so if we have two bits (potentially more), we OR them together, so under the hood it looks like (0001 | 0010) = 0011 .<br>
3- **whether to clear on exit**: if set to pdTRUE, all bits are cleared<br>
4- **whether to wait for all bits**: if set to pdTRUE, the function will wait for all bits before continuing, if set to pdFALSE, any change to any bit will make it continue.<br>
5- **wait time in ticks** <br>

Now we need to send the bit signal from somewhere to signal an eventto the wait function, so we call the **xEventGroupSetBits** function, which takes an event group handle, and bit variable i.e. bit1_var . <br><br>

### Event loops and Default event loops

Event loops are seperate from event groups.  ....


### NVS 

what is nvs and why do we need to initialize it? nvs stands for non volatile storage, wtf does that mean? well, it basically means that the storage retains the value even after a poweroff, unlike RAM for instance. So **esp_err_t nvs_flash_init** just initializes the NVS partition in memory, for more details https://tinyurl.com/4up8t7uk .


### Connecting to WIFI

