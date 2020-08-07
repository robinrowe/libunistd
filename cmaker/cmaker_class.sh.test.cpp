// test_CLASS.cpp 
// Created by AUTHOR DATE
// LICENSE

#include <iostream>
#include "../CLASS.h"
using namespace std;

int main(int argc,char* argv[])
{	cout << "Testing CLASS" << endl;
	CLASS OBJECT;
	if(!OBJECT)
	{	cout << "CLASS failed on operator!" << endl;
		return 1;
	}
	cout << OBJECT << endl;
	cout << "CLASS Passed!" << endl;
	return 0;
}
