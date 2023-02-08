#ifndef sys_random_h
#define sys_random_h

#define HAVE_GETRANDOM

int getrandom(void *buf, size_t buflen, unsigned int flags);

#endif