#include <dirent.h> 
#include <stdio.h> 

int main() 
{   const char* path ="./*";
	DIR* dir = opendir(path); 
	if(!dir) 
	{	printf("ERROR: opendir");
		return 1;
	} 
	struct dirent* entry = 0;
	while(entry = readdir(dir)) 
	{	if (!entry)
		{	printf("ERROR: readdir");
			return 2;
		}
		printf("Filename: %s\t\t Location in Directory Stream: %ld\n",
			entry->d_name,telldir(dir));
	}
	closedir(dir);
	return 0; 
}
