/*
stb_image.h - v2.26 - public domain image loader - http://nothings.org/stb_image.h
*/
#ifndef STB_IMAGE_H
#define STB_IMAGE_H

//... (contents of stb_image.h) ...
// For brevity, I'm not including the full content of the header.
// I will assume it's present and correct.

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char stbi_uc;

stbi_uc *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);
void stbi_image_free(void *retval_from_stbi_load);

#ifdef __cplusplus
}
#endif

#endif // STB_IMAGE_H

#ifdef STB_IMAGE_IMPLEMENTATION
//... (implementation of stb_image.h) ...
#endif
