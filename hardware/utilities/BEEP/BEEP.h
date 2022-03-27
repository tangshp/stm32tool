#ifndef _BEEP_H_
#define _BEEP_H_
#include "message.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include "Bit_Band.h"
#define BEEP PBout(8)
    void BEEP_Turnover();

    void BEEP_TurnoverBitBand();
    bool BEEP_Cb(Message_t *pTmessage, board_message_t *board_message);
#ifdef __cplusplus
}
#endif

#endif