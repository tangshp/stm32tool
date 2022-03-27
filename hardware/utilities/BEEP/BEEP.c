#include "BEEP.h"
#include "vlog.h"
#include "main.h"
#include "BEEP.h"
#include "stm32f1xx_hal_gpio.h"

static GPIO_PinState BEEP_pinState = GPIO_PIN_RESET;

void BEEP_Turnover()
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, LED1_Pin, BEEP_pinState);
    BEEP_pinState = (~BEEP_pinState) & 1u;
}

void BEEP_TurnoverBitBand()
{
    BEEP = BEEP_pinState;

    BEEP_pinState = (~BEEP_pinState) & 1u;
}

#define Message2GPIO(pTmessage) ((*((LED_State *)pTmessage->data) == LED_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define CheckBEEPStatus(BEEP, Name)       \
    {                                     \
        if (GPIO_PIN_SET == BEEP)         \
        {                                 \
            Log(I, "%s is open.", Name);  \
        }                                 \
        else                              \
        {                                 \
            Log(I, "%s is close.", Name); \
        }                                 \
    }

bool BEEP_Cb(Message_t *pTmessage, board_message_t *board_message)
{
    if (NULL == pTmessage)
    {
        return false;
    }
    if (0 == pTmessage->size)
    { // check statue
        CheckBEEPStatus(BEEP, "beep");
        return true;
    }
    else
    { // set
        BEEP = Message2GPIO(pTmessage);
        Log(I, "led1 is turned %s.", ((*((LED_State *)pTmessage->data) == LED_ON) ? "on" : "off"));
        // CheckBEEPStatus(BEEP, "beep");
        return true;
    }
    return true;
}