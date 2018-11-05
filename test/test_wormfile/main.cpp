//  WormFile test main
//  Robin Rowe 2017/3/21

#include <portable/WormFile.h>
#include <vector>
#include <iostream>
using namespace std;

int main()
{	portable::WormFile wormFile;
	const char* filename = "test.wrm";
	if(!wormFile.Open(filename))
	{	cout << "Can't open "<<filename<<endl;
		return 1;
	}
	const char* data = "Hello World";
	const size_t length = strlen(data)+1;
	unsigned offset = wormFile.Write(data, length);
	if(!offset)
	{	cout <<"Can't write "<<filename<<endl;
		return 2;
	}
	unsigned size = wormFile.Seek(offset);
	if(!size)
	{	cout <<"Invalid seek "<<filename<<endl;
		return 3;
	}
	if(size != length)
	{	cout << "Invalid blob size" << endl;
		return 4;
	}
	vector<char> buffer(length);
    if(!wormFile.Read(&buffer[0],length))
	{	cout << "Can't read "<< filename << endl;
		return 5;
	}
	if(strcmp(data,&buffer[0]))
	{	cout <<"Blob data mismatch" << endl;
		return 6;
	}
	cout << "SUCCESS!" << endl;
	return 0;
}
