#include "main.h"
#include "Bit_Band.h"
#include <stdbool.h>
#include "LED.h"
#include "BEEP.h"
#include "stm32f1xx_hal_gpio.h"
#include "vlog.h"

static GPIO_PinState LED0_pinState = GPIO_PIN_RESET;
static GPIO_PinState LED1_pinState = GPIO_PIN_SET;

#define LED1 PEout(5)
#define LED0 PBout(5)

#define Message2GPIO(pTmessage) ((*((LED_State *)pTmessage->data) == LED_ON) ? GPIO_PIN_RESET : GPIO_PIN_SET)
#define CheckLEDStatus(LED, Name)         \
    {                                     \
        if (GPIO_PIN_RESET == LED)        \
        {                                 \
            Log(I, "%s is open.", Name);  \
        }                                 \
        else                              \
        {                                 \
            Log(I, "%s is close.", Name); \
        }                                 \
    }

bool LED_Cb(Message_t *pTmessage, board_message_t *board_message)
{
    if (NULL == pTmessage)
    {
        return false;
    }
    if (0 == pTmessage->size)
    { // check statue
        switch (pTmessage->messageNo)
        {
        case CMD_LED0:
            CheckLEDStatus(LED0, "led0");
            break;

        case CMD_LED1:
            CheckLEDStatus(LED1, "led1");
            break;

        default:
            break;
        }
        return true;
    }
    else
    { // set
        switch (pTmessage->messageNo)
        {
        case CMD_LED0:
            LED0 = Message2GPIO(pTmessage);
            // CheckLEDStatus(LED0, "led0");
            Log(I, "led0 is turned %s.", ((*((LED_State *)pTmessage->data) == LED_ON) ? "on" : "off"));
            break;
        case CMD_LED1:
            LED1 = Message2GPIO(pTmessage);
            Log(I, "led1 is turned %s.", ((*((LED_State *)pTmessage->data) == LED_ON) ? "on" : "off"));
            // CheckLEDStatus(LED1, "led1");
            break;

        default:
            break;
        }
        return true;
    }
    return true;
}

bool setLED(LED_State state)
{
    if (LED_ON == state)
    {
        LED0 = GPIO_PIN_RESET;
    }
    else
    {
        LED0 = GPIO_PIN_SET;
    }
    return true;
}

void LED_Turnover()
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, LED1_pinState);
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, LED0_pinState);
    LED1_pinState = (~LED1_pinState) & 1u;
    LED0_pinState = (~LED0_pinState) & 1u;
}

void LED_TurnoverBitBand()
{
    LED0 = LED0_pinState;
    LED1 = LED1_pinState;
    LED1_pinState = (~LED1_pinState) & 1u;
    LED0_pinState = (~LED0_pinState) & 1u;
}