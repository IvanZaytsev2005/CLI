#include "stdint.h"

#define length_of_command 16
#define num_of_params 10

typedef struct cmd{
    uint8_t ComandDesignator[length_of_command];
    uint8_t (*function) (int32_t *par);
} CmdType;

typedef struct query {
    uint8_t command[length_of_command];
    int32_t params[num_of_params];
}QueryType;

typedef struct cli {
    CmdType* Cmds;

    uint8_t (*transmit) (uint8_t* data_out, uint8_t Len);

    uint16_t flag;

    uint8_t Ncmd;

    uint8_t* InputCnt;
    
    uint8_t* OutpuCnt;

    uint8_t LengthQuery;

    QueryType* query;//maybe this will counter to last query and i will store several last querys

} CliType;

enum state_cli {
    echo = 1 << 0,
    num_contr = 1 << 1,//for moving smth by numbers
    busy = 1 << 2,
    echo_code = 1 << 3
}state_cli;

