#include <stdio.h>
#include "os.h"
#include "os_types.h"
#include "task.h"

// TODO: Здесь будут глобальные переменные:
// - маска событий для каждой задачи (максимум 32 задачи, 16 событий)
// - маска системных событий
// - очередь задач, ожидающих события

/* ============ КОНФИГУРАЦИЯ ============ */

#define MAX_EVENTS 16
#define EVENT_MASK_ALL 0xFFFF

/* ============ ПЕРЕМЕННЫЕ МОДУЛЯ ============ */

// Маска системных событий
static TEventMask gSystemEventMask = 0;

// Очередь задач, ожидающих системные события
static int gSysEventWaitQueue[32];
static int gSysEventWaitCount = 0;

/* ============ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ============ */

// Проверка валидности маски
int IsEventMaskValid(TEventMask mask)
{
    TEventMask valid_mask = (1 << MAX_EVENTS) - 1;
    return (mask & ~valid_mask) == 0;
}

// Проверка, есть ли хотя бы одно событие из маски
static int HasAnyEvent(TEventMask task_mask, TEventMask wait_mask)
{
    return (task_mask & wait_mask) != 0;
}

// Сброс событий, которые были в wait_mask
static void ClearMatchedEvents(int task_id, TEventMask wait_mask)
{
    gTaskQueue[task_id].event_mask &= ~wait_mask;
}

/* ============ ИНИЦИАЛИЗАЦИЯ ============ */

void InitEventSystem(void)
{
    printf("[EVENT] Event system initialized (max events: %d)\n", MAX_EVENTS);

    for (int i = 0; i < TASK_MAX; i++) 
    {
        gTaskQueue[i].event_mask = 0;
        gTaskQueue[i].wait_mask = 0;
    }

    gSystemEventMask = 0;
    gSysEventWaitCount = 0;
}

/* ============ ПУБЛИЧНОЕ API ============ */

void SetEvent(TTask task, TEventMask mask) 
{
    printf("[EVENT] SetEvent(task=%d, mask=0x%02X) called\n", task, mask);

    if (task < 0 || task >= TASK_MAX) 
    {
        printf("[EVENT] ERROR: Invalid task ID %d\n", task);
        return;
    }

    if (!IsEventMaskValid(mask)) 
    {
        printf("[EVENT] ERROR: Invalid event mask 0x%02X\n", mask);
        return;
    }

    // Устанавливаем события
    TEventMask old_mask = gTaskQueue[task].event_mask;
    gTaskQueue[task].event_mask |= mask;

    printf("[EVENT] Task %d event mask: 0x%02X -> 0x%02X\n",
        task, old_mask, gTaskQueue[task].event_mask);

    // Если задача ждала хотя бы одно из установленных событий
    TEventMask wait_mask = gTaskQueue[task].wait_mask;

    if (wait_mask != 0 && HasAnyEvent(gTaskQueue[task].event_mask, wait_mask)) {
        printf("[EVENT] Task %d was waiting for events 0x%02X, now ready\n", task, wait_mask);

        if (gTaskQueue[task].state == TASK_WAIT_EVENT) 
        {
            gTaskQueue[task].wait_mask = 0;
            gTaskQueue[task].state = TASK_READY;
            InsertToReadyQueue(task);

            // Перепланирование
            if (gSystemRunning && gRunningTask != _NULL) 
            {
                int current_prio = gTaskQueue[gRunningTask].current_priority;
                int task_prio = gTaskQueue[task].current_priority;

                if (task_prio < current_prio) 
                {
                    OS_Schedule();
                }
            }
            else if (gSystemRunning) 
            {
                OS_Schedule();
            }
        }
    }
}

void GetEvent(TTask task, TEventMask* event) 
{
    printf("[EVENT] GetEvent(task=%d) called\n", task);

    if (task < 0 || task >= TASK_MAX) 
    {
        printf("[EVENT] ERROR: Invalid task ID %d\n", task);
        if (event) *event = 0;
        return;
    }

    if (event == NULL) 
    {
        printf("[EVENT] ERROR: Null pointer\n");
        return;
    }

    *event = gTaskQueue[task].event_mask;
    printf("[EVENT] Task %d event mask: 0x%02X\n", task, *event);
}

void WaitEvent(TEventMask mask) 
{
    int current_task = gRunningTask;

    printf("[EVENT] WaitEvent(task=%d, mask=0x%02X) called\n", current_task, mask);

    if (current_task == _NULL) 
    {
        printf("[EVENT] ERROR: No task running\n");
        return;
    }

    if (!IsEventMaskValid(mask)) 
    {
        printf("[EVENT] ERROR: Invalid event mask 0x%02X\n", mask);
        return;
    }

    if (mask == 0) 
    {
        printf("[EVENT] WARNING: Empty mask, returning immediately\n");
        return;
    }

    TEventMask current_events = gTaskQueue[current_task].event_mask;

    if (HasAnyEvent(current_events, mask)) 
    {
        // События уже есть - сбрасываем и выходим
        printf("[EVENT] Events 0x%02X already set, clearing\n", current_events & mask);
        ClearMatchedEvents(current_task, mask);
        gTaskQueue[current_task].wait_mask = 0;
    }
    else 
    {
        // Нет событий - блокируемся
        printf("[EVENT] No events, task %d blocked\n", current_task);

        gTaskQueue[current_task].wait_mask = mask;
        gTaskQueue[current_task].state = TASK_WAIT_EVENT;
        RemoveFromReadyQueue(current_task);

        OS_Schedule();
    }
}

void ClearEvent(TEventMask mask) 
{
    int current_task = gRunningTask;

    printf("[EVENT] ClearEvent(task=%d, mask=0x%02X) called\n", current_task, mask);

    if (current_task == _NULL) 
    {
        printf("[EVENT] ERROR: No task running\n");
        return;
    }

    if (!IsEventMaskValid(mask)) 
    {
        printf("[EVENT] ERROR: Invalid event mask 0x%02X\n", mask);
        return;
    }

    TEventMask old_mask = gTaskQueue[current_task].event_mask;
    gTaskQueue[current_task].event_mask &= ~mask;

    printf("[EVENT] Task %d event mask: 0x%02X -> 0x%02X\n",
        current_task, old_mask, gTaskQueue[current_task].event_mask);
}

void SetSysEvent(TEventMask mask) {
    printf("[EVENT] SetSysEvent(mask=0x%X) called\n", mask);
    // TODO: Установить системные события
    // TODO: Разбудить все задачи, ожидающие эти события
}

void GetSysEvent(TEventMask* event) {
    printf("[EVENT] GetSysEvent() called\n");
    // TODO: Вернуть текущую маску системных событий
    if (event != NULL) {
        *event = 0;
    }
}

void WaitSysEvent(TEventMask mask) {
    printf("[EVENT] WaitSysEvent(mask=0x%X) called\n", mask);
    // TODO: Если ни одно системное событие из mask не установлено - перевести задачу в waiting
}