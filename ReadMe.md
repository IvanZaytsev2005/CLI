# Модуль интерфейса командной строки для микроконтроллера. 

Установка библиотеки:
1. распаковать библиотеку как субмодуль
    ```bash
    git init
    git submodule add git@github.com:IvanZaytsev2005/CLI.git
    ```
2. добавить в основной код #include "cli.h"
3. Определить переменные для работы с библиотекой:
    ```c
    CliType cli;
    QueryType query;
    uint8_t DataInput[256];
    uint8_t DataOutput[256];
    uint8_t DataInUart;//переменная для приема байта по UART
    ```
4. Определить функцию отправки данных. Например:
    ```c
    uint8_t Transmit(uint8_t* Data, uint8_t len) {
    HAL_UART_Transmit(&huart1, Data, len, len*5);
    }
    ```  
5. Определить массив типа CmdType, содеражащий дескрипторы команд (длиной не более length_of_command 16 символов) и заголовки соответствующих функций. Пример:
    ```c
    CmdType cmd[] = {{"led", Led}
                        ,{"hello", HelloWorld}
    };
    ```
    Функции, соответствующие командам должны иметь вид:  
    ```c
    uint8_t NameFunction(int32_t* params);
    ```  
    Где params - указатель на массив получаемых параметров.  
    Пример функции:
    ```c
    uint8_t HelloWorld(int32_t* Pars) {
    print(&cli, "Hello World!!! My params is %d, %f", *Pars, (double)*(Pars+1)/1000.0);
    }
    ```
6. Инициализировать поля структуры CliType, в соответствии с определенными ранее переменными:  
    ```c
    cli.transmit = Transmit;
    cli.InputCnt = DataInput;
    cli.OutputCnt = DataOutput;
    cli.Cmds = cmd;
    cli.flag |= echo;
    cli.query = &query;
    cli.LengthQuery = 256;
    ```
7. по приему байта вызывать ProcessingInputData(uint8_t data), где data - полученный байт. Например
```c
HAL_UART_Receive_IT(&huart1, &DataUartIn, 1);

void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart) {
  //if(huart->Instance == USART1) {
    ProcessingInputData(&cli, DataUartIn);
    HAL_UART_Receive_IT(&huart1, &DataUartIn, 1);
  //}
}
```
8. В основном цикле проверять состояние флага busy переменной cli.flag. Данный флаг устанавливается при приеме строки. После появления данного флага необходимо вызвать функцию CMDProcessing(&cli), которая выполнит функцию, соответствующей принятой команде.
    ```c
      while (1)
      {
        if((cli.flag & busy) != 0) {
          CMDProcessing(&cli);
          cli.flag &= ~busy;
        }
      }
    ```

## Описание структуры CliType:
    cli.transmite - наименование функции отправки N байт.  
    cli.InputCnt - указатель на массив под входные данные (рекомендуется длина 256 байт)  
    cli.OutputCnt - указатель на массив под выходные данные  
    cli.query - указатель на переменную, содержащую распасенное сообщение  
    cli.LengthQuery - размер входного буфера (256)  