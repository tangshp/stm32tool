#ifndef _COMMANDCALLBACK_H_
#define _COMMANDCALLBACK_H_

#include "message.h"
#include <stdint.h>
#include <string>

bool initMsgManager();

bool packFuncFromLine(const std::string &line, Message_t *pTmessage);

bool registerCallback(uint32_t msgId, MessagePackageFuncPtr pTfunc);

bool setLed_pack(uint32_t messageId, Args &args, Message_t *pTmessage);

bool callBack(uint32_t messageId, Args &args, Message_t *pTmessage);

#endif
