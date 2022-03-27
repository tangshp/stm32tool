#include "callback.h"
#include "LED.h"
#include "BEEP.h"
#include <stddef.h>

uint32_t ID2IDX(uint32_t id)
{
    if (id > CMD_Begin && id < CMD_End)
    {
        return INDEX_OF_CMD(id);
    }
    else if (id > RESP_Begin && id < RESP_End)
    {
        return (id - RESP_Begin - 1);
    }
    else
        return 0XFFFFFFFF;
}

static FuncBoardCallbackPtr_t cbManager[NUM_CMD_RESP] = {NULL};

bool initCallback()
{
    registerCb(CMD_LED0, &LED_Cb);
    registerCb(CMD_LED1, &LED_Cb);
    registerCb(CMD_BEEP, &BEEP_Cb);

    return true;
}

bool registerCb(uint32_t messageId, FuncBoardCallbackPtr_t callback)
{
    if (NULL == callback)
    {
        return false;
    }
    cbManager[INDEX_OF_CMD(messageId)] = callback;
    return true;
}

bool messageProceccRun(Message_t *pTmessage, board_message_t *board_message)
{
    if (NULL == pTmessage)
    {
        return false;
    }
    uint32_t idx = ID2IDX(pTmessage->messageNo);
    if (idx >= RESP_End)
    {
        return false;
    }

    if (cbManager[idx])
    {
        return (*cbManager[idx])(pTmessage, board_message);
    }

    return false;
}