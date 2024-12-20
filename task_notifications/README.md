First of all, lets talk about notification array. Each task has a notification array, but the task can block only on one notifcation from the array at a time.
So for many macros we will use, there is a version of them appended with **Indexed**, this means the function can be used to specify any slot in the notification array, the versions that do not include this postfix defaults to the index 0. the number of indices in the array is stored in configTASK_NOTIFICATION_ARRAY_ENTRIES. Each task also has a state, it can either be pending or not pending.


To send a notification to a task from another task, there are a few functions/macros we can use: <br>
1- **xTaskNotifyGive**: increments the value by 1 and send it<br>
2- **xTaskNotify**: more general than **xTaskNotifyGive**, allows you to choose what to do with the value, whether to change it or increment it or else. To do that, you have to choose the new value in ulValue and the action in eAction, the actions are predfined <br>
a- eNoAction : value not updated <br>
b- eSetBits: notification value bitwised with ulValue.<br>
c- eIncrement
d- eSetVAlueWithOverwrite: force overwrites notification value to ulValue<br>
e- eSetValueWithoutOverwrite: only sets the value if notification doesnt have a value <br><br>

3- **ulTaskNotifyTake**: Decrements the value by 1, it also blocks for a specified number of ticks.<br>
4- **xTaskNotifyWAit**: allows you to choose what to do the notification value when recieved, ulBitsToClearOnEntry allows you to choose specific bits 
in the notification value to set to 0 right after it recieves it and before you can pull the value out. ulBitsToClearOnExit does the same but after you pull the  value out.  to pull out the value, pass a pointer to a buffer to the pulNotificationValue parameter.<br>
5-**xTaskNotifyStateClear**: clears the index from the pending state.