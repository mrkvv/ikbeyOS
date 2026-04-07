#include <stdio.h>
#include "os.h"
#include "os_types.h"
#include "task.h"

// Объявление задач
DeclareTask(Task1);
DeclareTask(Task2);
DeclareTask(Task3);

// Объявление ресурсов
DeclareResource(Res1, 3);
DeclareResource(Res2, 2);

// Объявление событий
DeclareEvent(Event1);
DeclareEvent(Event2);

// Реализация задач
TASK(Task1, param) {
    printf("\n========== TASK 1 RUNNING ==========\n");

    PIP_GetRes(0);
    WaitEvent(0x01);
    PIP_ReleaseRes(0);

    printf("========== TASK 1 FINISHED ==========\n");
    TerminateTask();
}

TASK(Task2, param) {
    printf("\n========== TASK 2 RUNNING ==========\n");

    SetEvent(0, 0x01);
    GetResource(0);
    ReleaseResource(0);

    printf("========== TASK 2 FINISHED ==========\n");
    TerminateTask();
}

TASK(Task3, param) {
    printf("\n========== TASK 3 RUNNING ==========\n");

    InitRes(0);
    PIP_GetRes(0);
    PIP_ReleaseRes(0);

    printf("========== TASK 3 FINISHED ==========\n");
    TerminateTask();
}

int main() {
    printf("========================================\n");
    printf("RTOS Project - Team Development\n");
    printf("========================================\n");
    printf("Properties:\n");
    printf("  - Scheduler: Flat, Preemptive RMA\n");
    printf("  - Resource: PIP\n");
    printf("  - Events: Task-owned\n");
    printf("  - Max tasks: 32, resources: 16, events: 16\n");
    printf("========================================\n\n");

    // для планировщика: Инициализация системы задач
    InitTaskSystem();

    // для планировщика: Регистрация задач с приоритетами
    RegisterTask(0, Task1, NULL, 1);  // Task1 - высокий приоритет
    RegisterTask(1, Task2, NULL, 2);  // Task2 - средний приоритет
    RegisterTask(2, Task3, NULL, 3);  // Task3 - низкий приоритет

    // Инициализация ресурсов
    InitRes(0);
    InitRes(1);

    // Активация задач
    ActivateTask(0);
    ActivateTask(1);
    ActivateTask(2);

    // Запуск ОС
    StartOS(0);

    printf("\n========================================\n");
    printf("OS Simulation Complete\n");
    printf("========================================\n");

    return 0;
}