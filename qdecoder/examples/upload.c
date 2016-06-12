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
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "qdecoder.h"

#define BASEPATH    "upload"

ssize_t savefile(const char *filepath, const void *buf, size_t size)
{

    int fd = open(filepath,
                  O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (fd < 0) return -1;

    ssize_t count = write(fd, buf, size);
    close(fd);

    return count;
}

int main(void)
{
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    // Parse queries.
    qentry_t *req = qcgireq_parse(NULL, 0);

    // get queries
    const char *text = req->getstr(req, "text", false);
    const char *filedata   = req->getstr(req, "binary", false);
    int filelength = req->getint(req, "binary.length");
    const char *filename   = req->getstr(req, "binary.filename", false);
    const char *contenttype = req->getstr(req, "binary.contenttype", false);

    // check queries
    if (text == NULL) qcgires_error(req, "Invalid usages.");
    if (filename == NULL || filelength == 0) {
        qcgires_error(req, "Select file, please.");
    }

    char  filepath[1024];
    sprintf(filepath, "%s/%s", BASEPATH, filename);

    if (savefile(filepath, filedata, filelength) < 0) {
        qcgires_error(req, "File(%s) open fail. Please make sure CGI or directory has right permission.",
                      filepath);
    }

    // result out
    qcgires_setcontenttype(req, "text/html");
    printf("You entered: <b>%s</b>\n", text);
    printf("<br><a href=\"%s\">%s</a> (%d bytes, %s) saved.",
           filepath, filename, filelength, contenttype);

    // dump
    printf("\n<p><hr>--[ DUMP INTERNAL DATA STRUCTURE ]--\n<pre>");
    req->print(req, stdout, false);
    printf("\n</pre>\n");

    // de-allocate
    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}
