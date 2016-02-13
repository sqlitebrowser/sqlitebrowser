#ifndef GEN_VERSION_H
#define GEN_VERSION_H
#define MAJOR_VERSION 3
#define MINOR_VERSION 8
#define PATCH_VERSION 99

#define str(s) #s
#define xstr(s) str(s)
#define APP_VERSION xstr(MAJOR_VERSION) "." xstr(MINOR_VERSION) "." xstr(PATCH_VERSION)
#endif
