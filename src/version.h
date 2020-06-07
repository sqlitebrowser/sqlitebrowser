#ifndef GEN_VERSION_H
#define GEN_VERSION_H
#define MAJOR_VERSION 3
#define MINOR_VERSION 11
#define PATCH_VERSION 300

#define str(s) #s
#define xstr(s) str(s)
#define APP_VERSION xstr(MAJOR_VERSION) "." xstr(MINOR_VERSION) "." xstr(PATCH_VERSION)

// If it is defined by the compiler, then it is a nightly build, and in the YYYYMMDD format.
#ifndef BUILD_VERSION
    #define BUILD_VERSION 0
#endif

#endif
