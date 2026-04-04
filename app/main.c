#include "rtos_api.h"

/* Объявить задачи */
DeclareTask(Task1)
DeclareTask(Task2)

/* Объявить ресурсы */
DeclareResource(ResA, 5)   /* потолок приоритета = 5 */

/* Тело задачи 1 */
TASK(Task1, 3)
{
    while (1) {
        GetResource(ResA);
        /* Критическая секция */
        ReleaseResource(ResA);
        TerminateTask();
    }
}

/* Тело задачи 2 */
TASK(Task2, 1)
{
    while (1) {
        TerminateTask();
    }
}

/* Главная */
void main(void)
{
    StartOS(Task1);
}