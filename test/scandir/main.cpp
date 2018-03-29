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
		printf("%s\n",entry->d_name);
	}
	closedir(dir);
	struct dirent **namelist;
	int n = scandir(path, &namelist, NULL, alphasort);
	if (n < 0)
	{	perror("scandir");
		return 3;
	}
	for(int i=0;i<n;i++) 
	{	printf("%i: %s\n",i+1,namelist[i]->d_name);
		free(namelist[i]);
	}
	free(namelist);
	return 0; 
}
