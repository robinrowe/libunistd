// Shutdown.cpp
// Created by Robin Rowe 2019-07-10
// License Copyright 2019 Robin.Rowe@HeroicRobots.com ***Proprietary***

#include "Shutdown.h"
using namespace std;

Shutdown* Shutdown::handler;

ostream& Shutdown::Print(ostream& os) const
{	return os << "Shutdown";
} 
