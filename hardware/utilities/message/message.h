#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdint.h>
#include <stdbool.h>
#include "messageNo.h"

#define MAX_LEN 16

#define BeginFlag 0XAABB
#define EndFlag 0XCCDD

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

    typedef enum GPIO_State_t
    {
        GPIO_Open,
        GPIO_Close
    } GPIO_State_t;

    typedef union board_message_t
    {
        GPIO_State_t gpioState;
    } board_message_t;

#pragma pack(pop)

    typedef enum RecvState
    {
        RDY_BEGIN0,
        RDY_BEGIN1,
        RDY_MESSAGE,
        RDY_END
    } RecvState;

    bool convert2MessageSerial(const Message_t *pMessage, MessageSerial_t *pMessageSerial);
    const char *nameFromMsg(uint32_t messageNo);

#ifdef __cplusplus
}
#endif

#endif