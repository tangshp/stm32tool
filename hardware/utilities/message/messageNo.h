#ifndef _MESSAGENO_H_
#define _MESSAGENO_H_

#define CMD_BASE (0X0001)
#define RESP_BASE (CMD_BASE + 1)

// command message number id
#define CMD_Begin ((CMD_BASE << 16) + (0X0000))
#define CMD_LED0 ((CMD_BASE << 16) + (0X0001))
#define CMD_LED1 ((CMD_BASE << 16) + (0X0002))
#define CMD_BEEP ((CMD_BASE << 16) + (0X0003))
#define CMD_End ((CMD_BASE << 16) + (0X0004))

#define INDEX_OF_CMD(CMD_XXX) (CMD_XXX - CMD_Begin - 1)

#define RESP_Begin ((RESP_BASE << 16) + (0X0000))
#define RESP_LED0 ((RESP_BASE << 16) + (0X0001))
#define RESP_LED1 ((RESP_BASE << 16) + (0X0002))
#define RESP_BEEP ((RESP_BASE << 16) + (0X0003))
#define RESP_End ((RESP_BASE << 16) + (0X0004))

#define NUM_CMD_RESP (INDEX_OF_CMD(CMD_End) - 1)

typedef enum LED_State
{
    LED_OFF,
    LED_ON
} LED_State;

typedef struct Msg2Name
{
    uint32_t msgNo;
    unsigned char name[16];
} Msg2Name_t;

#endif