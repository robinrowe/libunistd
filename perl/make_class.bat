@echo off
set pathName=C:\Users\rrowe\git\libunistd\perl
set className="%~1"
set make_class="%pathName%\make_class.pl"
set email="Robin.Rowe@CinePaint.org"
set license="License open source MIT"
perl %make_class% %className%
