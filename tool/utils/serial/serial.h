#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdint.h>

#if defined(__WIN32__) || defined(__CYGWIN__)
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#endif

#define BUFFER_SIZE 256
#define TIME_OUT 20 // 20 ms

enum Parity
{
	NO,
	EVEN,
	ODD
};

class Serial
{
private:
#if defined(__WIN32__) || defined(__CYGWIN__)
	HANDLE m_fd;
	DCB m_conf;
#else
	int m_fd;
	struct termios m_oldtio;
	struct termios m_newtio;
#endif

public:
	char Open(const char *port, uint32_t baud, uint8_t bits, Parity parity, uint8_t stopbit);
	void Close(void);
	char Write(char *buffer, int length);
	int Read(char *buffer);
	int ReadLine(unsigned char *buffer);
};

#endif
