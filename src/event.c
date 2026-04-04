#include "rtos_api.h"
#include "sys.h"

void _SetEvent(TTask task, TEventMask mask)
{
    char tid = task - gTaskQueue;  /* упрощённо */

    /* Установить биты события */
    /* ... хранить маску событий у задачи ... */

    /* Если задача ждёт эти события, разбудить */
    if (gTaskQueue[tid].state == TASK_WAITING &&
        (gTaskQueue[tid].wait_mask & mask)) {
        gTaskQueue[tid].state = TASK_READY;
        _Schedule(gTaskQueue[tid].priority);
    }
}

void _WaitEvent(TEventMask mask)
{
    gTaskQueue[gRunningTask].wait_mask = mask;
    gTaskQueue[gRunningTask].state = TASK_WAITING;
    _Schedule(0);
}