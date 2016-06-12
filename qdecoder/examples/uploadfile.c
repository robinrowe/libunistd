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
 ******************************************************************************/

#ifdef ENABLE_FASTCGI
#include "fcgi_stdio.h"
#else
#include <stdio.h>
#endif
#include <stdlib.h>
#include <stdbool.h>
#include "qdecoder.h"

#define BASEPATH    "upload"
#define TMPPATH     "tmp"

int main(void)
{
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    // parse queries
    qentry_t *req = qcgireq_setoption(NULL, true, TMPPATH, 60);
    if (req == NULL) qcgires_error(req, "Can't set option.");

    req = qcgireq_parse(req, 0);
    if (req == NULL) qcgires_error(req, "Server error.");

    // get queries
    const char *text = req->getstr(req, "text", false);
    if (text == NULL) qcgires_error(req, "Invalid usages.");

    // result out
    qcgires_setcontenttype(req, "text/html");
    printf("You entered: <b>%s</b>\n", text);
    int i;
    for (i = 1; i <= 3; i++) {
        char name[31+1];
        sprintf(name, "binary%d.length", i);
        int length =  req->getint(req, name);
        if (length > 0) {
            sprintf(name, "binary%d.filename", i);
            const char *filename = req->getstr(req, name, false);
            sprintf(name, "binary%d.contenttype", i);
            const char *contenttype = req->getstr(req, name, false);
            sprintf(name, "binary%d.savepath", i);
            const char *savepath = req->getstr(req, name, false);

            char newpath[1024];
            sprintf(newpath, "%s/%s", BASEPATH, filename);

            if (rename(savepath, newpath) == -1) {
                qcgires_error(req, "Can't move uploaded file %s to %s", savepath, newpath);
            }
            printf("<br>File %d : <a href=\"%s\">%s</a> (%d bytes, %s) saved.",
                   i, newpath, filename, length, contenttype);
        }
    }

    // dump
    printf("\n<p><hr>--[ DUMP INTERNAL DATA STRUCTURE ]--\n<pre>");
    req->print(req, stdout, true);
    printf("\n</pre>\n");

    // de-allocate
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}
