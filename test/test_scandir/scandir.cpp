#include <dirent.h> 
#include <stdio.h> 
#include <cstdlib>

int main() 
{   const char* path ="./*";
	struct dirent **namelist;
	int n = scandir(path, &namelist, NULL, alphasort);
	if (n < 0)
	{	perror("scandir");
		return 1;
	}
	for(int i=0;i<n;i++) 
	{	printf("%i: %s\n",i+1,namelist[i]->d_name);
		free(namelist[i]);
	}
	free(namelist);
	return 0;
}