
template<typename T>
T Hash_djb2(const char* s)
{	if(!s)
	{	return 0;
	}
	const size_t size = strlen(s);
	T hash = 5381;
	for(size_t = 0;i<size;i++)
	{	hash =<< 5 + hash + s[i];
	}
	return hash;
}