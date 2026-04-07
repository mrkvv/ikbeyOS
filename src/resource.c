#include <stdio.h>
#include "os.h"
#include "os_types.h"
#include "task.h"

/* ============ КОНФИГУРАЦИЯ ============ */

#define RESOURCE_MAX        16
#define _NULL             -1


/* ============ СТРУКТУРА РЕСУРСА ============ */

typedef struct {
    int locked; // 0 - свободен, 1 - захвачен
    int owner;
    int waiting_task; // задача, ожидающая этот ресурс (-1 если нет)
} TResourceStruct;


/* ============ ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ============ */

static TResourceStruct gResources[RESOURCE_MAX];


void InitRes(TResource ResNum) {
    if (ResNum < 0 || ResNum >= RESOURCE_MAX) {
        printf("[RES] InitRes(%d) ERROR: Invalid resource number\n", ResNum);
        return;
    }

    gResources[ResNum].locked = 0;
    gResources[ResNum].owner = _NULL;
    gResources[ResNum].waiting_task = _NULL;

    printf("[RES] InitRes(%d) called\n", ResNum);
}

void PIP_GetRes(TResource ResNum) {
    printf("[PIP] PIP_GetRes(%d) called\n", ResNum);

    if (ResNum < 0 || ResNum >= RESOURCE_MAX) {
        printf("[PIP] ERROR: Invalid resource %d\n", ResNum);
        return;
    }

    int current_task = gRunningTask;
    if (current_task == _NULL) {
        printf("[PIP] ERROR: No task is running right now!\n");
        return;
    }

    // ресурс свободен => отдаем
    if (!gResources[ResNum].locked) {

        gResources[ResNum].locked = 1;
        gResources[ResNum].owner = current_task;
        gResources[ResNum].waiting_task = _NULL;
        gTaskQueue[current_task].resource_owner = ResNum;

        printf("[PIP] Task %d captures resource %d\n", current_task, ResNum);
        return;
    }

    int owner = gResources[ResNum].owner;

    // запрет вложенного захвата
    if (owner == current_task) {
        printf("[PIP] ERROR: Nested capture of the same resource %d by task %d\n", ResNum, current_task);
        return;
    }

    printf("[PIP] Resource %d locked by task %d, task %d must wait\n", ResNum, owner, current_task);

    int current_prio = GetTaskPriority(current_task);
    int owner_prio = GetTaskPriority(owner);

    if (current_prio < owner_prio) { // наша задача круче задачи держателя ресурса
        printf("[PIP] Priority inheritance: improving task %d priority from %d to %d\n",
            owner, owner_prio, current_prio);

        gTaskQueue[owner].original_priority = owner_prio;
        SetTaskPriority(owner, current_prio);
    }

    printf("[PIP] Task %d is blocked and awaiting resource %d\n", current_task, ResNum);
    gResources[ResNum].waiting_task = current_task;
    BlockTask(current_task, ResNum);
}


void PIP_ReleaseRes(TResource ResNum) {
    printf("[PIP] PIP_ReleaseRes(%d) called\n", ResNum);

    if (ResNum < 0 || ResNum >= RESOURCE_MAX) {
        printf("[PIP] ERROR: Invalid resource %d\n", ResNum);
        return;
    }

    int current_task = gRunningTask;
    if (current_task == _NULL) {
        printf("[PIP] ERROR: No task is running right now!\n");
        return;
    }

    if (gResources[ResNum].owner != current_task) {
        printf("[PIP] ERROR: Task %d isn't the owner of the resource %d\n", current_task, ResNum);
        return;
    }

    // если меняли приоритет => восстанавливаем
    if (gTaskQueue[current_task].original_priority != 0) {
        int original_prio = gTaskQueue[current_task].original_priority;

        printf("[PIP] Restoring task %d priority from %d to %d\n",
            current_task, GetTaskPriority(current_task), original_prio);
        
        SetTaskPriority(current_task, original_prio);
        gTaskQueue[current_task].original_priority = 0;
    }

    gTaskQueue[current_task].resource_owner = _NULL;


    int waiting_task = gResources[ResNum].waiting_task;
    if (waiting_task != _NULL && GetTaskState(waiting_task) == TASK_BLOCKED) {
        // передаем ресурс и анблокаем задачу
        printf("[PIP] Transferring resource %d to waiting task %d\n", ResNum, waiting_task);

        gResources[ResNum].owner = waiting_task;
        gResources[ResNum].waiting_task = _NULL;
        gTaskQueue[waiting_task].resource_owner = ResNum;

        UnblockTask(waiting_task);
    }
    else {
        printf("[PIP] Resource %d released by task %d and there is no waiters!\n", ResNum, current_task);

        gResources[ResNum].locked = 0;
        gResources[ResNum].owner = _NULL;
        gResources[ResNum].waiting_task = _NULL;
    }

    // Вызываем планировщик для возможного переключения контекста
    OS_Schedule();
}

// не используется, редиректим на нашу пипную реализацию
void GetResource(TResource res) {
    printf("[RES] WARNING: GetResource(%d) isn't supported by ikbeyOS, redirected to PIP_GetRes\n", res);
    PIP_GetRes(res);
}

void ReleaseResource(TResource res) {
    printf("[RES] WARNING: ReleaseResource(%d) isn't supported by ikbeyOS, redirected to PIP_ReleaseRes\n", res);
    PIP_ReleaseRes(res);
}

// тоже не используется, оставлены для совместимости (?)
void InitPVS(TSemaphore S) {
    printf("[SEM] InitPVS(%d) called\n", S);
}

void P(TSemaphore S) {
    printf("[SEM] P(%d) called\n", S);
}

void V(TSemaphore S) {
    printf("[SEM] V(%d) called\n", S);
}