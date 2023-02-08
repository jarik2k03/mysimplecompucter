#include "computer.h"

#include <stdio.h>
#include <stdlib.h>

#define RAM 100

int* sc_init()
{
    int* memory = calloc(RAM, sizeof(int));
    if (memory != NULL) {
        printf("СуперКомпьютер запущен. Добро пожаловать!\n");
        return memory;
    } else {
        printf("Недостаточно ОЗУ для запуска.");
        return NULL;
    }
}