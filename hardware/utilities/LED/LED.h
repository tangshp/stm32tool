#ifndef _LED_H_
#define _LED_H_
#include "message.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void LED_Turnover();

    void LED_TurnoverBitBand();

    bool setLED(LED_State state);

    bool LED_Cb(Message_t *pTmessage, board_message_t *board_message);

#ifdef __cplusplus
}
#endif

#endif