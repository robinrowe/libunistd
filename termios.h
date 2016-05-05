// termios.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT
// Serial Ports

#ifndef termios_h
#define termios_h

enum
{	CS7,
	PARENB,          
	CS8, 
	PARODD,
	CSIZE,
	CSTOPB
};

typedef int tcflag_t;
typedef char cc_t;
#define NCCS 255

struct termios
{	tcflag_t c_iflag;
	tcflag_t c_oflag; 
	tcflag_t c_cflag;
	tcflag_t c_lflag; 
	cc_t c_cc[NCCS];
};

inline
int tcgetattr(int fd, struct termios *termios_p)
{	return 0;
}

inline
int tcsetattr(int fileDescriptor, int actions,struct termios *terminto)
{	return 0;
}

#define O_NOCTTY 0
#define O_NDELAY 0

enum
{	CRTSCTS,
	VMIN,
	VTIME,
	IGNBRK,
	BRKINT,
	ICRNL,
	INLCR,
	PARMRK,
	INPCK,
	ISTRIP,
	IXON
};

enum
{	ECHO,
	ECHONL,
	ICANON,
	IEXTEN,
	ISIG,
	TCSANOW
};

enum
{	CLOCAL,
	CREAD,
	ECHOE,
	IXOFF,
	IXANY,
	IGNCR,
	BSDLY,
	CRDLY,
	ONLCR,
	OCRNL,
	ONLRET,
	OFDEL,
	OFILL,
	OPOST
};

enum
{	B1200,
	B4800,
	B9600,
	B19200,
	B38400,
	B57600,
	B115200
};

enum
{	TCIFLUSH,
	TCOFLUSH,
	TCIOFLUSH,
	TCOOFF,
	TCOON,
	TCIOFF,
	TCION
};

enum
{	TIOCMGET,
	TIOCMSET,
	TIOCMBIC,
	TIOCMBIS 
};

enum
{	TIOCM_LE,	// DSR (data set ready/line enable)
	TIOCM_DTR,	// DTR (data terminal ready)
	TIOCM_RTS,	// RTS (request to send)
	TIOCM_ST,	// Secondary TXD (transmit)
	TIOCM_SR,	// Secondary RXD (receive)
	TIOCM_CTS,	// CTS (clear to send)
	TIOCM_CAR,	// DCD (data carrier detect)
	TIOCM_CD,	// see TIOCM_CAR
	TIOCM_RNG,	// RNG (ring)
	TIOCM_RI,	// see TIOCM_RNG
	TIOCM_DSR	// DSR (data set ready)
};

typedef int speed_t;

int tcgetattr(int fd, struct termios *termios_p);

int tcsetattr(int fd, int optional_actions,const struct termios *termios_p);

int tcsendbreak(int fd, int duration);

int tcdrain(int fd);

int tcflush(int fd, int queue_selector);

int tcflow(int fd, int action);

void cfmakeraw(struct termios *termios_p);

speed_t cfgetispeed(const struct termios *termios_p);

speed_t cfgetospeed(const struct termios *termios_p);

int cfsetispeed(struct termios *termios_p, speed_t speed);

int cfsetospeed(struct termios *termios_p, speed_t speed);

int cfsetspeed(struct termios *termios_p, speed_t speed);


inline
int cfsetispeed(struct termios *termios_p, speed_t speed)
{	return 0;
}

inline
int cfsetospeed(struct termios *termios_p, speed_t speed)
{	return 0;
}

inline
speed_t cfgetispeed(const struct termios *termios_p)
{	return 0;
}

inline
speed_t cfgetospeed(const struct termios *termios_p)
{	return 0;
}

#endif
