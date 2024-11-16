
typedef struct cmd{
    char ComandDesignator[16];
    uint8_t *(function) (int32_t *par);
} CmdType;

