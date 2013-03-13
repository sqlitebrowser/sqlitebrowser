ANTLR C++ Support Libraries Additional Notes

1.1 Using Microsoft Visual C++

Currently this is still (or again) somewhat experimental. MSVC is not the
development platform and I don't have access to the compiler currently.
YMMV

Make sure you compile the library *and* your project with the same
settings. (multithreaded/debug/etc.)

Visual C++ 6 only is supported for static builds. Some hacking and STLPort
is needed to build a DLL (only for experts).

Visual C++ 7.0 and 7.1 should support both static and DLL builds (DLL
builds might be broken). In general the main problem is getting the right
template instantiations into the DLL. For 7.0 you might have to tweak the
list in lib/cpp/src/dll.cpp. I'm told 7.1 does not need this.

For a static build (works probably best)

1. Create a win32 static library project.
2. Enable RTTI. (Run Time Type Information)
3. Add the source files from <installpath>/antlr/lib/cpp/src to the project
   (except dll.cpp) put <installpath>/antlr/lib/cpp in the search path for
   include files.

For the DLL build (MSVC 7.0 tested)

* Project settings ("create new project" dialogs)
      - Win32 project
        - Application Settings
          - Application type
            - DLL
          - Additional options
            - Export symbols
* Project properties (change defaults to)
      - Configuration Properties
        - C/C++
          - General
            - Additional Include Directories
              - drive:\antlr-2.7.2\lib\cpp
          - Preprocessor
            - Preprocessor Definitions
              - WIN32;_DEBUG;_WINDOWS;_USRDLL;ANTLR_EXPORTS
            - Code Generation
              - Runtime Library
                - Multi-threaded Debug DLL (/MDd)
              - Enable Function-Level Linking:
                - Yes
            - Language
              - Enable Run-Time Type Info
                - Yes
            - Precompiled Headers
              - Create/Use Precompiled Headers

NOTE: Do not use the antlr generated and support library in a multithreaded
way. It was not designed for a multithreaded environment.

1.3 Building with GCJ

NOTE: outdated the new Makefiles do not support this anymore.

It is also possible to build a native binary of ANTLR. This is somewhat
experimental and can be enabled by giving the --enable-gcj option to
configure. You need a recent GCC to do this and even then the constructed
binary crashes on some platforms.

2. Tested Compilers for this release

Don't get worried if your favourite compiler is not mentioned here. Any
somewhat recent ISO compliant C++ compiler should have little trouble with
the runtime library.

*NOTE* this section was not updated for the new configure script/Makefiles some of the things listed here to pass different flags to configure may not work anymore. Check INSTALL.txt or handedit generated scripts after configure.

2.1 Solaris

2.1.1 Sun Workshop 6.0

Identifies itself as:

   CC: Sun WorkShop 6 2000/08/30 C++ 5.1 Patch 109490-01

Compiles out of the box configure using:

   CXX=CC CC=cc AR=CC ARFLAGS="-xar -o" ./configure

Use CC to make the archive to ensure bundling of template instances. Check
manpage for details.

2.1.2 GCC

Tested 3.0.4, 3.2.1, 3.2.3, 3.3.2, 3.4.0.

All tested gcc are using a recent GNU binutils for linker and assembler.
You will probably run into trouble if you use the solaris
linker/assembler.

2.2 Windows

2.2.1 Visual C++

Visual C++ 6.0 reported to work well with static build. DLL build not
supported (reported to work when using STLPort in previous ANTLR versions).
I heart that in some cases there could be problems with precompiled headers
and the use of normal '/' in the #include directives (with service pack 5).

Visual C++ 7.0 reported to work, might need some tweaks for DLL builds due
to some shuffling around in the code.

Visual C++ 7.1 reported to work, might need some tweaks, see above.

My current guess is that DLL builds are all over the line broken. A
workaround is to make a DLL from the complete generated parser including
the static ANTLR support library.

2.2.2 Cygwin/MinGW

Not expecting any big problems maybe some tweaks needed in configure.

3. Old notes for a number of compilers

3.1 SGI Irix 6.5.10 MIPSPro compiler

You can't compile ANTLR with the MIPSPro compiler on anything < 6.5.10
because SGI just fixed a big bug dealing with namespaces in that release.

Note: To get it to compile do basically the following:

   CC=cc CXX=CC CXXFLAGS=-LANG:std ./configure --prefix=/usr/local/antlr

Note probably dates back to 2.7.0-2.7.1 era.

3.2 Sun CC 5

It may be you'll have to change one or two static_cast<char*>()'s to a
C-style cast. (think that's a compiler bug)

Configure using:

   CXX=CC CC=cc RANLIB="CC -xar" ./configure

The custom ranlib is needed to get the template instances into the archive.
Check manpages. Maybe the Sun CC 6 instructions above will work as well.

3.3 GCC on some platforms (Alpha Tru64)

The -pipe option not supported it seems. Configure using:

CFLAGS="-W -Wall" ./configure

Or remove the -pipe's from the generated scripts/Config.make.

4. IT DOESN'T WORK!?

4.1 Compile problems

The ANTLR code uses some relatively new features of C++ which not all
compilers support yet (such as namespaces, and new style standard headers).

At the moment, you may be able to work around the problem with a few nasty
tricks:

Try creating some header files like 'iostream' just containing:

#include <iostream.h>

and compile with an option to define away the word 'std', such as

CC .... -Dstd= ....

Also in the antlr subdirectory there's a file config.hpp. Tweak this one to
enable/disable the different bells and whistles used in the rest of the code.
Don't forget to submit those changes back to us (along with compiler info)
so we can incorporate them in our next release!

4.2 Reporting problems

When reporting problems please try to be as specific as possible e.g.
mention ANTLR release, and try to provide a clear and minimal example of
what goes wrong and what you expected.

Bug reports can be done to Terence or the current subsystem maintainers as
mentioned in the doc directory. Another option is to use the mailing list
linked from http://www.antlr.org.

Before reporting a problem you might want to try with a development
snapshot, there is a link to these in the File Sharing section of

http://www.antlr.org.
