/******************************************************************************
 * qDecoder - http://www.qdecoder.org
 *
 * Copyright (c) 2000-2012 Seungyoung Kim.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
 * $Id: internal.h 636 2012-05-07 23:40:43Z seungyoung.kim $
 ******************************************************************************/

#ifndef _QINTERNAL_H
#define _QINTERNAL_H

/*
 * Internal Macros
 */
#ifdef BUILD_DEBUG
#define DEBUG(fmt, args...) fprintf(stderr, "[DEBUG] " fmt " (%s:%d)\n", ##args, __FILE__, __LINE__);
#else
// rsr #define DEBUG(fms, args...)
#define DEBUG printf
#endif  /* BUILD_DEBUG */

/*
 * Macro Functions
 */
#define CONST_STRLEN(x)     (sizeof(x) - 1)

#define DYNAMIC_VSPRINTF(s, f)                                          \
    do {                                                                \
        int _strsize;                                                \
        for(_strsize = 1024; ; _strsize *= 2) {                         \
            s = (char*)malloc(_strsize);                                \
            if(s == NULL) {                                             \
                DEBUG("DYNAMIC_VSPRINTF(): can't allocate memory.");    \
                break;                                                  \
            }                                                           \
            va_list _arglist;                                           \
            va_start(_arglist, f);                                      \
            int _n = vsnprintf(s, _strsize, f, _arglist);               \
            va_end(_arglist);                                           \
            if(_n >= 0 && _n < _strsize) break;                         \
            free(s);                                                    \
        }                                                               \
    } while(0)

/*
 * Internal Definitions
 */
#define CRLF "\r\n"
#define MAX_LINEBUF (1023+1)
#define DEF_DIR_MODE  (S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)
#define DEF_FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

/*
 * qInternalCommon.c
 */
extern char  _q_x2c(char hex_up, char hex_low);
extern char *_q_makeword(char *str, char stop);
extern char *_q_urlencode(const void *bin, size_t size);
extern size_t _q_urldecode(char *str);
extern char *_q_fgets(char *str, size_t size, FILE *fp);
extern char *_q_fgetline(FILE *fp, size_t initsize);
extern int _q_unlink(const char *pathname);
extern char *_q_strcpy(char *dst, size_t size, const char *src);
extern char *_q_strtrim(char *str);
extern char *_q_strunchar(char *str, char head, char tail);
extern char *_q_filename(const char *filepath);
extern off_t _q_filesize(const char *filepath);
extern off_t _q_iosend(FILE *outfp, FILE *infp, off_t nbytes);
extern int _q_countread(const char *filepath);
extern bool _q_countsave(const char *filepath, int number);

#endif  /* _QINTERNAL_H */
