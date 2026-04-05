#ifndef __RTOS_API_H
#define __RTOS_API_H

#include "hardware.h"
#include "defs.h"

/******************************************************************************/
/* Макросы для пользователя (как в ChaOS)                                     */
/******************************************************************************/

/* Объявить задачу */
#define DeclareTask(taskID)                    \
    extern const _TaskStruct taskID[1];

/* Объявить ресурс */
#define DeclareResource(resID, prior)          \
    enum { resID = prior };

/* Активировать задачу */
#define ActivateTask(taskID)                   \
    _Activate(taskID->entry, taskID->priority)

/* Запустить ОС */
#define StartOS(taskID)                        \
    _Start(taskID->entry, taskID->priority)

/* Захватить ресурс */
#define GetResource(resID)                     \
    _GetResource(resID)

/* Освободить ресурс */
#define ReleaseResource(resID)                 \
    _ReleaseResource(resID)

/* Установить событие */
#define SetEvent(taskID, mask)                 \
    _SetEvent(taskID, mask)

/* Ожидать событие */
#define WaitEvent(mask)                        \
    _WaitEvent(mask)

/* Получить событие */
#define GetEvent(taskID, mask_ptr)             \
    _GetEvent(taskID, mask_ptr)

/* Очистить событие */
#define ClearEvent(mask)                       \
    _ClearEvent(mask)

/* Объявить задачу с телом */
#define TASK(taskID, priority)                 \
    void taskID##_body(void);                  \
    const _TaskStruct taskID[1] =              \
        { {taskID##_body, priority} };         \
    void taskID##_body(void)

/******************************************************************************/
/* Типы данных API                                                            */
/******************************************************************************/

typedef void (*_pfn)(void);

typedef struct {
    _pfn entry;
    char priority;
} _TaskStruct, *_PTaskStruct;

typedef _PTaskStruct TTask;
typedef char TResource;
typedef unsigned int TEventMask;

/******************************************************************************/
/* Функции API (вызываются макросами)                                         */
/******************************************************************************/

void _Start(_pfn task, char priority);
void _Activate(_pfn task, char priority);
void _Terminate(void);
void _GetResource(TResource resource);
void _ReleaseResource(TResource resource);
void _SetEvent(TTask task, TEventMask mask);
void _WaitEvent(TEventMask mask);
void _GetEvent(TTask task, TEventMask *mask);
void _ClearEvent(TEventMask mask);

/* Сервисы */
void TerminateTask(void);
void ShutdownOS(void);

#endif