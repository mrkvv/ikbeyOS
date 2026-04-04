#include "rtos_api.h"
#include "sys.h"

void _Activate(_pfn entry, char priority)
{
    int new_task;

    /* Взять свободный TCB */
    new_task = gFreeTask;
    if (new_task == _NULL) return;
    gFreeTask = gTaskQueue[new_task].link;

    /* Инициализировать TCB */
    gTaskQueue[new_task].priority = priority;
    gTaskQueue[new_task].current_priority = priority;
    gTaskQueue[new_task].state = TASK_READY;
    gTaskQueue[new_task].wait_mask = 0;

    /* Создать стек задачи (упрощённо) */
    /* ... выделить стек, сохранить entry ... */

    /* Добавить в очередь готовых (по приоритету) */
    /* ... RMA: вставка по убыванию priority ... */

    /* Вызвать планировщик */
    _Schedule(priority);
}

void _Terminate(void)
{
    gTaskQueue[gRunningTask].state = TASK_SUSPENDED;
    /* Освободить ресурсы, если есть */
    /* ... */
    _Schedule(0);
}

void _Schedule(char new_priority)
{
    /* Preemptive RMA:
       если новая задача имеет приоритет > текущей,
       то переключиться */
    if (gRunningTask != _NULL) {
        if (new_priority > gTaskQueue[gRunningTask].current_priority) {
            _Dispatch();
        }
    }
    else {
        _Dispatch();
    }
}