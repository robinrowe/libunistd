
template <typename T>
class SharedMemory
{	int fd;
	T* t;
	T err;
	size_t size;
public:
	~SharedMemory()
	{	Close();
	}
	SharedMemory(const char* name,size_t size)
	:	fd(0)
	,	p(0)
	,	size(0)
	{	int oflags = O_RDWR | O_CREAT;
		fd = shm_open(name,oflags,0644);
		if(fd)
		{	shm_ftruncate(fd,size);
			p = (t*) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	}	}
	SharedMemory(const char* name)
	:	fd(0)
	,	p(0)
	,	size(0)
	{	int oflags=O_RDWR;
		fd = shm_open(name,oflags,0644);
		if(fd)
		{	size = shm_size(fd)/sizeof(t);
			p = (t*) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	}	}
	T& operator[](size_t i)
	{	if(i>=size)
		{	return err;
		}
		return p[i];
	}
	operator t*() 
	{	return p;
	}
	bool Flush()
	{	return shm_flush(fd);
	}
	bool Close()
	{	return shm_close(fd);
	}
	size_t Size() const
	{	return size;
	}
};
