freeRtos is a task based operating system. the kernel scheduler determines what task to run at a single time based on the priority of the task. <br><br>
In the code provided, we notice calling the **vTaskDelay** function, which is needed to give sometime for other tasks to run. <br><br>
the scheduler tries to run tasks seemingly concurrently, so it alternates between tasks until all are finished. you can see this when looking at the output of the program.
