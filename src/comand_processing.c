#include "comand_processing.h"
#include "serial_driver.h"
#include "types.h"

extern char Comand[16];
extern int32_t Params[10];

void FindFunc(CmdType cmd, uint8_t Ncmd)
{
    for(uint8_t i=1; i<Ncmd; i++)
    {
        if(strcmp(cmd[i].ComandDesignator, Comand))
        {
            return(i);
        }
    }
    return(0);
}

uint8_t stcmp(char a[16],char b[16])
{
    // SerialWrite(a, 16);
    // SerialWrite(b,16);
    uint8_t i=0;
    while(i<16)
    {
        //SerialWrite(&i, 1);
        if(a[i]!=b[i])
        {
            return(0);
        }
        i++;
    }
    return(1);
}