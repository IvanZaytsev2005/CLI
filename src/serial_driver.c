#include "/home/dgtop/stm32/lib/CLI/inc/serial_driver.h"
#include "/home/dgtop/stm32/lib/CLI/inc/comand_processing.h"

//include librari for serial work
#include "stm32f4xx_hal_uart.h"
extern UART_HandleTypeDef huart6;
//extern uint8_t flag;
//extern uint8_t flag_cmd;
//massive for input line
uint8_t InputLine[256];
char Comand[16];
int32_t Params[10];
const char Ok[] = "Zaebiz";
const char Error[] = "You are have an errors!!!!!!!!"; 
//here add the function for write data to computer
void SerialWrite(uint8_t *data, uint16_t size)
{
    HAL_UART_Transmit(&huart6, data, size, 100);
}

const char NewLine[8] = {"\r\n--> "};

uint8_t ProcessingInputData(uint8_t data)
{
    static uint8_t count;
    uint8_t Out[256];
    int32_t par[3];
    uint8_t lenght;
    switch (data)
    {
        case '\r':
            return (1);
            break;
        default:
            if((flag_cmd&8)!=0)
            {
                lenght = sprintf(Out, "%d \n\r", data);
                SerialWrite(Out, lenght);
            }
            InputLine[count] = data;
            if((flag_cmd&16)==0)
            {
                SerialWrite(&data, 1);
            }
            count++;
            break;
    }
    return(0);
}

void CMDProcessing()
{
    for(uint8_t i=0; i<16; i++)
    {
        Comand[i] = 0;
    }
    for(uint8_t i=0; i<10; i++)
    {
        Params[i] = 0;
    }
    uint8_t* InputLineCounter = InputLine;
    uint8_t* ComandCounter = Comand;
    uint8_t data = *InputLineCounter;
    uint8_t ComandSize=0;
    while ((data == ' '))
    {
        InputLineCounter++;
        data = *InputLineCounter;
    }
    while ((data!=' ')&(data != '\r'))
    {
        *ComandCounter = data;
        ComandSize++;
        InputLineCounter++;
        data = *InputLineCounter;
        ComandCounter++;
    }
    while (data == ' ')
    {
        InputLineCounter++;
        data = *InputLineCounter;
    }
    uint8_t count_params = 0;
    int8_t sign = 1;
    Params[0] = 0;
    while (data!='\r')
    {
        if(data == ' ')
        {
            Params[count_params] = Params[count_params]*sign;
            sign = 1;
            count_params++;
            Params[count_params] = 0;
        }
        else
        {
            if(data == '-')
            {
                sign = -1;
            }
            else
            {
                Params[count_params] = Params[count_params]*10;
                Params[count_params] += (data & (~0x30));
            }
        }
        InputLineCounter++;
        data = *InputLineCounter;
    }
    Params[count_params] = Params[count_params]*sign;
    FindFunc();
    for(uint16_t i=0; i<256; i++)
    {
        InputLine[i] = 0;
    }
}