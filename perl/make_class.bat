@echo off
set pathName=C:\Users\rrowe\git\libunistd\perl
set className="%~1"
set make_class="%pathName%\make_class.pl"
perl %make_class% %className%
