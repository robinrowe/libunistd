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
 * $Id: internal.c 636 2012-05-07 23:40:43Z seungyoung.kim $
 ******************************************************************************/

#ifdef ENABLE_FASTCGI
#include "fcgi_stdio.h"
#else
#include <stdio.h>
#endif
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "qdecoder.h"
#include "internal.h"

// Change two hex character to one hex value.
char _q_x2c(char hex_up, char hex_low)
{
    char digit;

    digit = 16 * (hex_up >= 'A' ? ((hex_up & 0xdf) - 'A') + 10 : (hex_up - '0'));
    digit += (hex_low >= 'A' ? ((hex_low & 0xdf) - 'A') + 10 : (hex_low - '0'));

    return digit;
}

char *_q_makeword(char *str, char stop)
{
    char *word;
    int  len, i;

    for (len = 0; ((str[len] != stop) && (str[len])); len++);
    word = (char *)malloc(sizeof(char) * (len + 1));

    for (i = 0; i < len; i++) word[i] = str[i];
    word[i] = '\0';

    if (str[len])len++;
    for (i = len; str[i]; i++) str[i - len] = str[i];
    str[i - len] = '\0';

    return word;
}

char *_q_urlencode(const void *bin, size_t size)
{
    const char URLCHARTBL[256] = {
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , /* 00-0F */
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , /* 10-1F */
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'-','.','/', /* 20-2F */
        '0','1','2','3','4','5','6','7','8','9',':', 0 , 0 , 0 , 0 , 0 , /* 30-3F */
        '@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O', /* 40-4F */
        'P','Q','R','S','T','U','V','W','X','Y','Z', 0 ,'\\',0 , 0 ,'_', /* 50-5F */
        0 ,'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o', /* 60-6f */
        'p','q','r','s','t','u','v','w','x','y','z', 0 , 0 , 0 , 0 , 0 , /* 70-7F */
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , /* 80-8F */
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , /* 90-9F */
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , /* A0-AF */
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , /* B0-BF */
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , /* C0-CF */
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , /* D0-DF */
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , /* E0-EF */
        0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0   /* F0-FF */
    }; // 0 means must be encoded.

    if (bin == NULL) return NULL;
    if (size == 0) return strdup("");

    // malloc buffer
    char *pszEncStr = (char *)malloc((size * 3) + 1);
    if (pszEncStr == NULL) return NULL;

    char *pszEncPt = pszEncStr;
    char *pBinPt = (char *)bin;
    const char *pBinEnd = (pBinPt + size - 1);
    for (; pBinPt <= pBinEnd; pBinPt++) {
        if (URLCHARTBL[(int)(*pBinPt)] != 0) {
            *pszEncPt++ = *pBinPt;
        } else {
            unsigned char cUpper4 = (*pBinPt >> 4);
            unsigned char cLower4 = (*pBinPt & 0x0F);

            *pszEncPt++ = '%';
            *pszEncPt++ = (cUpper4 < 0x0A) ? (cUpper4 + '0') : ((cUpper4 - 0x0A) + 'a');
            *pszEncPt++ = (cLower4 < 0x0A) ? (cLower4 + '0') : ((cLower4 - 0x0A) + 'a');
        }
    }
    *pszEncPt = '\0';

    return pszEncStr;
}

size_t _q_urldecode(char *str)
{
    if (str == NULL) {
        return 0;
    }

    char *pEncPt, *pBinPt = str;
    for (pEncPt = str; *pEncPt != '\0'; pEncPt++) {
        switch (*pEncPt) {
            case '+': {
                *pBinPt++ = ' ';
                break;
            }
            case '%': {
                *pBinPt++ = _q_x2c(*(pEncPt + 1), *(pEncPt + 2));
                pEncPt += 2;
                break;
            }
            default: {
                *pBinPt++ = *pEncPt;
                break;
            }
        }
    }
    *pBinPt = '\0';

    return (pBinPt - str);
}

// This function is perfectly same as fgets();
char *_q_fgets(char *str, size_t size, FILE *fp)
{
    int c;
    char *ptr;

    for (ptr = str; size > 1; size--) {
        c = fgetc(fp);
        if (c == EOF) break;
        *ptr++ = (char)c;
        if (c == '\n') break;
    }

    *ptr = '\0';
    if (strlen(str) == 0) return NULL;

    return str;
}

char *_q_fgetline(FILE *fp, size_t initsize)
{
    size_t memsize = initsize;

    char *str = (char *)malloc(memsize);
    if (str == NULL) return NULL;

    char *ptr;
    size_t readsize;
    for (ptr = str, readsize = 0; ;) {
        int c = fgetc(fp);
        if (c == EOF) {
            if (readsize == 0) {
                free(str);
                return NULL;
            }
            break;
        }

        *ptr++ = (char)c;
        readsize++;
        if (readsize == memsize) {
            memsize *= 2;
            char *strtmp = (char *)malloc(memsize);
            if (strtmp == NULL) {
                free(str);
                return NULL;
            }

            memcpy(strtmp, str, readsize);
            free(str);
            str = strtmp;
            ptr = str + readsize;
        }

        if (c == '\n') break;
    }

    *ptr = '\0';

    return str;
}

/* win32 compatible */
int _q_unlink(const char *pathname)
{
#ifdef _WIN32
    return _unlink(pathname);
#endif
    return unlink(pathname);
}

char *_q_strcpy(char *dst, size_t size, const char *src)
{
    if (dst == NULL || size == 0 || src == NULL) return dst;

    size_t copylen = strlen(src);
    if (copylen >= size) copylen = size - 1;
    memmove((void *)dst, (void *)src, copylen);
    dst[copylen] = '\0';

    return dst;
}

char *_q_strtrim(char *str)
{
    int i, j;

    if (str == NULL) return NULL;
    for (j = 0; str[j] == ' ' || str[j] == '\t' || str[j] == '\r' || str[j] == '\n'; j++);
    for (i = 0; str[j] != '\0'; i++, j++) str[i] = str[j];
    for (i--; (i >= 0) && (str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n'); i--);
    str[i+1] = '\0';

    return str;
}

char *_q_strunchar(char *str, char head, char tail)
{
    if (str == NULL) return NULL;

    int len = strlen(str);
    if (len >= 2 && str[0] == head && str[len-1] == tail) {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }

    return str;
}

char *_q_filename(const char *filepath)
{
    char *path = strdup(filepath);
    const char *bname = basename(path);
    char *filename = strdup(bname);
    free(path);
    return filename;
}

off_t _q_filesize(const char *filepath)
{
    struct stat finfo;
    if (stat(filepath, &finfo) < 0) return -1;
    return finfo.st_size;
}

#define QIOSEND_CHUNK_SIZE (1024 * 4)
off_t _q_iosend(FILE *outfp, FILE *infp, off_t nbytes) {
    if (nbytes == 0) return 0;

    unsigned char buf[QIOSEND_CHUNK_SIZE];
    off_t total = 0; // total size sent
    while (total < nbytes) {
        size_t chunksize; // this time sending size
        if (nbytes - total <= sizeof(buf)) chunksize = nbytes - total;
        else chunksize = sizeof(buf);

        // read
        size_t rsize = fread(buf, 1, chunksize, infp);
        if (rsize == 0) break;
        DEBUG("read %zu", rsize);

        // write
        size_t wsize = fwrite(buf, 1, rsize, outfp);
        if (wsize == 0) break;
        DEBUG("write %zu", wsize);

        total += wsize;
        if (rsize != wsize) {
            DEBUG("size mismatch. read:%zu, write:%zu", rsize, wsize);
            break;
        }
    }

    if (total > 0) return total;
    return -1;
}

int _q_countread(const char *filepath)
{
    int fd = open(filepath, O_RDONLY, 0);
    if (fd < 0) return 0;

    char buf[10+1];
    ssize_t readed = read(fd, buf, (sizeof(buf) - 1));
    close(fd);

    if (readed > 0) {
        buf[readed] = '\0';
        return atoi(buf);
    }
    return 0;
}

bool _q_countsave(const char *filepath, int number)
{
    int fd = open(filepath, O_CREAT|O_WRONLY|O_TRUNC, DEF_FILE_MODE);
    if (fd < 0) return false;

    char buf[10+1];
    snprintf(buf, sizeof(buf), "%d", number);
    ssize_t updated = write(fd, buf, strlen(buf));
    close(fd);

    if (updated > 0) return true;
    return false;
}
