// test_CLASS.cpp 
// Created by AUTHOR DATE
// License LICENSE

#include <iostream>
#include "../CLASS.h"
using namespace std;

int main(int argc,char* argv[])
{	cout << "Testing CLASS" << endl;
	CLASS OBJECT;
	if(!OBJECT)
	{	cout << "CLASS failed, operator! == true" << endl;
		return 1;
	}
	cout << OBJECT << endl << "Done!" << endl;
	return 0;
}
