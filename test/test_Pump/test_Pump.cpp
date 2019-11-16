// test_Pump.cpp 
// Created by Robin Rowe 2019-10-18
// License MIT open source

#include <iostream>
#include <portable/pump/Pump.h>
using namespace std;

int main(int argc,char* argv[])
{	cout << "Testing Pump" << endl;
	portable::Pump pump("TestPump");
	pump.StartJoin();
	cout << "Shutdown main" << endl;
	return 0;
}

/*
Testing Pump
Trace: Pump hooked SIGINT (portable::Pump::Pump@25)
SIGINT interrupt
Shutdown TestPump
Shutdown main
*/