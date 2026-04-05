#include <stdio.h>
#include "os.h"
#include "os_types.h"

// TODO: Здесь будут глобальные переменные:
// - маска событий для каждой задачи (максимум 32 задачи, 16 событий)
// - маска системных событий
// - очередь задач, ожидающих события

void SetEvent(TTask task, TEventMask mask) {
    printf("[EVENT] SetEvent(task=%d, mask=0x%X) called\n", task, mask);
    // TODO: Установить события для задачи task согласно маске
    // TODO: Если задача ждала хотя бы одно из этих событий - перевести в ready
    // TODO: Вызвать планировщик
}

void GetEvent(TTask task, TEventMask* event) {
    printf("[EVENT] GetEvent(task=%d) called\n", task);
    // TODO: Вернуть текущую маску установленных событий задачи
    if (event != NULL) {
        *event = 0;
    }
}

void WaitEvent(TEventMask mask) {
    printf("[EVENT] WaitEvent(mask=0x%X) called\n", mask);
    // TODO: Если ни одно событие из mask не установлено - перевести задачу в waiting
    // TODO: При выходе сбросить события, которые вызвали выход
}

void ClearEvent(TEventMask mask) {
    printf("[EVENT] ClearEvent(mask=0x%X) called\n", mask);
    // TODO: Очистить события согласно маске
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