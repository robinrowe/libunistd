// termios.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT
// Serial Ports

#ifndef termios_h
#define termios_h

#include "../portable/stub.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

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

inline
int tcgetattr(int fd, struct termios *termios_p)
{   STUB_NEG(tcgetattr);
}

inline
int tcsetattr(int fd, int optional_actions,const struct termios *termios_p)
{   STUB_NEG(tcsetattr);
}

inline
int tcsendbreak(int fd, int duration)
{   STUB_NEG(tcsendbreak);
}

inline
int tcdrain(int fd)
{   STUB_NEG(tcdrain);
}

inline
int tcflush(int fd, int queue_selector)
{   STUB_NEG(tcflush);
}

inline
int tcflow(int fd, int action)
{   STUB_NEG(tcflow);
}

inline
void cfmakeraw(struct termios *termios_p)
{   STUB(cfmakeraw);
}

inline
speed_t cfgetispeed(const struct termios *termios_p)
{   STUB_NEG(cfgetispeed);
}

inline
speed_t cfgetospeed(const struct termios *termios_p)
{   STUB_NEG(cfgetospeed);
}

inline
int cfsetispeed(struct termios *termios_p, speed_t speed)
{   STUB_NEG(cfsetispeed);
}

inline
int cfsetospeed(struct termios *termios_p, speed_t speed)
{   STUB_NEG(cfsetospeed);
}

inline
int cfsetspeed(struct termios *termios_p, speed_t speed)
{   STUB_NEG(cfsetspeed);
}

#if 0

enum
{	TCGETS,
	TCSETS,
	TCSETSW,
	TCSETSF,
	TCGETA,
	TCSETA,
	TCSETAW,
	TCSETAF,
	TIOCGLCKTRMIOS,
	TIOCSLCKTRMIOS,
	TIOCGWINSZ,
	TIOCSWINSZ,
	TCSBRK,
	TCSBRKP,
	TIOCSBRK,
	TIOCCBRK,
	TCXONC,
	TIOCINQ,
	TIOCOUTQ,
	TCFLSH,
	TIOCSTI,
	TIOCCONS,
	TIOCSCTTY,
	TIOCNOTTY,
	TIOCGPGRP,
	TIOCSPGRP,
	TIOCGSID,
	TIOCEXCL,
	TIOCGEXCL,
	TIOCNXCL,
	TIOCGETD,
	TIOCSETD,
	TIOCPKT,
	TIOCPKT_FLUSHREAD,
	TIOCPKT_FLUSHWRITE,
	TIOCPKT_STOP,
	TIOCPKT_START,
	TIOCPKT_DOSTOP,
	TIOCPKT_NOSTOP,
	TIOGCPKT,
	TIOCSPTLCK,
	TIOCGPTLCK,
	TIOCMGET,
	TIOCMSET,
	TIOCMBIC,
	TIOCMBIS,
	TIOCM_LE,
	TIOCM_DTR,
	TIOCM_RTS,
	TIOCM_ST,
	TIOCM_SR,
	TIOCM_CTS,
	TIOCM_CAR,
	TIOCM_CD,
	TIOCM_RNG,
	TIOCM_RI,
	TIOCM_DSR,
	TIOCMIWAIT,
	TIOCGICOUNT,
	TIOCGSOFTCAR,
	TIOCSSOFTCAR,
	TIOCLINUX
};
#endif
#ifdef __cplusplus
}
#endif

#endif
