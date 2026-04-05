#include <stdio.h>
#include "os.h"
#include "os_types.h"

// TODO: Здесь будут глобальные переменные:
// - очередь готовых задач (максимум 32 задачи)
// - массив приоритетов задач
// - массив состояний задач (suspended, ready, running)
// - текущая выполняемая задача
// - указатели на контексты задач

void ActivateTask(TTask task) {
    printf("[TASK] ActivateTask(%d) called\n", task);
    // TODO: Перевести задачу из suspended в ready
    // TODO: Добавить задачу в очередь готовых задач
    // TODO: Вызвать планировщик
}

void ISRActivateTask(TTask task) {
    printf("[TASK] ISRActivateTask(%d) called from ISR\n", task);
    // TODO: Перевести задачу из suspended в ready
    // TODO: Добавить задачу в очередь готовых задач
    // TODO: НЕ вызывать планировщик (только пометить)
}

void TerminateTask(void) {
    printf("[TASK] TerminateTask() called\n");
    // TODO: Завершить текущую задачу
    // TODO: Перевести её в состояние suspended
    // TODO: Освободить все ресурсы задачи
    // TODO: Вызвать планировщик
}

// Внутренняя функция планировщика (не из спецификации)
void OS_Schedule(void) {
    printf("[SCHED] Schedule() called - RMA, preemptive\n");
    // TODO: Реализовать RMA планировщик
    // TODO: Выбрать задачу с наивысшим приоритетом из очереди ready
    // TODO: Если выбранная задача не равна текущей - вызвать Dispatch
}

// Внутренняя функция переключения контекста (не из спецификации)
void OS_Dispatch(TTask from, TTask to) {
    printf("[DISPATCH] Switching from task %d to task %d\n", from, to);
    // TODO: Сохранить контекст текущей задачи
    // TODO: Восстановить контекст новой задачи
    // TODO: Выполнить переключение
}