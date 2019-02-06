// test_CLASS.cpp 
// Created by AUTHOR DATE
// License MIT LICENSE

#include <iostream>
#include "../CLASS.h"
using namespace std;

int main(int argc,char* argv[])
{	cout << "Testing CLASS" << endl;
	CLASS aCLASS;
	if(!aCLASS)
	{	cout << "CLASS failed, operator! == true" << endl;
		return 1;
	}
	cout << aCLASS << endl << "Done!" << endl;
	return 0;
}
