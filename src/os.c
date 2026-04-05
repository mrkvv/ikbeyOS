#include <stdio.h>
#include "os.h"
#include "os_types.h"

void EnterISR(void) {
    printf("[ISR] EnterISR() called\n");
    // TODO: Сохранить контекст прерывания
    // TODO: Переключиться на системный стек
}

void LeaveISR(void) {
    printf("[ISR] LeaveISR() called\n");
    // TODO: Восстановить контекст прерывания
    // TODO: Если нужно переключение задач - вызвать планировщик
}

void StartOS(TTask task) {
    printf("[OS] StartOS(task=%d) called\n", task);
    // TODO: Инициализация всех системных структур
    // TODO: Установить начальную задачу
    // TODO: Запустить планировщик
    // TODO: Возврат только после ShutdownOS
}

void ShutdownOS(void) {
    printf("[OS] ShutdownOS() called\n");
    // TODO: Немедленное завершение работы системы
    // TODO: Остановить все задачи
    // TODO: Освободить ресурсы
}