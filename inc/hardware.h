#ifndef __HARDWARE_H
#define __HARDWARE_H

/* Запретить прерывания (если используете) */
#define DI()            asm cli

/* Разрешить прерывания */
#define EI()            asm sti

/* Установить указатель стека */
#define SetSP(expr)     _SP = (expr)

/* Получить указатель стека */
#define GetSP()         _SP

/* Объявление прерывания (не используется, но для совместимости) */
#define ISR(IntID)      void interrupt IntID (void)

/* Тип обработчика (заглушка) */
typedef void (*THandler)(void);

#endif