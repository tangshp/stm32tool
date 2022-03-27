#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdint.h>
#include <stdbool.h>
#include "messageNo.h"

#define MAX_LEN 16

#define BeginFlag 0XAABB
#define EndFlag 0XCCDD
class Args;
#ifdef __cplusplus
extern "C"
{
#endif

#define NElEMENT(arr) (sizeof(arr) / sizeof(arr[0]))

#pragma pack(push, 1)
    typedef struct Message
    {
        uint32_t messageNo;
        uint16_t size; // valid date length
        unsigned char data[MAX_LEN];
        unsigned char param[MAX_LEN];
    } Message_t;

    typedef struct MessageSerial
    {
        uint8_t beginFlag[2];
        Message_t message;
        uint8_t endFlag[2];
    } MessageSerial_t;
#pragma pack(pop)

    bool convert2MessageSerial(const Message_t *pMessage, MessageSerial_t *pMessageSerial);

    typedef bool (*MessagePackageFuncPtr)(uint32_t messageId, Args &args, Message_t *pTmessage);

#ifdef __cplusplus
}
#endif

#endif