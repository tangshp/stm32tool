#include <string.h>
#include "message.h"

static Msg2Name_t msg2Name[] = {
    {CMD_LED0, "CMD_LED0"},
    {CMD_LED1, "CMD_LED1"},
    {CMD_BEEP, "CMD_BEEP"},
    {RESP_LED0, "RESP_LED0"},
    {RESP_LED1, "RESP_LED1"},
    {RESP_BEEP, "RESP_BEEP"},
};

static size_t numMsg2Name = NElEMENT(msg2Name);

bool convert2MessageSerial(const Message_t *pMessage, MessageSerial_t *pMessageSerial)
{
    if (NULL == pMessage || NULL == pMessageSerial)
    {
        return false;
    }
    pMessageSerial->beginFlag[0] = (uint8_t)(BeginFlag | 0XFF);
    pMessageSerial->beginFlag[1] = (uint8_t)(BeginFlag >> 8);
    pMessageSerial->endFlag[0] = (uint8_t)(EndFlag | 0XFF);
    pMessageSerial->endFlag[1] = (uint8_t)(EndFlag >> 8);
    memcpy(&(pMessageSerial->message), pMessage, sizeof(Message_t));
    return true;
}

const char *nameFromMsg(uint32_t messageNo)
{
    for (size_t i = 0; i < numMsg2Name; i++)
    {
        if (messageNo == msg2Name[i].msgNo)
        {
            return (const char *)(msg2Name[i].name);
        }
    }
    return (const char *)(msg2Name[numMsg2Name - 1].name);
}
