#include "stm32tool.h"
#include "linenoise.hpp"
#include "commandCallback.h"
#include "linenoise.hpp"
#include "color.h"
#include <stdio.h>

using namespace std;

uint8_t recvCnt = 1;
#define recvCntMAX 3

Serial serial;
char buffer[256] = {0}; // 串口缓存
extern command_id_t command_ids[];
extern uint32_t num_command_ids;

Stm32tool::Stm32tool(const SerialConfig &serialConfig)
{
    if (false == this->initSerial(serialConfig))
    {
        printf("Open %s failure\n", serialConfig.port);
        return;
    }

    this->linenoise_init();

    if (false == this->initCommand())
    {
        printf("failure to init command thread.\n");
        return;
    }
}

Stm32tool::~Stm32tool()
{
    serial.Close();
    CloseHandle(hThread);
}

bool Stm32tool::initSerial(const SerialConfig &config)
{
    char res = serial.Open(config.port, config.baud, config.bits, config.parity, config.stopbit);
    if (1 != res)
    { // 串口打开失败
        return false;
    }

    messageSerial.beginFlag[0] = (uint8_t)(BeginFlag >> 8);   // AA
    messageSerial.beginFlag[1] = (uint8_t)(BeginFlag & 0XFF); // BB
    messageSerial.endFlag[0] = (uint8_t)(EndFlag >> 8);       // CC
    messageSerial.endFlag[1] = (uint8_t)(EndFlag & 0XFF);     // DD
    pMessage = &(messageSerial.message);
    memset(pMessage, 0, sizeof(Message_t));

    initMsgManager();
    return true;
}

int Stm32tool::serialRead()
{

    this->length = serial.Read(buffer);
    // this->length = this->serial.Read(buffer);
    if (this->length)
    {
        printf("%s", buffer);
        // this->push2Terminal((const char *)buffer);
        // fflush(stdout)-;
    }
    memset(buffer, 0u, 256);
    return length;
}

bool Stm32tool::linenoise_init()
{

    // Enable the multi-line mode
    linenoise::SetMultiLine(true);

    // Set max length of the history
    linenoise::SetHistoryMaxLen(HistoryMaxLen);

    // Setup completion words every time when a user types
    linenoise::SetCompletionCallback([](const char *editBuffer, std::vector<std::string> &completions)
                                     {
                            
                                             if (editBuffer[0] == 'l')
                                             {
                                                 completions.push_back("led0");
                                                 completions.push_back("led1");
                                             }else if (editBuffer[0]=='b')
                                             {
                                                 completions.push_back("beep");
                                             } });

    // Load history
    linenoise::LoadHistory(path);
    return true;
}

Stm32tool::LineRunStatus Stm32tool::linenoise_run(LineRunStatus lastResult)
{
    std::string line;
    LineRunStatus runStatus = LineRunStatus::RUN_OK;
#ifdef _WIN32
    auto quit = false;
    if (lastResult == LineRunStatus::RUN_OK)
    {
        quit = linenoise::Readline(BOLD GREEN ">>> " NONE, line);
    }
    else
    {
        quit = linenoise::Readline(BOLD RED ">>> " NONE, line);
    }

#else
    auto quit = linenoise::Readline("\033[32mこんにちは\x1b[0m> ", line);
#endif
    // cout << "echo: '" << line << "'" << endl;

    if (quit)
    {
        cout << "Ctrl + C" << endl;
        return LineRunStatus::CTRL_C;
    }
    if (0 == line.length())
    {
        return INPUT_EMPTY;
    }
    if (false == packFuncFromLine(line, this->pMessage))
    {
        runStatus = LineRunStatus::RUN_ERR;
    }

    serial.Write((char *)&(this->messageSerial), sizeof(MessageSerial_t));
    // Add line to history
    linenoise::AddHistory(line.c_str());

    // Save history
    linenoise::SaveHistory(path);
    return runStatus;
}

DWORD WINAPI Stm32tool::command(void *param)
{
    Stm32tool *This = (Stm32tool *)param;
    Stm32tool::LineRunStatus linenoiseRunRes = LineRunStatus::RUN_OK;
    while (true)
    {
        if (recvCnt++ < recvCntMAX)
        { // 等待充分接收 等待recvCnt*200让主线程充分接收stm32串口发送的消息
        }
        else
        {
            recvCnt = 0;
            linenoiseRunRes = This->linenoise_run(linenoiseRunRes);
            if (LineRunStatus::CTRL_C == linenoiseRunRes)
            { // input Ctrl + C
                return 0;
            };
        }
        Sleep(CommandPeriod);
    }

    return 0;
}

bool Stm32tool::initCommand()
{
    hThread = CreateThread(NULL, 0, Stm32tool::command, (void *)this, 0, &threadId);
    return true;
}

bool Stm32tool::commandIsRun()
{
    GetExitCodeThread(hThread, &exitCode);

    if (STILL_ACTIVE != exitCode)
    { // 判断线程是否还在运行
        printf("exit stm32tool\n");
        return false;
    }
    return true;
}

bool Stm32tool::run()
{
    while (true)
    {
        this->serialRead();
        if (false == this->commandIsRun())
        {
            return 0;
        }
        Sleep(ReadPeriod);
    }
    return 0;
}

int Stm32tool::push2Terminal(const char *str)
{
    return linenoise::win32_write(STDOUT_FILENO, str, strlen(str));
}