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
#include <string.h>
#include "qdecoder.h"

int main(void)
{
#ifdef ENABLE_FASTCGI
    while(FCGI_Accept() >= 0) {
#endif
    // Parse (GET/COOKIE/POST) queries.
    qentry_t *req = qcgireq_parse(NULL, 0);

    const char *mode = req->getstr(req, "mode", false);
    const char *name = req->getstr(req, "cname", false);
    const char *value = req->getstr(req, "cvalue", false);

    if (mode == NULL) { // View Cookie
        qcgires_setcontenttype(req, "text/plain");
        printf("Total %d entries\n", req->size(req));
        req->print(req, stdout, true);
    } else if (!strcmp(mode, "set")) { // Set Cookie
        if (name == NULL || value == NULL) {
            qcgires_error(req, "Query not found");
        }
        if (strlen(name) == 0) {
            qcgires_error(req, "Empty cookie name can not be stored.");
        }

        qcgires_setcookie(req, name, value, 0, NULL, NULL, false);
        qcgires_setcontenttype(req, "text/html");
        printf("Cookie('%s'='%s') entry is stored.<br>\n", name, value);
        printf("Click <a href='cookie.cgi'>here</a> to view your cookies\n");
    } else if (!strcmp(mode, "remove")) { // Remove Cookie
        if (name == NULL) {
            qcgires_error(req, "Query not found");
        }
        if (!strcmp(name, "")) {
            qcgires_error(req, "Empty cookie name can not be removed.");
        }

        qcgires_removecookie(req, name, NULL, NULL, false);
        qcgires_setcontenttype(req, "text/html");
        printf("Cookie('%s') entry is removed.<br>\n", name);
        printf("Click <a href='cookie.cgi'>here</a> to view your cookies\n");
    } else {
        qcgires_error(req, "Unknown mode.");
    }

    req->free(req);
#ifdef ENABLE_FASTCGI
    }
#endif
    return 0;
}
