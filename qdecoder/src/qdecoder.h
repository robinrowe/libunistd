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
 * $Id: qdecoder.h 655 2012-12-07 22:12:44Z seungyoung.kim $
 ******************************************************************************/

/**
 * qDecoder Header file
 *
 * @file qdecoder.h
 */

#ifndef _QDECODER_H
#define _QDECODER_H

#define _Q_PRGNAME  "qdecoder"
#define _Q_VERSION  "12.0.4"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/*
 * Types and definitions
 */

typedef struct qentry_s qentry_t;
typedef struct qentobj_s qentobj_t;

typedef enum {
    Q_CGI_ALL    = 0,
    Q_CGI_COOKIE = 0x01,
    Q_CGI_POST   = 0x02,
    Q_CGI_GET    = 0x04
} Q_CGI_T;

/*
 * qcgireq.c
 */
extern qentry_t *qcgireq_setoption(qentry_t *request, bool filemode,
                                   const char *basepath, int clearold);
extern qentry_t *qcgireq_parse(qentry_t *request, Q_CGI_T method);
extern char *qcgireq_getquery(Q_CGI_T method);

/*
 * qcgires.c
 */
extern bool qcgires_setcookie(qentry_t *request, const char *name,
                              const char *value, int expire, const char *path,
                              const char *domain, bool secure);
extern bool qcgires_removecookie(qentry_t *request, const char *name,
                                 const char *path, const char *domain,
                                 bool secure);

extern bool qcgires_setcontenttype(qentry_t *request, const char *mimetype);
extern const char *qcgires_getcontenttype(qentry_t *request);
extern bool qcgires_redirect(qentry_t *request, const char *uri);
extern int qcgires_download(qentry_t *request, const char *filepath,
                            const char *mimetype);
extern void qcgires_error(qentry_t *request, char *format, ...);

/*
 * qcgisess.c
 */
extern qentry_t  *qcgisess_init(qentry_t *request, const char *dirpath);
extern bool qcgisess_settimeout(qentry_t *session, time_t seconds);
extern const char *qcgisess_getid(qentry_t *session);
extern time_t qcgisess_getcreated(qentry_t *session);
extern bool qcgisess_save(qentry_t *session);
extern bool qcgisess_destroy(qentry_t *session);

/*
 * qentry.c - Linked-List Table
 */

/* public functions */
extern qentry_t *qEntry(void);

/* qentry container */
struct qentry_s {
    /* public functions */
    bool (*put) (qentry_t *entry, const char *name, const void *data,
                 size_t size, bool replace);
    bool (*putstr) (qentry_t *entry, const char *name, const char *str,
                    bool replace);
    bool (*putstrf) (qentry_t *entry, bool replace, const char *name,
                     const char *format, ...);
    bool (*putint) (qentry_t *entry, const char *name, int num, bool replace);

    void *(*get) (qentry_t *entry, const char *name, size_t *size, bool newmem);
    void *(*getlast) (qentry_t *entry, const char *name, size_t *size,
                      bool newmem);
    char *(*getstr) (qentry_t *entry, const char *name, bool newmem);
    char *(*getstrf) (qentry_t *entry, bool newmem, const char *namefmt, ...);
    char *(*getstrlast) (qentry_t *entry, const char *name, bool newmem);

    int (*getint) (qentry_t *entry, const char *name);
    int (*getintlast) (qentry_t *entry, const char *name);

    void *(*caseget) (qentry_t *entry, const char *name, size_t *size,
                      bool newmem);
    char *(*casegetstr) (qentry_t *entry, const char *name, bool newmem);
    int (*casegetint) (qentry_t *entry, const char *name);

    bool (*getnext) (qentry_t *entry, qentobj_t *obj, const char *name,
                     bool newmem);

    int (*size) (qentry_t *entry);
    int (*remove) (qentry_t *entry, const char *name);
    bool (*truncate) (qentry_t *entry);
    bool (*reverse) (qentry_t *entry);

    bool (*save) (qentry_t *entry, const char *filepath);
    int (*load) (qentry_t *entry, const char *filepath);

    bool (*print) (qentry_t *entry, FILE *out, bool print_data);
    bool (*free) (qentry_t *entry);

    /* private variables */
    int num;            /*!< number of objects */
    qentobj_t *first;   /*!< first object pointer */
    qentobj_t *last;    /*!< last object pointer */
};

/* qentry object */
struct qentobj_s {
    char *name;          /*!< object name */
    void *data;          /*!< data object */
    size_t size;         /*!< object size */
    qentobj_t *next;     /*!< link pointer */
};

#ifdef __cplusplus
}
#endif

#endif /*_QDECODER_H */
