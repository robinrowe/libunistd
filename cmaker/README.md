# Cmaker README

Robin.Rowe@Cinepaint.org 2009/3/5

Cmaker is a set of programs that create a framework of cmake files. This saves time when creating a project and ensures consistency in cmake design. It also creates unit tests for use with ctest. 

## Usage

Add cmaker directory to your path or use with full path to call. Cmaker expects a LICENSE file from which it will read the license text to paste into the cmake files it creates.


	cd YourProjectDirectory
	echo "MIT Open Source" > LICENSE
	export AUTHOR="Your Name"
	cmaker_project.sh YourProjectName
	cmaker_program.sh YourExecutableName
	cmaker_class.sh YourClassName AnotherClassName ...
	mkdir build
	cd build
	cmake ..
	make
	ctest

To add another program or class to your project later, simply call the relevant script again. The purpose of cmaker_sources.sh is to import existing legacy files. Is not needed by new projects.
	
--0--