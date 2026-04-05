#include <stdio.h>
#include "os.h"
#include "os_types.h"

// TODO: Здесь будут глобальные переменные:
// - массив ресурсов (максимум 16)
// - для каждого ресурса: locked flag, owner task, ceiling priority
// - оригинальные приоритеты задач для PIP

void InitRes(TResource ResNum) {
    printf("[RES] InitRes(%d) called - PIP mode\n", ResNum);
    // TODO: Инициализировать ресурс для PIP
    // TODO: Установить locked = 0
    // TODO: Установить owner = -1
}

void PIP_GetRes(TResource ResNum) {
    printf("[PIP] PIP_GetRes(%d) called\n", ResNum);
    // TODO: Если ресурс свободен - захватить
    // TODO: Если ресурс занят другой задачей - применить PIP
    // TODO: Повысить приоритет владельца ресурса до ceiling priority
    // TODO: Заблокировать текущую задачу до освобождения ресурса
}

void PIP_ReleaseRes(TResource ResNum) {
    printf("[PIP] PIP_ReleaseRes(%d) called\n", ResNum);
    // TODO: Освободить ресурс
    // TODO: Восстановить оригинальный приоритет владельца
    // TODO: Если есть ожидающие задачи - разблокировать
    // TODO: Вызвать планировщик
}

void GetResource(TResource res) {
    printf("[RES] GetResource(%d) called\n", res);
    // TODO: Захват ресурса (для совместимости с HLP)
}

void ReleaseResource(TResource res) {
    printf("[RES] ReleaseResource(%d) called\n", res);
    // TODO: Освобождение ресурса (для совместимости с HLP)
}

void InitPVS(TSemaphore S) {
    printf("[SEM] InitPVS(%d) called\n", S);
    // TODO: Инициализация семафора
}

void P(TSemaphore S) {
    printf("[SEM] P(%d) called\n", S);
    // TODO: Захват семафора
}

void V(TSemaphore S) {
    printf("[SEM] V(%d) called\n", S);
    // TODO: Освобождение семафора
}