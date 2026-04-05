#ifndef __SYS_H
#define __SYS_H

#include "defs.h"
#include "hardware.h"

/******************************************************************************/
/* Состояния задачи                                                           */
/******************************************************************************/
#define TASK_SUSPENDED  0
#define TASK_READY      1
#define TASK_RUNNING    2
#define TASK_WAITING    3

/******************************************************************************/
/* Системная структура задачи (внутренняя)                                    */
/******************************************************************************/
typedef struct {
    unsigned context;        /* Сохранённый контекст (указатель стека) */
    char priority;           /* Базовый приоритет (RMA) */
    char current_priority;   /* Текущий приоритет (для PIP) */
    char state;              /* Состояние задачи */
    char link;               /* Связь в очереди (индекс) */
    TEventMask wait_mask;    /* Какие события ожидаются */
} TTaskStruct;

/******************************************************************************/
/* Системная структура ресурса (для PIP)                                      */
/******************************************************************************/
typedef struct {
    char owner;              /* Какая задача владеет */
    char ceiling_priority;   /* Потолок приоритета */
    char locked;             /* Заблокирован? */
} TResourceStruct;

/******************************************************************************/
/* Глобальные переменные ядра                                                 */
/******************************************************************************/
extern char gRunningTask;           /* Текущая задача */
extern char gTaskQueueHead;         /* Голова очереди готовых */
extern TTaskStruct gTaskQueue[TASK_MAX];
extern TResourceStruct gResources[RESOURCE_MAX];
extern char gFreeTask;              /* Список свободных TCB */
extern unsigned gReturnSP;          /* Точка возврата при ShutdownOS */

/******************************************************************************/
/* Функции ядра                                                               */
/******************************************************************************/
void _Schedule(char new_priority);
void _Dispatch(void);
void _SwitchContext(unsigned* old_sp, unsigned new_sp);

#endif