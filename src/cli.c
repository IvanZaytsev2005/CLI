#include "cli.h"
#include "stdint.h"
#include <stdarg.h>


const uint8_t NewLine[4] = {"--> "};

uint8_t ProcessingInputData(CliType* cli, uint8_t data)
{
    static uint8_t count;
    int32_t par[3];
    uint8_t lenght;
    switch (data)
    {
        case '\r':
            cli->InputCnt[count] = data;
            count = 0;
            cli->flag |= busy;
            lenght = sprintf(cli->OutputCnt, "\n\r");
            cli->transmit(cli->OutputCnt, lenght);
            return (1);
            break;
        case 8:
            if(count > 0) {
                count--;
            }
            cli->transmit(&data, 1);
            break;
        default:
            cli->InputCnt[count] = data;
            if((cli->flag & echo_code) != 0) {
                lenght = sprintf(cli->OutputCnt, "%d \n\r", data);
                cli->transmit(cli->OutputCnt, lenght);
            } else {
                if((cli->flag & echo) != 0) {
                    cli->transmit(&data, 1);
                }
            }
            count++;
            break;
    }
    return(0);
}

void CMDProcessing(CliType* cli)
{
    for(uint8_t i = 0; i < length_of_command; i++)
    {
        cli->query->command[i] = 0;
    }
    for(uint8_t i=0; i < num_of_params; i++)
    {
        cli->query->params[i] = 0;
    }
    uint8_t* InputLineCounter = cli->InputCnt;
    uint8_t* ComandCounter = cli->query->command;
    while ((*InputLineCounter == ' ')) {
        InputLineCounter++;
    }
    while ((*InputLineCounter!=' ') & (*InputLineCounter != '\r')) {
        *ComandCounter = *InputLineCounter;
        ComandCounter++;
        InputLineCounter++;
    }
    while ((*InputLineCounter == ' ')) {
        InputLineCounter++;
    }
    uint8_t count_params = 0;
    while(*InputLineCounter != '\r') {
        cli->query->params[count_params] = atoi(InputLineCounter);
        while ((*InputLineCounter!=' ') & (*InputLineCounter != '\r')) {
            InputLineCounter++;
        }
        while ((*InputLineCounter == ' ')) {
            InputLineCounter++;
        }
        count_params++;
    }
    
    // int8_t sign = 1;
    // Params[0] = 0;
    // while (data!='\r')
    // {
    //     if(data == ' ')
    //     {
    //         Params[count_params] = Params[count_params]*sign;
    //         sign = 1;
    //         count_params++;
    //         Params[count_params] = 0;
    //     }
    //     else
    //     {
    //         if(data == '-')
    //         {
    //             sign = -1;
    //         }
    //         else
    //         {
    //             Params[count_params] = Params[count_params]*10;
    //             Params[count_params] += (data & (~0x30));
    //         }
    //     }
    //     InputLineCounter++;
    //     data = *InputLineCounter;
    // }
    // Params[count_params] = Params[count_params]*sign;
    uint8_t num;
    num = FindFunc(cli);
    // uint8_t N;
    // N = sprintf(cli->OutputCnt," %d \n", num);
    // cli->transmit(cli->OutputCnt, N);
    cli->Cmds[num].function(cli->query->params);
    cli->transmit(NewLine, 4);
    for(uint16_t i = 0; i < cli->LengthQuery; i++)
    {
        cli->InputCnt[i] = 0;
    }
}

uint8_t FindFunc(CliType* cli)
{
    uint8_t* DesigCount;
    uint8_t* ComCount;
    ComCount = cli->query->command;
    for(uint8_t i = 1; i < cli->Ncmd; i++)
    {
        DesigCount = cli->Cmds[i].ComandDesignator;
        // cli->transmit(DesigCount, 16);
        // cli->transmit(ComCount, 16);   
        if(strcmp(DesigCount, ComCount) == 1)
        {
            return(i);
        }
    }
    return(0);
}

uint8_t strcmp(uint8_t* a, uint8_t* b) {
    uint8_t i=0;
    while(i<16) {
        if(a[i] != b[i]) {
            return(0);
        }
        i++;
    }
    return(1);
}

void print(CliType*cli, const uint8_t* str, ...) {
    uint16_t N = 0;
    int d; 
    double f;
    va_list factor;
    va_start(factor, str);
    for(char *c = str; *c; c++)
    {
        if(*c!='%')
        {
            N += sprintf(cli->OutputCnt + N, "%c", *c);
            continue;
        }
        switch(*++c)    // если символ - %, то переходим к следующему символу
        {
            case 'd': 
                d = va_arg(factor, int);
                N += sprintf(cli->OutputCnt + N,"%d", d);
                break;
            case 'f': 
                f = va_arg(factor, double);
                N += sprintf(cli->OutputCnt + N, "%f", f);
                break;
            default:
                N += sprintf(cli->OutputCnt + N, "%c", *c);
        }
    }
    va_end(factor);
    N += sprintf(cli->OutputCnt + N, "\n\r");
    cli->transmit(cli->OutputCnt, N);
}