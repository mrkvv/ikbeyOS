#include "rtos_api.h"
#include "sys.h"

void _Start(_pfn task, char priority)
{
    int i;

    /* Инициализация очередей */
    gTaskQueueHead = _NULL;
    gRunningTask = _NULL;

    /* Список свободных TCB */
    gFreeTask = 0;
    for (i = 0; i < TASK_MAX - 1; i++)
        gTaskQueue[i].link = i + 1;
    gTaskQueue[TASK_MAX - 1].link = _NULL;

    /* Инициализация ресурсов */
    for (i = 0; i < RESOURCE_MAX; i++) {
        gResources[i].owner = _NULL;
        gResources[i].locked = 0;
        gResources[i].ceiling_priority = 0;
    }

    /* Сохраняем стек для ShutdownOS */
    gReturnSP = GetSP();

    /* Активируем первую задачу */
    _Activate(task, priority);
}

void ShutdownOS(void)
{
    SetSP(gReturnSP);
    return;
}