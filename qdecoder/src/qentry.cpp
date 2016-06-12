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
 * $Id: qentry.c 653 2012-12-01 08:01:19Z seungyoung.kim $
 ******************************************************************************/

/**
 * @file qentry.c Linked-list Data Structure API
 *
 * @code
 *   [Code sample - String]
 *
 *   // init a linked-list.
 *   qentry_t *entry = qEntry();
 *
 *   // insert a string element
 *   entry->putstr(entry, "str", "hello world", true);
 *
 *   // get the string.
 *   char *str = entry->getstr(entry, "str", false);
 *   if(str != NULL) {
 *     printf("str = %s\n", str);
 *     free(str);
 *   }
 *
 *   // print out all elements in the list.
 *   entry->print(entry, stdout, false);
 *
 *   // free the list.
 *   entry->free(entry);
 *
 *   [Result]
 * @endcode
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
#include <sys/stat.h>
#include "qdecoder.h"
#include "internal.h"

/*
 * Member method protos
 */
#ifndef _DOXYGEN_SKIP

#define _VAR        '$'
#define _VAR_OPEN   '{'
#define _VAR_CLOSE  '}'
#define _VAR_CMD    '!'
#define _VAR_ENV    '%'

static bool _put(qentry_t *entry, const char *name, const void *data,
                 size_t size, bool replace);
static bool _putstr(qentry_t *entry, const char *name, const char *str,
                    bool replace);
static bool _putstrf(qentry_t *entry, bool replace, const char *name,
                     const char *format, ...);
static bool _putint(qentry_t *entry, const char *name, int num, bool replace);

static void *_get(qentry_t *entry, const char *name, size_t *size, bool newmem);
static void *_getlast(qentry_t *entry, const char *name, size_t *size,
                      bool newmem);
static char *_getstr(qentry_t *entry, const char *name, bool newmem);
static char *_getstrf(qentry_t *entry, bool newmem, const char *namefmt, ...);
static char *_getstrlast(qentry_t *entry, const char *name, bool newmem);

static int _getint(qentry_t *entry, const char *name);
static int _getintlast(qentry_t *entry, const char *name);

static void *_caseget(qentry_t *entry, const char *name, size_t *size,
                      bool newmem);
static char *_casegetstr(qentry_t *entry, const char *name, bool newmem);
static int _casegetint(qentry_t *entry, const char *name);

static bool _getnext(qentry_t *entry, qentobj_t *obj, const char *name,
                     bool newmem);

static int _size(qentry_t *entry);
static int _remove(qentry_t *entry, const char *name);
static bool _truncate(qentry_t *entry);
static bool _reverse(qentry_t *entry);

static bool _save(qentry_t *entry, const char *filepath);
static int _load(qentry_t *entry, const char *filepath);

static bool _print(qentry_t *entry, FILE *out, bool print_data);
static bool _free(qentry_t *entry);

#endif

/**
 * Create new qentry_t linked-list object
 *
 * @return a pointer of malloced qentry_t structure in case of successful,
 *         otherwise returns NULL.
 *
 * @code
 *   qentry_t *entry = qEntry();
 * @endcode
 */
qentry_t *qEntry(void)
{
    qentry_t *entry = (qentry_t *)malloc(sizeof(qentry_t));
    if (entry == NULL) return NULL;

    memset((void *)entry, 0, sizeof(qentry_t));

    // member methods
    entry->put          = _put;
    entry->putstr       = _putstr;
    entry->putstrf      = _putstrf;
    entry->putint       = _putint;

    entry->get          = _get;
    entry->getlast      = _getlast;
    entry->getstr       = _getstr;
    entry->getstrf      = _getstrf;
    entry->getstrlast   = _getstrlast;

    entry->getint       = _getint;
    entry->getintlast   = _getintlast;

    entry->caseget      = _caseget;
    entry->casegetstr   = _casegetstr;
    entry->casegetint   = _casegetint;

    entry->getnext      = _getnext;

    entry->size         = _size;
    entry->remove       = _remove;
    entry->truncate     = _truncate;
    entry->reverse      = _reverse;

    entry->save         = _save;
    entry->load         = _load;

    entry->print        = _print;
    entry->free         = _free;

    return entry;
}

/**
 * qentry_t->put(): Store object into linked-list structure.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name.
 * @param   object  object pointer
 * @param   size    size of the object
 * @param   replace in case of false, just insert. in case of true, remove all
 *          same key then insert object if found.
 *
 * @return  true if successful, otherwise returns false.
 */
static bool _put(qentry_t *entry, const char *name,
                 const void *data, size_t size, bool replace)
{
    // check arguments
    if (entry == NULL || name == NULL || data == NULL || size <= 0) {
        return false;
    }

    // duplicate name
    char *dup_name = strdup(name);
    if (dup_name == NULL) return false;

    // duplicate object
    void *dup_data = malloc(size);
    if (dup_data == NULL) {
        free(dup_name);
        return false;
    }
    memcpy(dup_data, data, size);

    // make new object entry
    qentobj_t *obj = (qentobj_t *)malloc(sizeof(qentobj_t));
    if (obj == NULL) {
        free(dup_name);
        free(dup_data);
        return false;
    }
    obj->name = dup_name;
    obj->data = dup_data;
    obj->size = size;
    obj->next = NULL;

    // if replace flag is set, remove same key
    if (replace == true) _remove(entry, dup_name);

    // make chain link
    if (entry->first == NULL) entry->first = entry->last = obj;
    else {
        entry->last->next = obj;
        entry->last = obj;
    }

    entry->num++;

    return true;
}

/**
 * qentry_t->putstr(): Add string object into linked-list structure.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name.
 * @param   str string value
 * @param   replace in case of false, just insert. in case of true, remove all
 *                  same key then insert object if found.
 *
 * @return  true if successful, otherwise returns false.
 */
static bool _putstr(qentry_t *entry, const char *name, const char *str,
                    bool replace)
{
    size_t size = (str!=NULL) ? (strlen(str) + 1) : 0;
    return _put(entry, name, (const void *)str, size, replace);
}

/**
 * qentry_t->putstrf(): Add formatted string object into linked-list structure.
 *
 * @param   entry   qentry_t pointer
 * @param   replace in case of false, just insert. in case of true, remove all
 *                  same key then insert object if found.
 * @param   name    key name.
 * @param   format  formatted value string
 *
 * @return  true if successful, otherwise returns false.
 */
 #pragma warning(disable:4996)

static bool _putstrf(qentry_t *entry, bool replace, const char *name,
                     const char *format, ...)
{
    char *str;
    DYNAMIC_VSPRINTF(str, format);
    if (str == NULL) return false;

    bool ret = _putstr(entry, name, str, replace);
    free(str);

    return ret;
}

/**
 * qentry_t->putint(): Add integer object into linked-list structure.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name.
 * @param   num number value
 * @param   replace in case of false, just insert. in case of true, remove all
 *                  same key then insert object if found.
 *
 * @return  true if successful, otherwise returns false.
 */
static bool _putint(qentry_t *entry, const char *name, int num, bool replace)
{
    char str[20+1];
    if (snprintf(str, 20+1, "%d", num) >= 20+1) str[20] = '\0';
    return _put(entry, name, (void *)str, strlen(str) + 1, replace);
}

/**
 * qentry_t->get(): Find object with given name
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name
 * @param   size    if size is not NULL, object size will be stored.
 * @param   newmem  whether or not to allocate memory for the data.
 *
 * @return  a pointer of data if key is found, otherwise returns NULL.
 *
 * @code
 *   qentry_t *entry = qEntry();
 *   (...codes...)
 *
 *   // with newmem flag unset
 *   size_t size;
 *   const char *data = entry->get(entry, "key_name", &size, false);
 *
 *   // with newmem flag set
 *   size_t size;
 *   char *data = entry->get(entry, "key_name", &size, true);
 *   if(data != NULL) free(data);
 * @endcode
 *
 * @note
 * If newmem flag is set, returned data will be malloced and should be
 * deallocated by user. Otherwise returned pointer will point internal buffer
 * directly and should not be de-allocated by user. In thread-safe mode, newmem
 * flag always should be true.
 */
static void *_get(qentry_t *entry, const char *name, size_t *size, bool newmem)
{
    if (entry == NULL || name == NULL) return NULL;

    void *data = NULL;
    qentobj_t *obj;
    for (obj = entry->first; obj; obj = obj->next) {
        if (!strcmp(obj->name, name)) {
            if (size != NULL) *size = obj->size;

            if (newmem == true) {
                data = malloc(obj->size);
                memcpy(data, obj->data, obj->size);
            } else {
                data = obj->data;
            }

            break;
        }
    }

    return data;
}

/**
 * qentry_t->getlast(): Find lastest matched object with given name.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name
 * @param   size    if size is not NULL, object size will be stored.
 * @param   newmem  whether or not to allocate memory for the data.
 *
 * @return  a pointer of malloced data if key is found, otherwise returns NULL.
 *
 * @note
 * If you have multiple objects with same name. this method can be used to
 * find out lastest matched object.
 */
static void *_getlast(qentry_t *entry, const char *name, size_t *size,
                      bool newmem)
{
    if (entry == NULL || name == NULL) return NULL;

    qentobj_t *lastobj = NULL;
    qentobj_t *obj;
    for (obj = entry->first; obj; obj = obj->next) {
        if (!strcmp(name, obj->name)) lastobj = obj;
    }

    void *data = NULL;
    if (lastobj != NULL) {
        if (size != NULL) *size = lastobj->size;
        if (newmem == true) {
            data = malloc(lastobj->size);
            memcpy(data, lastobj->data, lastobj->size);
        } else {
            data = lastobj->data;
        }
    }

    return data;
}

/**
 * qentry_t->getstr():  Find string object with given name.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name
 * @param   newmem  whether or not to allocate memory for the data.
 *
 * @return  a pointer of malloced data if key is found, otherwise returns NULL.
 */
static char *_getstr(qentry_t *entry, const char *name, bool newmem)
{
    return (char *)_get(entry, name, NULL, newmem);
}

/**
 * qentry_t->_getstrf():  Find string object with given formatted name.
 *
 * @param   entry   qentry_t pointer
 * @param   newmem  whether or not to allocate memory for the data.
 * @param   namefmt formatted name string
 *
 * @return  a pointer of malloced data if key is found, otherwise returns NULL.
 */
static char *_getstrf(qentry_t *entry, bool newmem, const char *namefmt, ...)
{
    char *name;
    DYNAMIC_VSPRINTF(name, namefmt);
    if (name == NULL) return NULL;

    char *data = (char *)_get(entry, name, NULL, newmem);
    free(name);

    return data;
}

/**
 * qentry_t->getstrlast(): Find lastest matched string object with given name.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name
 * @param   newmem  whether or not to allocate memory for the data.
 *
 * @return  a pointer of malloced data if key is found, otherwise returns NULL.
 */
static char *_getstrlast(qentry_t *entry, const char *name, bool newmem)
{
    return (char *)_getlast(entry, name, NULL, newmem);
}

/**
 * qentry_t->getint(): Find integer object with given name.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name
 *
 * @return  a integer value of the integer object, otherwise returns 0.
 */
static int _getint(qentry_t *entry, const char *name)
{
    int n = 0;
    void *str = _get(entry, name, NULL, true);
    if (str != NULL) {
        n = atoi((char*)str);
        free(str);
    }
    return n;
}

/**
 * qentry_t->getintlast(): Find lastest matched integer object with given name.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name
 *
 * @return  a integer value of the object.
 */
static int _getintlast(qentry_t *entry, const char *name)
{
    void *str =_getlast(entry, name, NULL, true);
    int n = 0;
    if (str != NULL) {
        n = atoi((char*)str);
        free(str);
    }
    return n;

}

/**
 * qentry_t->caseget(): Find object with given name. (case-insensitive)
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name
 * @param   size    if size is not NULL, object size will be stored.
 * @param   newmem  whether or not to allocate memory for the data.
 *
 * @return  a pointer of malloced data if key is found, otherwise returns NULL.
 */
static void *_caseget(qentry_t *entry, const char *name, size_t *size,
                      bool newmem)
{
    if (entry == NULL || name == NULL) return NULL;

    void *data = NULL;
    qentobj_t *obj;
    for (obj = entry->first; obj; obj = obj->next) {
        if (!strcasecmp(name, obj->name)) {
            if (size != NULL) *size = obj->size;
            if (newmem == true) {
                data = malloc(obj->size);
                memcpy(data, obj->data, obj->size);
            } else {
                data = obj->data;
            }

            break;
        }
    }

    return data;
}

/**
 * qentry_t->casegetstr(): Find string object with given name in
 * case-insensitive way.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name
 * @param   newmem  whether or not to allocate memory for the data.
 *
 * @return  a pointer of malloced data if key is found, otherwise returns NULL.
 */
static char *_casegetstr(qentry_t *entry, const char *name, bool newmem)
{
    return (char *)_caseget(entry, name, NULL, newmem);
}

/**
 * qentry_t->casegetint(): Find integer object with given name in
 * case-insensitive way.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name
 *
 * @return  a integer value of the object.
 */
static int _casegetint(qentry_t *entry, const char *name)
{
    void *str =_caseget(entry, name, NULL, true);
    int n = 0;
    if (str != NULL) {
        n = atoi((char*) str);
        free(str);
    }
    return n;
}

/**
 * qentry_t->getnext(): Get next object structure.
 *
 * @param   entry   qentry_t pointer
 * @param   obj found data will be stored in this object
 * @param   name    key name, if key name is NULL search every key in the list.
 * @param   newmem  whether or not to allocate memory for the data.
 *
 * @return  true if found otherwise returns false
 *
 * @note
 * obj should be filled with 0 by using memset() before first call.
 * If newmem flag is true, user should de-allocate obj.name and obj.data
 * resources.
 *
 * @code
 *   qentry_t *entry = qEntry();
 *   entry->putstr(entry, "key1", "hello world 1", false);
 *   entry->putstr(entry, "key2", "hello world 2", false);
 *   entry->putstr(entry, "key3", "hello world 3", false);
 *
 *   qentobj_t obj;
 *   memset((void*)&obj, 0, sizeof(obj)); // must be cleared before call
 *   while(entry->getnext(entry, &obj, NULL, false) == true) {
 *     printf("NAME=%s, DATA=%s", SIZE=%zu", obj.name, obj.data, obj.size);
 *   }
 *
 *   // with newmem flag
 *   qentobj_t obj;
 *   memset((void*)&obj, 0, sizeof(obj)); // must be cleared before call
 *   while(entry->getnext(entry, &obj, NULL, true) == true) {
 *     printf("NAME=%s, DATA=%s", SIZE=%zu", obj.name, obj.data, obj.size);
 *     free(obj.name);
 *     free(obj.data);
 *   }
 * @endcode
 */
static bool _getnext(qentry_t *entry, qentobj_t *obj, const char *name,
                     bool newmem)
{
    if (entry == NULL || obj == NULL) return NULL;

    // if obj->name is NULL, it means this is first call.
    if (obj->name == NULL) obj->next = entry->first;

    qentobj_t *cont;
    bool ret = false;
    for (cont = obj->next; cont; cont = cont->next) {
        if (name != NULL && strcmp(cont->name, name)) continue;

        if (newmem == true) {
            obj->name = strdup(cont->name);
            obj->data = malloc(cont->size);
            memcpy(obj->data, cont->data, cont->size);
        } else {
            obj->name = cont->name;
            obj->data = cont->data;
        }
        obj->size = cont->size;
        obj->next = cont->next;

        ret = true;
        break;
    }

    return ret;
}

/**
 * qentry_t->size(): Get total number of stored objects
 *
 * @param   entry   qentry_t pointer
 *
 * @return  total number of stored objects.
 */
static int _size(qentry_t *entry)
{
    if (entry == NULL) return 0;

    return entry->num;
}

/**
 * qentry_t->remove(): Remove matched objects as given name.
 *
 * @param   entry   qentry_t pointer
 * @param   name    key name
 *
 * @return  a number of removed objects.
 */
static int _remove(qentry_t *entry, const char *name)
{
    if (entry == NULL || name == NULL) return 0;

    int removed = 0;
    qentobj_t *prev, *obj;
    for (prev = NULL, obj = entry->first; obj;) {
        if (!strcmp(obj->name, name)) { // found
            // copy next chain
            qentobj_t *next = obj->next;

            // adjust counter
            entry->num--;
            removed++;

            // remove entry itself
            free(obj->name);
            free(obj->data);
            free(obj);

            // adjust chain links
            if (next == NULL) entry->last = prev;  // if the object is last one
            if (prev == NULL) entry->first = next; // if the object is first one
            else prev->next = next;       // if the object is middle or last one

            // set next entry
            obj = next;
        } else {
            // remember prev object
            prev = obj;

            // set next entry
            obj = obj->next;
        }
    }

    return removed;
}

/**
 * qentry_t->truncate(): Truncate qentry_t
 *
 * @param   entry   qentry_t pointer
 *
 * @return  always returns true.
 */
static bool _truncate(qentry_t *entry)
{
    if (entry == NULL) return false;

    qentobj_t *obj;
    for (obj = entry->first; obj;) {
        qentobj_t *next = obj->next;
        free(obj->name);
        free(obj->data);
        free(obj);
        obj = next;
    }

    entry->num = 0;
    entry->first = NULL;
    entry->last = NULL;

    return true;
}

/**
 * qentry_t->reverse(): Reverse-sort internal stored object.
 *
 * @param   entry   qentry_t pointer
 *
 * @return  true if successful otherwise returns false.
 *
 * @note
 * This method can be used to improve look up performance.
 * if your application offen looking for last stored object.
 */
static bool _reverse(qentry_t *entry)
{
    if (entry == NULL) return false;

    qentobj_t *prev, *obj;
    for (prev = NULL, obj = entry->first; obj;) {
        qentobj_t *next = obj->next;
        obj->next = prev;
        prev = obj;
        obj = next;
    }

    entry->last = entry->first;
    entry->first = prev;

    return true;
}

/**
 * qentry_t->save(): Save qentry_t as plain text format
 *
 * @param   entry   qentry_t pointer
 * @param   filepath save file path
 *
 * @return  true if successful, otherwise returns false.
 */
static bool _save(qentry_t *entry, const char *filepath)
{
    if (entry == NULL) return false;

    FILE *fd;
    if ((fd = fopen(filepath, "w")) < 0) {
        DEBUG("qentry_t->save(): Can't open file %s", filepath);
        return false;
    }

    // change mode
    fchmod(fileno(fd), DEF_FILE_MODE);

    fprintf(fd, "# Generated by " _Q_PRGNAME ".\n");
    fprintf(fd, "# %s\n", filepath);

    qentobj_t *obj;
    for (obj = entry->first; obj; obj = obj->next) {
        char *encval = _q_urlencode(obj->data, obj->size);
        fprintf(fd, "%s=%s\n", obj->name, encval);
        free(encval);
    }

    fclose(fd);

    return true;
}

/**
 * qentry_t->load(): Load and append entries from given filepath
 *
 * @param   entry   qentry_t pointer
 * @param   filepath save file path
 *
 * @return  a number of loaded entries.
 */
static int _load(qentry_t *entry, const char *filepath)
{
    if (entry == NULL) return 0;

    FILE *fp = fopen(filepath, "r");
    if (fp == NULL) return 0;

    int cnt;
    for (cnt = 0; ; cnt++) {
        char *line = _q_fgetline(fp, MAX_LINEBUF);
        if (line == NULL) break;

        // parse & store
        char *data = line;
        char *name  = _q_makeword(data, '=');
        _q_strtrim(data);
        _q_strtrim(name);

        size_t size = _q_urldecode(data);
        _put(entry, name, data, size, false);
    }

    return cnt;
}

/**
 * qentry_t->print(): Print out stored objects for debugging purpose.
 *
 * @param   entry       qentry_t pointer
 * @param   out         output stream FILE descriptor such like stdout, stderr.
 * @param   print_data  true for printing out object value, false for disable
 *                      printing out object value.
 */
static bool _print(qentry_t *entry, FILE *out, bool print_data)
{
    if (entry == NULL || out == NULL) return false;

    qentobj_t *obj;
    for (obj = entry->first; obj; obj = obj->next) {
        fprintf(out, "%s=%s (%lu)\n", obj->name,
                (print_data?(char *)obj->data:"(data)"),
                (unsigned long)obj->size);
    }

    return true;
}

/**
 * qentry_t->free(): Free qentry_t
 *
 * @param   entry   qentry_t pointer
 *
 * @return  always returns true.
 */
static bool _free(qentry_t *entry)
{
    if (entry == NULL) return false;

    _truncate(entry);

    free(entry);
    return true;
}
