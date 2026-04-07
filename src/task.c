#include <stdio.h>
#include "task.h"
#include "os_types.h"

/* ============ КОНФИГУРАЦИЯ ============ */
#define TASK_MAX        32
#define _NULL           -1

/* ============ ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ============ */
TTaskStruct gTaskQueue[TASK_MAX];
int gRunningTask = _NULL;
int gTaskQueueHead = _NULL;
static int gFreeTaskHead = 0;
int gSystemRunning = 0;

/* ============ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ============ */

static void InitFreeList(void)
{
    int i;
    for (i = 0; i < TASK_MAX - 1; i++) {
        gTaskQueue[i].link = i + 1;
        gTaskQueue[i].id = i;
        gTaskQueue[i].state = TASK_SUSPENDED;
        gTaskQueue[i].entry = NULL;
        gTaskQueue[i].terminated = 0;
        gTaskQueue[i].event_mask = 0;
        gTaskQueue[i].wait_mask = 0;
        gTaskQueue[i].wait_resource = _NULL;
        gTaskQueue[i].resource_owner = _NULL;
        gTaskQueue[i].priority = 0;
        gTaskQueue[i].current_priority = 0;
        gTaskQueue[i].original_priority = 0;
    }
    gTaskQueue[TASK_MAX - 1].link = _NULL;
    gTaskQueue[TASK_MAX - 1].id = TASK_MAX - 1;
    gTaskQueue[TASK_MAX - 1].state = TASK_SUSPENDED;
    gTaskQueue[TASK_MAX - 1].terminated = 0;
    gFreeTaskHead = 0;
}

static int AllocateTCB(void)
{
    int task = gFreeTaskHead;
    if (task == _NULL) return _NULL;

    gFreeTaskHead = gTaskQueue[task].link;
    gTaskQueue[task].link = _NULL;
    gTaskQueue[task].state = TASK_SUSPENDED;
    gTaskQueue[task].entry = NULL;
    gTaskQueue[task].terminated = 0;
    gTaskQueue[task].event_mask = 0;
    gTaskQueue[task].wait_mask = 0;
    gTaskQueue[task].wait_resource = _NULL;
    gTaskQueue[task].resource_owner = _NULL;

    return task;
}

static void FreeTCB(int task)
{
    if (task == _NULL || task >= TASK_MAX) return;

    gTaskQueue[task].link = gFreeTaskHead;
    gTaskQueue[task].state = TASK_SUSPENDED;
    gTaskQueue[task].entry = NULL;
    gTaskQueue[task].terminated = 0;
    gTaskQueue[task].event_mask = 0;
    gTaskQueue[task].wait_mask = 0;
    gFreeTaskHead = task;
}

/* ============ УПРАВЛЕНИЕ ОЧЕРЕДЬЮ (ДЛЯ ДРУГИХ МОДУЛЕЙ) ============ */

void InsertToReadyQueue(int task_index)
{
    int prev = _NULL;
    int curr = gTaskQueueHead;

    if (task_index < 0 || task_index >= TASK_MAX) return;
    if (gTaskQueue[task_index].state == TASK_READY) return;
    if (gTaskQueue[task_index].state == TASK_RUNNING) return;

    // Проверка на дубликат
    int temp = gTaskQueueHead;
    while (temp != _NULL) {
        if (temp == task_index) return;
        temp = gTaskQueue[temp].link;
    }

    // Вставка с сортировкой по приоритету (меньше число = выше приоритет)
    while (curr != _NULL) {
        if (gTaskQueue[task_index].current_priority < gTaskQueue[curr].current_priority) {
            if (prev == _NULL) {
                gTaskQueue[task_index].link = gTaskQueueHead;
                gTaskQueueHead = task_index;
            }
            else {
                gTaskQueue[prev].link = task_index;
                gTaskQueue[task_index].link = curr;
            }
            gTaskQueue[task_index].state = TASK_READY;
            return;
        }
        prev = curr;
        curr = gTaskQueue[curr].link;
    }

    // Вставка в конец
    if (prev == _NULL) {
        gTaskQueueHead = task_index;
    }
    else {
        gTaskQueue[prev].link = task_index;
    }
    gTaskQueue[task_index].link = _NULL;
    gTaskQueue[task_index].state = TASK_READY;
}

void RemoveFromReadyQueue(int task_index)
{
    int prev = _NULL;
    int curr = gTaskQueueHead;

    while (curr != _NULL) {
        if (curr == task_index) {
            if (prev == _NULL) {
                gTaskQueueHead = gTaskQueue[curr].link;
            }
            else {
                gTaskQueue[prev].link = gTaskQueue[curr].link;
            }
            gTaskQueue[task_index].link = _NULL;
            if (gTaskQueue[task_index].state == TASK_READY) {
                gTaskQueue[task_index].state = TASK_SUSPENDED;
            }
            return;
        }
        prev = curr;
        curr = gTaskQueue[curr].link;
    }
}

static int FindHighestPriorityTask(void)
{
    return gTaskQueueHead;
}

/* ============ ФУНКЦИИ ДЛЯ ДРУГИХ МОДУЛЕЙ ============ */

int GetCurrentTask(void)
{
    return gRunningTask;
}

int GetTaskState(int task)
{
    if (task < 0 || task >= TASK_MAX) return TASK_SUSPENDED;
    return gTaskQueue[task].state;
}

int GetTaskPriority(int task)
{
    if (task < 0 || task >= TASK_MAX) return -1;
    return gTaskQueue[task].current_priority;
}

void SetTaskPriority(int task, int new_priority)
{
    if (task < 0 || task >= TASK_MAX) return;
    gTaskQueue[task].current_priority = new_priority;

    // Пересортировка очереди, если задача в ready или running
    if (gTaskQueue[task].state == TASK_READY) {
        RemoveFromReadyQueue(task);
        InsertToReadyQueue(task);
    }
    else if (gTaskQueue[task].state == TASK_RUNNING && task == gRunningTask) {
        // Текущая задача - нужно проверить вытеснение
        RemoveFromReadyQueue(task);
        InsertToReadyQueue(task);
        OS_Schedule();
    }
}

void SetTaskOriginalPriority(int task, int original_priority)
{
    if (task < 0 || task >= TASK_MAX) return;
    gTaskQueue[task].original_priority = original_priority;
}

void SetTaskState(int task, int new_state)
{
    if (task < 0 || task >= TASK_MAX) return;

    int old_state = gTaskQueue[task].state;
    gTaskQueue[task].state = new_state;

    // Если задача была в ready и стала не ready - убираем из очереди
    if (old_state == TASK_READY && new_state != TASK_READY) {
        RemoveFromReadyQueue(task);
    }

    // Если задача стала ready - добавляем в очередь
    if (new_state == TASK_READY && old_state != TASK_READY) {
        InsertToReadyQueue(task);
    }
}

void BlockTask(int task, int resource_id)
{
    if (task < 0 || task >= TASK_MAX) return;

    gTaskQueue[task].wait_resource = resource_id;
    gTaskQueue[task].state = TASK_BLOCKED;
    RemoveFromReadyQueue(task);

    if (task == gRunningTask) {
        gRunningTask = _NULL;
        OS_Schedule();
    }
}

void UnblockTask(int task)
{
    if (task < 0 || task >= TASK_MAX) return;
    if (gTaskQueue[task].state == TASK_BLOCKED) {
        gTaskQueue[task].wait_resource = _NULL;
        gTaskQueue[task].state = TASK_READY;
        InsertToReadyQueue(task);
        OS_Schedule();
    }
}

/* ============ ПЛАНИРОВЩИК ============ */

static void OS_Dispatch(int from, int to);

void OS_Schedule(void)
{
    int next_task = FindHighestPriorityTask();

    if (next_task == _NULL) {
        return;
    }

    if (next_task != gRunningTask) {
        OS_Dispatch(gRunningTask, next_task);
    }

    RemoveFromReadyQueue(next_task);
    gRunningTask = next_task;
    gTaskQueue[gRunningTask].state = TASK_RUNNING;

    // Выполняем задачу
    if (gTaskQueue[gRunningTask].entry != NULL && !gTaskQueue[gRunningTask].terminated) {
        gTaskQueue[gRunningTask].entry(gTaskQueue[gRunningTask].param);
    }

    // Если задача завершилась - TerminateTask сама вызовет планировщик
}

static void OS_Dispatch(int from, int to)
{
    // В реальной ОС здесь было бы сохранение/восстановление контекста
    if (from != _NULL && to != _NULL && from != to) {
        // Переключение контекста
    }
}

/* ============ ПУБЛИЧНОЕ API ИЗ СПЕЦИФИКАЦИИ ============ */

void InitTaskSystem(void)
{
    InitFreeList();
    gTaskQueueHead = _NULL;
    gRunningTask = _NULL;
    gSystemRunning = 0;
    printf("[TASK] System initialized (max tasks: %d)\n", TASK_MAX);
}

int RegisterTask(TTask task_id, void (*entry)(void*), void* param, int priority)
{
    if (task_id < 0 || task_id >= TASK_MAX) return -1;
    if (gTaskQueue[task_id].state != TASK_SUSPENDED) return -1;

    gTaskQueue[task_id].entry = entry;
    gTaskQueue[task_id].param = param;
    gTaskQueue[task_id].priority = priority;
    gTaskQueue[task_id].current_priority = priority;
    gTaskQueue[task_id].original_priority = 0;
    gTaskQueue[task_id].id = task_id;
    gTaskQueue[task_id].terminated = 0;

    return 0;
}

void ActivateTask(TTask task)
{
    if (!gSystemRunning) {
        printf("  System not started yet, task will be activated on StartOS\n");
    }

    if (task < 0 || task >= TASK_MAX) return;

    if (gTaskQueue[task].state == TASK_SUSPENDED) {
        InsertToReadyQueue(task);

        if (gSystemRunning && gRunningTask != _NULL) {
            if (gTaskQueue[task].current_priority < gTaskQueue[gRunningTask].current_priority) {
                if (gTaskQueue[gRunningTask].state == TASK_RUNNING) {
                    gTaskQueue[gRunningTask].state = TASK_READY;
                    InsertToReadyQueue(gRunningTask);
                }
                gRunningTask = _NULL;
                OS_Schedule();
            }
        }
        else if (gSystemRunning) {
            OS_Schedule();
        }
    }
}

void ISRActivateTask(TTask task)
{
    if (task < 0 || task >= TASK_MAX) return;
    if (gTaskQueue[task].state == TASK_SUSPENDED) {
        InsertToReadyQueue(task);
    }
}

void TerminateTask(void)
{
    int finished = gRunningTask;
    if (finished == _NULL) return;

    gTaskQueue[finished].terminated = 1;
    RemoveFromReadyQueue(finished);
    FreeTCB(finished);
    gRunningTask = _NULL;

    OS_Schedule();
}
