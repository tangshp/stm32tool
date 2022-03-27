#include "usart.h"
#include "vlog.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
// #ifdef DEBUG

#define which_UART(x) huart##x

#ifdef LOG_UARTx
#define HUART LOG_UARTx
#else
#define HUART huart1
#endif

#define MAX_STR 1024
char txStr[MAX_STR] = {0};

int print(const char *str)
{
    return (HAL_UART_Transmit(&HUART, (uint8_t *)str, strlen(str), 0XFFFF));
}

int flushOut(const char *__format, va_list args)
{
    vsnprintf(txStr, MAX_STR, __format, args);
    return (HAL_UART_Transmit(&HUART, (uint8_t *)txStr, strlen(txStr), 0XFFFF));
}

int printf(const char *__format, ...)
{
    va_list list;
    va_start(list, __format);
    HAL_StatusTypeDef status = flushOut(__format, list);
    va_end(list); // 置空 args
    return status;
}

int Log(int log_level, ...)
{
    switch (log_level)
    {
    case I:
        printf("[ \033[1m\033[32mINFO\033[0m ] : ");
        break;
    case E:
        printf("[ \033[1m\033[31mERRO\033[0m ] : ");
        break;
    case W:
        printf("[ \033[1m\033[33mWARN\033[0m ] : ");
        break;
    default:
        break;
    }

    va_list args;
    va_start(args, log_level); // 初始化 args
    char *format;
    format = va_arg(args, char *); // 获取变参数的第一个参数
    HAL_StatusTypeDef status = flushOut(format, args);
    va_end(args); // 置空 args

    HAL_UART_Transmit(&HUART, (uint8_t *)ENTER, strlen(ENTER), 0XFFFF);
    return status;
}
// #else
// int printf(const char *__format, ...)
// {
//     return HAL_OK;
// }
// int Log(int log_level, ...)
// {
//     return HAL_OK;
// }
// #endif