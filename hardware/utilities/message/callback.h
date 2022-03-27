#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include "message.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef bool (*FuncBoardCallbackPtr_t)(Message_t *pTmessage, board_message_t *board_message);
    bool registerCb(uint32_t messageId, FuncBoardCallbackPtr_t callback);
    bool initCallback();
    bool messageProceccRun(Message_t *pTmessage, board_message_t *board_message);
#ifdef __cplusplus
}
#endif
#endif