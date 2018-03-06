# build-env-make
Contains the structure of a build environment for a library using make and google test.

It is to be used as a template when starting to develop a new library. Untar the file, and you should have the basic directories and Makefiles. Then, make a few changes to the make files and start coding and running AUTs.

Requirements:
1. C++ compiler and tools
2. make
3. Google Test

To use it:
1. untar build-env-make.tar.bz
2. rename the directory build-env-make to a name related to your library. This will be your root directory
3. cd to your root directory and update Makefile.common with the name of your library and the name of your root directory (just the directory name, not the path)
4. update your google test directory paths, as well as other paths, compiler, and flags that may be different in your environment.
5. run "make check" from your root directory and make sure everything builds and all tests pass
6. run "make distclean"
7. cd to src/ and replace the ".h" and ".C" files with yours
8. update the Makefile with your src file(s)
9. cd to test/ and replace the ".C" file with yours
10. update the Makefile with your src file(s)
11. once you have some code and tests, you can compile them and run them with make: "$ make check"

Targets:
1. make check   # compiles and executes all AUTs
2. make   # compiles the code in current directory and its children
