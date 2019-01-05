// print_malloc.h

#ifndef print_malloc_h
#define print_malloc_h

#include <stdio.h>
#include <stdlib.h> 
#include <malloc.h>

inline
void debug_print(const char* op,void* p,const char* filename,int lineno,size_t size)
{	
#ifdef _WIN32
	char sep = '\\';
#else
	char sep = '/';
#endif
	const char* ptr = strrchr(filename,sep);
	if(sep)
	{	filename = ptr+1;
	}
	printf("%s: %p %s:%i %zu bytes\n",op,p,filename,lineno,size);
}

inline
void *debug_malloc(size_t size,const char* filename,int line)
{	void* p = malloc(size);
	debug_print("+malloc",p,filename,line,size);
	return p;
}

inline
void debug_free(void *p,const char* filename,int line)
{	debug_print("-free",p,filename,line,0);
	free(p);
}

inline
void *debug_calloc(size_t nmemb, size_t size,const char* filename,int line)
{	void* p = calloc(nmemb,size);
	debug_print("+calloc",p,filename,line,nmemb*size);
	return p;
}

inline
void *debug_realloc(void *ptr, size_t size,const char* filename,int line)
{	debug_print("-free",ptr,filename,line,0);
	void* p = realloc(ptr,size);
	debug_print("+realloc",p,filename,line,size);
	return p;
}

#define malloc(size) debug_malloc(size,__FILE__,__LINE__)
#define calloc(n,size) debug_calloc(n,size,__FILE__,__LINE__)
#define realloc(p,size) debug_realloc(p,size,__FILE__,__LINE__)
#define free(p) debug_free(p,__FILE__,__LINE__)

/*
void SetDebugHeap()
{// Get current flag  
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );  
	// Turn on leak-checking bit.  
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;  
	tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	tmpFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
	tmpFlag |= _CRTDBG_ALLOC_MEM_DF;
	// Turn off CRT block checking bit.  
//	tmpFlag &= ~_CRTDBG_CHECK_CRT_DF;  
	// Set flag to the new value.  
	_CrtSetDbgFlag( tmpFlag );  
_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
}
*/
#endif