# Cmaker README

Robin.Rowe@Cinepaint.org 2009/3/5

Cmaker is a set of programs that create a framework of cmake files. This saves time when creating a project and ensures consistency in cmake design. It also creates unit tests for use with ctest. 

A project created in Cmaker will build without writing ANY code. You may run all a project's programs and unit tests from the start if created by Cmaker. All written for you. Saves a lot of time. 

Works on Windows (with git bash), MacOS or Linux.

## Usage

Add cmaker directory to your path or use with full path to call. 

Cmaker expects to find a LICENSE file in your project directory from which it will read the first line as the license text to paste into the project files it creates. If you don't have a LICENSE file, you can create one like this:

	echo "MIT Open Source" > LICENSE

Creating a complete frameworks, including project, program, class files, and unit tests:

	cd YourProjectDirectory
	export AUTHOR="Your Name"
	cmaker_project.sh YourProjectName
	cmaker_program.sh YourExecutableName
	cmaker_class.sh YourClassName AnotherClassName ...
	mkdir build
	cd build
	cmake ..
	make
	ctest

To add another program or class to your project later, simply call the relevant script again. You only need to call cmaker_project.sh once. 

### Legacy Code

Use cmaker_sources.sh to import existing legacy files. Is not needed by new projects.

	cmaker_sources.sh

Use cmaker_top.sh to create the top level CMakelists.txt that calls CMakelists.txt files in subdirectories. Of course, create the subdirectories first.

	cmaker_top.sh project_name
	
Use cmaker_merge.sh to add cmaker frameworks to legacy projects. Note this appends the old files to the new, requires editing results by hand.

	cmaker_merge.sh old_dir

Use cmaker_to_cpp.sh to recursively rename .c files to .cpp files.

	cmaker_to_cpp.sh
	
--0--
