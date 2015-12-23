The user program dagger.c initializes function name, data, stack position of user program,  frequency of device, and then task_create system call is called.

In task_create we check if number of tasks are more than 63, and user has initialized valid stack addresses within bounds of user stack start and end addresses, we then sort the input tasks, and initialize the runqueues and devices to be null and the devices match values to be current system time plus the device frequency of each device, we then initialize the schedular and clear the run list and sets them to empty. Here we check for schedulability using UB TEST which obtains values from the task structure to compute the values for the test. The user tasks are then initialized to system tcb, sending function in r4, data in r5, stack position in r6 and kernel stack in sp,lr is given launch task. After initializing tcb we add them to run queue by calling runqueue_add, where group and position in group are found and added to group_run_bits and run_bits.

After that we call dispatch_nosave from create task, the interrupts are disabled first, because context switching is called in this function. We first find highest priority task  by calling highest_priority function, remove it from run queue and add it to current task. For the first we only do ctx_switch_half because we don’t have any previous task to be stored, we pass cur_tcb context and load them, current task is launched, launch task then enable interrupts.

In our C_IRQ_handler we call dev_update which is called on each timer interrupt, dev update determine that the interrupt corresponds to the event frequency of a device. If the interrupt corresponded to the interrupt frequency of a device, then we update the next match values for those tasks and remove them from sleep queue, adding it all to run queue. These tasks are all ready to run now. If there were task added when dev_update is called, the flag is set and dispatch_save is called.

In dispatch_save adds the currently running task  to run queue and then fetches highest priority task, removing it from run queue and making it currently running task.Next ctx_switch_full is called where we store the previous task and load the current highest priority task. All this happens only if current task is not the highest priority task.

After finishing an iteration, user program calls event wait, which is then calling dev_wait. In dev_wait we get the current task and put it to the sleep queue of the device and then call dispatch_sleep. 

In dispatch_sleep we fetch the highest priority task, remove it from run queue and call ctx_switch_full, storing previous task and removing current task.

Our mutex is initialized in kernel, where we make gtMutex available and make our gtMutexpSleep_queue NULL. If our user program calls we mutex_create, we find a gtMutex that is available in the gtMutex.bAvailable array, makes to false.

When user calls, mutex_lock, we disable interrupt then check for the availiabilty, if available, and if gtMutex[mutex].bLock is not already locked by some other task, the task is assigned the mutex and the tasks priority is set to maximum priority, if blocked then the task is added to mutex sleep queue where it waits for mutex to be unlocked, dispatch sleep is called then. In case of mutex_unlock, we remove the other tasks of that mutex from sleep queue, and add it to run queue. If the task holds no other mutex we restore the native priority of the task back from the highest priority alloted during mutex_lock.

