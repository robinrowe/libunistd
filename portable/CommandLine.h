// CommandLine.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef CommandLine_h
#define CommandLine_h

#include <map>

class CommandLine
{	std::map<std::string,std::string> data;
public:
	void Read(int argc,const char** argv)
	{	for(int i=0;i<argc;i++)
		{	const char* cmd = argv[i];
			const char* eq = strchr(cmd,'=');
			if(eq)
			{	std::string key(cmd,eq-cmd);
				data[std::move(key)]=eq+1;
}
	}
	const char* Get(const char* key)
	{
	}
};

#endif

int main ()
{
  std::map<char,int> mymap;
  const auto it;

  mymap['a']=50;
  mymap['b']=100;
  mymap['c']=150;
  mymap['d']=200;

  it = mymap.find('b');
  if (it != mymap.end())
    mymap.erase (it);

  // print content:
  std::cout << "elements in mymap:" << '\n';
  std::cout << "a => " << mymap.find('a')->second << '\n';
  std::cout << "c => " << mymap.find('c')->second << '\n';
  std::cout << "d => " << mymap.find('d')->second << '\n';

  return 0;