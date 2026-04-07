#include <stdio.h>
#include "os.h"
#include "os_types.h"
#include "task.h"

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

//простите люди добрые но если не зациклить ос то 
//после StartOS(0) программа завершается и задачи(Task1, Task2...) не выполняются!!!!
//я пока поменяла тут да простит меня бог и кирилл
void StartOS(TTask task) {
    printf("[OS] StartOS(task=%d) called\n", task);
    gSystemRunning = 1;

    // Активируем начальную задачу если нужно
    if (task >= 0) {
        ActivateTask(task);
    }

    // Бесконечный цикл планировщика
    while (gSystemRunning) {
        OS_Schedule();
    }
}

void ShutdownOS(void) {
    printf("[OS] ShutdownOS() called\n");
    // TODO: Немедленное завершение работы системы
    // TODO: Остановить все задачи
    // TODO: Освободить ресурсы
}