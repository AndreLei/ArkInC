BUILDING CURL LIBRARY
_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
WINDOWS
____________________________________________________________________________________________
1. UNPACK curl-7.55.1.zip
2. Open VS2015 x86 Native Tools Command Prompt
3. use cd command to move to folder ".../curl-7.55.1/winbuild"
4. run build command:

	->	nmake /f Makefile.vc mode=static MACHINE=x86

5. Include example can be found in ../CMakeLists.txt
____________________________________________________________________________________________

GMP SETUP

http://www.cs.nyu.edu/exact/core/gmp/