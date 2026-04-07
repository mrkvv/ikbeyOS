#ifndef TASK_H
#define TASK_H

#include "os_types.h"

/* ============ —ќ—“ќяЌ»я «јƒј„ (дл€ других модулей) ============ */
#define TASK_SUSPENDED      0
#define TASK_READY          1
#define TASK_RUNNING        2
#define TASK_WAITING        3
#define TASK_WAIT_EVENT     4
#define TASK_BLOCKED        5

/* ============ —“–” “”–ј «јƒј„» (доступна дл€ других модулей) ============ */
typedef struct {
    int id;
    int priority;           // базовый приоритет (RMA = 1/период)
    int current_priority;   // текущий приоритет (дл€ PIP)
    int original_priority;  // оригинальный приоритет дл€ PIP
    int state;
    int link;               // дл€ очереди
    void (*entry)(void*);   // точка входа задачи
    void* param;            // параметр задачи
    int terminated;
    TEventMask event_mask;   // маска установленных событий
    TEventMask wait_mask;    // маска ожидаемых событий
    int wait_resource;       // какой ресурс ждЄт задача (-1 если не ждЄт)
    int resource_owner;      // какой ресурс захвачен (-1 если нет)
} TTaskStruct;

/* ============ √ЋќЅјЋ№Ќџ≈ ѕ≈–≈ћ≈ЌЌџ≈ (extern дл€ других модулей) ============ */
extern TTaskStruct gTaskQueue[];
extern int gRunningTask;
extern int gSystemRunning;

/* ============ ‘”Ќ ÷»» ƒЋя ƒ–”√»’ ћќƒ”Ћ≈… ============ */

// »нициализаци€ системы задач
void InitTaskSystem(void);

// ”правление очередью готовых задач
void InsertToReadyQueue(int task);
void RemoveFromReadyQueue(int task);

// ѕолучение информации о задачах
int GetCurrentTask(void);
int GetTaskState(int task);
int GetTaskPriority(int task);
void SetTaskPriority(int task, int new_priority);
void SetTaskOriginalPriority(int task, int original_priority);
int FindHighestPriorityTask(void);

// ”правление состо€ни€ми задач
void SetTaskState(int task, int new_state);
void BlockTask(int task, int resource_id);
void UnblockTask(int task);

// ѕланировщик
void OS_Schedule(void);

#endif /* TASK_H */
