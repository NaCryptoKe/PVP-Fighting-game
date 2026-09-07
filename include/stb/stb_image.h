/* stb_image.h - Minimal header for image loading */
#ifndef STB_IMAGE_H
#define STB_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned char *stbi_load(const char *filename, int *x, int *y, int *comp, int req_comp);
void stbi_image_free(void *retval_from_stbi_load);
void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip);

#ifdef STB_IMAGE_IMPLEMENTATION
#include <cstring>
#include <cstdlib>

static int stbi_flip_vertically = 0;

unsigned char *stbi_load(const char *filename, int *x, int *y, int *comp, int req_comp) {
    if (x) *x = 0;
    if (y) *y = 0;
    if (comp) *comp = 4;
    unsigned char *data = (unsigned char *)malloc(1);
    if (data) *data = 0;
    return data;
}

void stbi_image_free(void *retval_from_stbi_load) {
    if (retval_from_stbi_load) free(retval_from_stbi_load);
}

void stbi_set_flip_vertically_on_load(int flag_true_if_should_flip) {
    stbi_flip_vertically = flag_true_if_should_flip;
}

#endif

#ifdef __cplusplus
}
#endif

#endif
