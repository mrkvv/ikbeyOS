#ifndef OS_H
#define OS_H

#include "os_types.h"

// Макросы
#define TASK(TaskID, TaskParam) void TaskID(void* TaskParam)
#define ISR(IntID) void IntID(void)
#define DeclareTask(TaskID)
#define DeclareResource(ResourceID, CP)
#define DeclareEvent(EventID)

// Управление задачами
void ActivateTask(TTask task);
void ISRActivateTask(TTask task);
void TerminateTask(void);

// Управление прерываниями
void EnterISR(void);
void LeaveISR(void);

// Управление ресурсами
void GetResource(TResource res);
void ReleaseResource(TResource res);
void InitPVS(TSemaphore S);
void P(TSemaphore S);
void V(TSemaphore S);
void InitRes(TResource ResNum);
void PIP_GetRes(TResource ResNum);
void PIP_ReleaseRes(TResource ResNum);

// Управление событиями
void SetEvent(TTask task, TEventMask mask);
void GetEvent(TTask task, TEventMask* event);
void WaitEvent(TEventMask mask);
void ClearEvent(TEventMask mask);
void SetSysEvent(TEventMask mask);
void GetSysEvent(TEventMask* event);
void WaitSysEvent(TEventMask mask);

// Управление ОС
void StartOS(TTask task);
void ShutdownOS(void);

#endif