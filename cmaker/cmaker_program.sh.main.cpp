// EXECUTABLE.cpp 
// Created by AUTHOR DATE
// License LICENSE

#include <iostream>
using namespace std;

void Usage()
{	cout << "Usage: EXECUTABLE " << endl;
}

enum
{	ok,
	invalid_args

};

int main(int argc,char* argv[])
{	cout << "EXECUTABLE starting..." << endl;
	if(argc < 1)
	{	Usage();
		return invalid_args;
	}

	cout << "EXECUTABLE done!" << endl;
	return ok;
}
