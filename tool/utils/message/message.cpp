#include "message.h"
#include <string.h>
#include <stdio.h>

command_id_t command_ids[] = {
    {"led0", CMD_LED0, "l"},
    {"led1", CMD_LED1, "l"},
    {"beep", CMD_BEEP, "b"},
};

uint32_t num_command_ids = NElEMENT(command_ids);

MessagePackageFuncPtr msgManager[NElEMENT(command_ids)] = {NULL};

bool convert2MessageSerial(const Message_t *pMessage, MessageSerial_t *pMessageSerial)
{
    if (NULL == pMessage || NULL == pMessageSerial)
    {
        return false;
    }

    memcpy(&(pMessageSerial->message), pMessage, sizeof(Message_t));
    unsigned char *pChar = (unsigned char *)pMessageSerial;
    // for (size_t i = 0; i < sizeof(MessageSerial_t); i++)
    // {
    //     printf("%02X ", pChar[i]);
    // }
    // printf("\n");

    return true;
}
