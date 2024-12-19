to create a mutex in freeRtos,<br>
we use *SemaphoreHandle_t* struct, and *xSemaphoreCreateMutex* function to initialize it. <br> <br>
to acquire/lock the mutex, <br> 
we use *xSemaphoreTake*, which takes two parameters, the mutex struct, and the time it waits for the mutex to become free if its locked. *portMAX_DELAY* can be used for the second parameter to wait indefinitely. <br> <br>
To unlock the mutex we use *xSemaphoreGive* function.

<br> To destroy a mutex use *vSemaphoreDelete*.
