#ifndef mem_h
#define mem_h

#ifdef __cplusplus
extern "C"
{
#endif

void* memmem(const void* haystack, size_t haystack_len, const void* needle, size_t needle_len)
{
    if (haystack == NULL) return NULL;
    if (haystack_len == 0) return NULL;
    if (needle == NULL) return NULL;
    if (needle_len == 0) return NULL;

    for (const char* h = haystack;
        haystack_len >= needle_len;
        ++h, --haystack_len) {
        if (!memcmp(h, needle, needle_len)) {
            return (void*)h;
        }
    }
    return NULL;
}

#ifdef __cplusplus
}
#endif

#endif