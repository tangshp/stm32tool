#ifndef _VLOG_H_
#define _VLOG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define ENTER "\r\n"

    enum Log_Level
    {
        I,
        E,
        W
    };
    int print(const char *str);

    int Log(int log_level, ...);

    int printf(const char *__format, ...);

#ifdef __cplusplus
}
#endif

#endif