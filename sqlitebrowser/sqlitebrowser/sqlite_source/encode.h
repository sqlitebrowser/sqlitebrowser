#ifndef _ENCODE_H_
#define _ENCODE_H_

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

int sqlite_encode_binary(const unsigned char *in, int n, unsigned char *out);
int sqlite_decode_binary(const unsigned char *in, unsigned char *out);

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif /* _ENCODE_H_ */
