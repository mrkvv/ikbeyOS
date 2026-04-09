#ifndef EVENT_H
#define EVENT_H

#include "os_types.h"

/* ============ МАКРОСЫ ============ */

#define DeclareEvent(EventID) extern const int EventID

/* ============ ПУБЛИЧНОЕ API ============ */

// Управление событиями задачи (из спецификации)
void SetEvent(TTask task, TEventMask mask);
void GetEvent(TTask task, TEventMask* event);
void WaitEvent(TEventMask mask);
void ClearEvent(TEventMask mask);

// Инициализация системы событий
void InitEventSystem(void);

// Вспомогательные функции
int IsEventMaskValid(TEventMask mask);
const char* GetEventString(void);

#endif
