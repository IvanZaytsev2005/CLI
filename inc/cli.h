#include "types.h"
#include "stdint.h"

uint8_t ProcessingInputData(CliType* cli, uint8_t data);
void CMDProcessing(CliType* cli);
int8_t FindFunc(CliType* cli);
uint8_t my_strcmp(uint8_t* a, uint8_t* b);
void print(CliType*cli, uint8_t mode, const uint8_t* str, ...);
//Defines
