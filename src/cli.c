#include "cli.h"


const uint8_t NewLine[8] = {"\r\n--> "};

uint8_t ProcessingInputData(CliType* cli, uint8_t data)
{
    static uint8_t count;
    uint8_t Out[256];
    int32_t par[3];
    uint8_t lenght;
    switch (data)
    {
        case '\r':
            count = 0;
            cli->flag |= busy;
            return (1);
            break;
        default:
            cli->InputCnt[count] = data;
            if((cli->flag & echo_code) != 0) {
                lenght = sprintf(Out, "%d \n\r", data);
                cli->transmit(Out, lenght);
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
    for(uint8_t i=0; i<10; i++)
    {
        cli->query->params[i] = 0;
    }
    uint8_t* InputLineCounter = cli->InputCnt;
    uint8_t* ComandCounter = cli->query->command;
    uint8_t data = *InputLineCounter;
    while ((data == ' ')) {
        InputLineCounter++;
        data = *InputLineCounter;
    }
    while ((data!=' ')&(data != '\r')) {
        *ComandCounter = data;
        ComandCounter++;
        InputLineCounter++;
        data = *InputLineCounter;
    }
    while (data == ' ')
    {
        InputLineCounter++;
        data = *InputLineCounter;
    }
    uint8_t count_params = 0;
    while(*InputLineCounter != '\r') {
        cli->query->params[count_params] = atoi(InputLineCounter);
        while (*InputLineCounter == ' ') {
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
    cli->Cmds[FindFunc(cli)].function(cli->query->params);
    for(uint8_t i = 0; i < cli->LengthQuery; i++)
    {
        cli->InputCnt[i] = 0;
    }
}

uint8_t FindFunc(CliType* cli)
{
    for(uint8_t i = 1; i < cli->Ncmd; i++)
    {
        if(strcmp(cli->Cmds[i].ComandDesignator, cli->query->command))
        {
            return(i);
        }
    }
    return(0);
}

uint8_t stcmp(uint8_t* a,uint8_t* b) {
    uint8_t i=0;
    while(i<length_of_command) {
        if(a[i] != b[i]) {
            return(0);
        }
        i++;
    }
    return(1);
}

