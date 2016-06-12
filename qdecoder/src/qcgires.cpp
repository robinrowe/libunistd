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
 * $Id: qcgires.c 654 2012-12-02 22:23:00Z seungyoung.kim $
 ******************************************************************************/

/**
 * @file qcgires.c CGI Response API
 */

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
#include <time.h>
#include <fcntl.h>
#include "qdecoder.h"
#include "internal.h"

/**
 * Set cookie
 *
 * @param request   a pointer of request structure
 * @param name      cookie name
 * @param value     cookie value
 * @param expire    expire related time in seconds (0 means end of session)
 * @param path      cookie path (NULL can current path)
 * @param domain    cookie domain (NULL means current domain)
 * @param secure    secure flag
 *
 * @return  true in case of success, otherwise returns false
 *
 * @code
 *   // Apply cookie in the current domain and directory for 1 day.
 *   qcgires_setcookie(req, "NAME", "VALUE", 86400, NULL, NULL, false);
 *
 *   // Apply cookie to the "/" directory of "*.qdecoder.org" until the
 *   // browser is closed.
 *   qcgires_setcookie(req, name, value, 0, "/", ".qdecoder.org", false);
 *
 *   // As for the followings, cookies will be set up only when security
 *   // requirements are satisfied.
 *   qcgires_setcookie(req, name, value, 0, NULL, NULL, true);
 * @endcode
 */
bool qcgires_setcookie(qentry_t *request, const char *name, const char *value,
                       int expire, const char *path, const char *domain, bool secure)
{
    if (qcgires_getcontenttype(request) != NULL) {
        DEBUG("Should be called before qcgires_setcontenttype().");
        return false;
    }

    char *encname = _q_urlencode(name, strlen(name));
    char *encvalue = _q_urlencode(value, strlen(value));
    char cookie[(4 * 1024) + 256];
    snprintf(cookie, sizeof(cookie), "%s=%s", encname, encvalue);
    free(encname), free(encvalue);

    if (expire != 0) {
        char gmtstr[sizeof(char) * (CONST_STRLEN("Mon, 00 Jan 0000 00:00:00 GMT") + 1)];
        time_t utctime = time(NULL) + expire;
        struct tm *gmtm = gmtime(&utctime);
        strftime(gmtstr, sizeof(gmtstr), "%a, %d %b %Y %H:%M:%S GMT", gmtm);

        strcat(cookie, "; expires=");
        strcat(cookie, gmtstr);
    }

    if (path != NULL) {
        if (path[0] != '/') {
            DEBUG("Path string(%s) must start with '/' character.", path);
            return false;
        }
        strcat(cookie, "; path=");
        strcat(cookie, path);
    }

    if (domain != NULL) {
        if (strstr(domain, "/") != NULL || strstr(domain, ".") == NULL) {
            DEBUG("Invalid domain name(%s).", domain);
            return false;
        }
        strcat(cookie, "; domain=");
        strcat(cookie, domain);
    }

    if (secure == true) {
        strcat(cookie, "; secure");
    }

    printf("Set-Cookie: %s" CRLF, cookie);

    return true;
}

/**
 * Remove cookie
 *
 * @param request   a pointer of request structure
 * @param name      cookie name
 * @param path      cookie path
 * @param domain    cookie domain
 * @param secure    secure flag
 *
 * @return      true in case of success, otherwise returns false
 *
 * @code
 *   qcgires_setcookie(req, "NAME", "VALUE", 0, NULL, NULL, NULL);
 *   qcgires_removecookie(req, "NAME", NULL, NULL, NULL);
 *
 *   qcgires_setcookie(req, "NAME", "VALUE", 0, "/", "www.qdecoder.org", NULL);
 *   qcgires_removecookie(req, "NAME", "/", "www.qdecoder.org", NULL);
 * @endcode
 */
bool qcgires_removecookie(qentry_t *request, const char *name, const char *path,
                          const char *domain, bool secure)
{
    return qcgires_setcookie(request, name, "", -1, path, domain, secure);
}

/**
 * Set responding content-type
 *
 * @param request   a pointer of request structure
 * @param mimetype  mimetype
 *
 * @return      true in case of success, otherwise returns false
 *
 * @code
 *   qcgires_setcontenttype(req, "text/html");
 * @endcode
 */
bool qcgires_setcontenttype(qentry_t *request, const char *mimetype)
{
    if (request != NULL &&
        request->getstr(request, "_Q_CONTENTTYPE", false) != NULL) {
        DEBUG("alreay set.");
        return false;
    }

    printf("Content-Type: %s" CRLF CRLF, mimetype);

    if (request != NULL) {
        request->putstr(request, "_Q_CONTENTTYPE", mimetype, true);
    }
    return true;
}

/**
 * Get content-type
 *
 * @param request   a pointer of request structure
 *
 * @return a pointer of mimetype string in case of success,
 *         otherwise returns NULL
 *
 * @code
 *   qcgires_setcontenttype(req, "text/html");
 * @endcode
 */
const char *qcgires_getcontenttype(qentry_t *request)
{
    if (request == NULL) return NULL;
    return request->getstr(request, "_Q_CONTENTTYPE", false);
}

/**
 * Send redirection header
 *
 * @param request   a pointer of request structure
 * @param uri       new URI
 *
 * @return      true in case of success, otherwise returns false
 *
 * @code
 *   qcgires_redirect(req, "http://www.qdecoder.org/");
 * @endcode
 */
bool qcgires_redirect(qentry_t *request, const char *uri)
{
    if (qcgires_getcontenttype(request) != NULL) {
        DEBUG("Should be called before qcgires_setcontenttype().");
        return false;
    }

    printf("Location: %s" CRLF CRLF, uri);
    return true;
}

/**
 * Force to send(download) file to client in accordance with given mime type.
 *
 * @param request   a pointer of request structure
 * @param filepath  file to send
 * @param mimetype  mimetype. NULL can be used for "application/octet-stream".
 *
 * @return      the number of bytes sent. otherwise(file not found) returns -1.
 *
 * @note
 * Do not call qcgires_getcontenttype() before.
 * The results of this function are the same as those acquired
 * when the corresponding files are directly linked to the Web.
 * But this is especially useful in preprocessing files to be downloaded
 * only with user certification and in enabling downloading those files,
 * which cannot be opend on the Web, only through specific programs.
 */
int qcgires_download(qentry_t *request, const char *filepath,
                     const char *mimetype)
{
    if (qcgires_getcontenttype(request) != NULL) {
        DEBUG("Should be called before qcgires_setcontenttype().");
        return -1;
    }

    FILE *fp;
    if (filepath == NULL || (fp = fopen(filepath, "r")) == NULL) {
        DEBUG("Can't open file.");
        return -1;
    }

    const char *mime;
    if (mimetype == NULL) mime = "application/octet-stream";
    else mime = mimetype;

    char *disposition;
    if (!strcmp(mime, "application/octet-stream")) disposition = "attachment";
    else disposition = "inline";

    char *filename = _q_filename(filepath);
    off_t filesize = _q_filesize(filepath);

    printf("Content-Disposition: %s;filename=\"%s\"" CRLF, disposition, filename);
    printf("Content-Transfer-Encoding: binary" CRLF);
    printf("Accept-Ranges: bytes" CRLF);
    printf("Content-Length: %lu" CRLF, (unsigned long)filesize);
    printf("Connection: close" CRLF);
    qcgires_setcontenttype(request, mime);

    free(filename);

    fflush(stdout);

    int sent = _q_iosend(stdout, fp, filesize);

    fclose(fp);
    return sent;
}

/**
 * Print out HTML error page and exit program
 *
 * @param request   a pointer of request structure
 * @param format    error message
 *
 * @return      none
 *
 * @code
 *   qcgires_error(req, "Error: can't find userid.");
 * @endcode
 */
void qcgires_error(qentry_t *request, char *format, ...)
{
    char *buf;
    DYNAMIC_VSPRINTF(buf, format);
    if (buf == NULL) {
        exit(EXIT_FAILURE);
    }

    if (getenv("REMOTE_ADDR") == NULL)  {
        printf("Error: %s\n", buf);
    } else {
        qcgires_setcontenttype(request, "text/html");

        printf("<html>\n");
        printf("<head>\n");
        printf("<title>Error: %s</title>\n", buf);
        printf("<script language='JavaScript'>\n");
        printf("  alert(\"%s\");\n", buf);
        printf("  history.back();\n");
        printf("</script>\n");
        printf("</head>\n");
        printf("</html>\n");
    }

    free(buf);
    if (request != NULL) request->free(request);
    exit(EXIT_FAILURE);
}
