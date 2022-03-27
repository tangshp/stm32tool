#include "commandCallback.h"
#include <string.h>
#include <vector>

extern command_id_t command_ids[];
extern uint32_t num_command_ids;
extern MessagePackageFuncPtr msgManager[];
#define maxNumArgs 8
#define maxNumChar 64 // 输入命令行最长字符个数

class Args
{
public:
    Args(const std::string &line)
    {
        if (0 == line.length())
        {
            printf("Error, 0==line.length()\n");
            return;
        }
        memcpy(chBuffer, line.c_str(), line.length());
        pCommand = strtok(chBuffer, " ");
        p = pCommand;
        this->parser();
    }

    bool parser()
    {
        uint8_t i = 0;
        while (p)
        {
            if (i < maxNumArgs)
            {
                args[i] = p;
                i++;
            }

            p = strtok(NULL, " ");
        }
        cnt = i;
        return true;
    }

    void print()
    {
        printf("pCommand:%s\n", pCommand);
        for (size_t i = 0; i < cnt; i++)
        {
            printf("%d:%s\n", i, args[i]);
        }
        printf("cnt:%d---------\n", cnt);
    }

    bool isMeeting(char *pTcmd)
    {
        if (0 == strcasecmp(pTcmd, pCommand))
        {
            return true;
        }
        return false;
    }

    bool haveArgs()
    {
        if (cnt == 1)
        {
            return false;
        }
        return true;
    }

    template <class T>
    bool nextArgs(T *pTvalue)
    {

        if (idx >= maxNumArgs)
        {
            return false;
        }

        if (std::is_same<T, uint8_t>::value)
        {
            if (NULL == pTvalue)
            {
                return false;
            }

            sscanf(args[idx], "%u", pTvalue);
            idx++;
            return true;
        }

        return true;
    }
    ~Args()
    {
    }

private:
    char *pCommand = NULL;
    char *p = NULL;
    char *args[maxNumArgs] = {NULL};
    char chBuffer[102] = {0};
    uint8_t cnt;
    uint8_t idx = 1;
};

bool packFuncFromLine(const std::string &line, Message_t *pTmessage)
{
    if (0 == line.length())
    {
        return NULL;
    }

    Args args(line);
    // args.print();

    char *pTcmd = NULL;
    for (size_t i = 0; i < num_command_ids; i++)
    {
        pTcmd = command_ids[i].commandStr;
        if (args.isMeeting(pTcmd))
        {
            // printf("message id is %08X\n", command_ids[i].messageId);
            callBack(command_ids[i].messageId, args, pTmessage);

            return true;
        }
    }
    printf("message id is unkown\n");
    return false;
}

bool callBack(uint32_t messageId, Args &args, Message_t *pTmessage)
{
    MessagePackageFuncPtr pTfunc = *(msgManager + INDEX_OF_CMD(messageId));
    if (NULL == pTfunc)
    {
        printf("NULL == pTfunc %d, please to check initMsgManager\n", INDEX_OF_CMD(messageId));
        return false;
    }
    return (*pTfunc)(messageId, args, pTmessage);
}

bool setLed_pack(uint32_t messageId, Args &args, Message_t *pTmessage)
{
    if (NULL == pTmessage)
    {
        return false;
    }
    pTmessage->messageNo = messageId;
    if (args.haveArgs())
    { // set on/off
        LED_State ledState;
        args.nextArgs((uint8_t *)&ledState);
        if (ledState >= 1)
        {
            ledState = LED_ON;
        }
        else
        {
            ledState = LED_OFF;
        }
        memcpy(pTmessage->data, &ledState, sizeof(ledState));
        pTmessage->size = sizeof(ledState);
    }
    else
    {
        // check state
        pTmessage->size = 0;
    }

    return true;
}

bool initMsgManager()
{
    registerCallback(CMD_LED0, &setLed_pack);
    registerCallback(CMD_LED1, &setLed_pack);
    registerCallback(CMD_BEEP, &setLed_pack);
    return true;
}

bool registerCallback(uint32_t msgId, MessagePackageFuncPtr pTfunc)
{
    size_t i = 0;
    if (NULL == pTfunc)
    {
        return false;
    }

    i = INDEX_OF_CMD(msgId);

    if (i < num_command_ids)
    {
        *(msgManager + i) = pTfunc;
        i++;
        return true;
    }
    return false;
}