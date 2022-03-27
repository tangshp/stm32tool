#ifndef _STM32TOOL_H_
#define _STM32TOOL_H_

#include "serial.h"
#include "message.h"
#include <stdbool.h>
#include <windows.h>
#include <process.h>

struct SerialConfig
{
    const char *port;
    uint32_t baud;
    uint8_t bits;
    Parity parity;
    uint8_t stopbit;
    // config("COM4", 115200, 8, NO, 1);
    SerialConfig(const char *_port, uint32_t _baud, uint8_t _bits, Parity _parity, uint8_t _stopbit) : port(_port), baud(_baud), bits(_bits), parity(_parity), stopbit(_stopbit)
    {
    }
};

#define CommandPeriod 60 // ms
#define ReadPeriod 20    // ms

#define HistoryMaxLen 20

class Stm32tool
{
public:
    typedef enum LinenoiseRunStatue
    {
        INVALID,
        CTRL_C,
        RUN_OK,
        RUN_ERR,
        INPUT_EMPTY
    } LineRunStatus;
    Message_t *pMessage = NULL;
    MessageSerial_t messageSerial;
    int length = 0; // 数据长度

    const char *path = ".stm32toolrc";

    // 线程
    HANDLE hThread;
    DWORD threadId;
    DWORD exitCode = 0;

public:
    Stm32tool(const SerialConfig &serialConfig);

    bool run();

    ~Stm32tool();

    static DWORD WINAPI command(LPVOID lpParam);

    bool initSerial(const SerialConfig &config);

    int push2Terminal(const char *str);

    bool initCommand();

    bool commandIsRun();

    bool linenoise_init();

    LineRunStatus linenoise_run(LineRunStatus lastResult);

    int serialRead();
};

#endif