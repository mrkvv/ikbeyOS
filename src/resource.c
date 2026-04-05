#include "rtos_api.h"
#include "sys.h"

void _GetResource(TResource rid)
{
    if (gResources[rid].locked) {
        /* PIP: поднять приоритет владельца до ceiling */
        char owner = gResources[rid].owner;
        char ceiling = gResources[rid].ceiling_priority;

        if (ceiling > gTaskQueue[owner].current_priority) {
            gTaskQueue[owner].current_priority = ceiling;
            /* Перепланирование */
            _Schedule(ceiling);
        }

        /* Заблокировать текущую задачу на ресурсе */
        /* ... */
    }
    else {
        /* Захватить ресурс */
        gResources[rid].locked = 1;
        gResources[rid].owner = gRunningTask;
    }
}

void _ReleaseResource(TResource rid)
{
    /* Восстановить исходный приоритет владельца */
    char owner = gResources[rid].owner;
    gTaskQueue[owner].current_priority = gTaskQueue[owner].priority;

    /* Освободить ресурс */
    gResources[rid].locked = 0;
    gResources[rid].owner = _NULL;

    /* Перепланирование */
    _Schedule(gTaskQueue[owner].priority);
}