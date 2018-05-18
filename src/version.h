#ifndef GEN_VERSION_H
#define GEN_VERSION_H
#define MAJOR_VERSION 3
#define MINOR_VERSION 10
#define PATCH_VERSION 99

#define str(s) #s
#define xstr(s) str(s)

// It is required in 'winapp.rc' and always use the Major.Minor.Patch format.
#define PRODUCT_VERSION xstr(MAJOR_VERSION) "." xstr(MINOR_VERSION) "." xstr(PATCH_VERSION)

// If BUILD_VERSION is defined by the compiler, then it is a nightly build, and APP_VERSION
// will use the Major.Minor.Patch.Build format, otherwise it is Major.Minor.Patch only.
// If 'BUILD_VERSION == 0' then it is a stable verion.
#ifndef BUILD_VERSION
    #define BUILD_VERSION 0
    #define APP_VERSION PRODUCT_VERSION
#else
    #define APP_VERSION PRODUCT_VERSION "." xstr(BUILD_VERSION)
#endif

#endif
