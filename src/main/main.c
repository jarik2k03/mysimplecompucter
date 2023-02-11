#include <computer/computer.h>
#include <stdio.h>

int main(void)
{
    int command = 41, operand = 127;
    int value = 0;
    sc_init();
    
    //sc_memoryRand();
    sc_memoryLoad("load.bin");
    // sc_memorySave("save.bin");

    sc_outputMemory();
    sc_commandEncode(command, operand, &value);
    printf("Закодировано: %d\n", value);
    sc_commandDecode(value, &command, &operand);
    printf("Декодирована команда: %d, операнд: %d\n", command, operand);
}