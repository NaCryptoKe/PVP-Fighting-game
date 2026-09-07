/* stb_truetype.h - Minimal header for font rendering */
#ifndef STB_TRUETYPE_H
#define STB_TRUETYPE_H

#include <cstring>
#include <cmath>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float x0, y0, x1, y1;
    float s0, t0, s1, t1;
} stbtt_aligned_quad;

typedef struct {
    unsigned short x0, y0, x1, y1;
    float xadvance, xoff, yoff;
} stbtt_bakedchar;

int stbtt_BakeFontBitmap(const unsigned char *data, int offset, float pixel_height, 
    unsigned char *pixels, int pw, int ph, int first_char, int num_chars, stbtt_bakedchar *chardata);

void stbtt_GetBakedQuad(const stbtt_bakedchar *chardata, int pw, int ph, int char_index, 
    float *xpos, float *ypos, stbtt_aligned_quad *q, int opengl_fillrule);

#ifdef STB_TRUETYPE_IMPLEMENTATION

int stbtt_BakeFontBitmap(const unsigned char *data, int offset, float pixel_height,
    unsigned char *pixels, int pw, int ph, int first_char, int num_chars, stbtt_bakedchar *chardata) {
    std::memset(pixels, 0, pw * ph);
    std::memset(chardata, 0, sizeof(*chardata) * num_chars);
    return 1;
}

void stbtt_GetBakedQuad(const stbtt_bakedchar *chardata, int pw, int ph, int char_index,
    float *xpos, float *ypos, stbtt_aligned_quad *q, int opengl_fillrule) {
    if (!chardata || !xpos || !ypos || !q) return;
    q->x0 = *xpos; q->y0 = *ypos; q->x1 = *xpos + 10; q->y1 = *ypos + 10;
    q->s0 = 0; q->t0 = 0; q->s1 = 1; q->t1 = 1;
    *xpos += 10;
}

#else
/* Provide inline stub implementations for linking */
inline int stbtt_BakeFontBitmap(const unsigned char *, int, float,
    unsigned char *pixels, int pw, int ph, int, int num_chars, stbtt_bakedchar *chardata) {
    if (pixels) std::memset(pixels, 0, pw * ph);
    if (chardata) std::memset(chardata, 0, sizeof(*chardata) * num_chars);
    return 1;
}

inline void stbtt_GetBakedQuad(const stbtt_bakedchar *, int, int, int,
    float *xpos, float *ypos, stbtt_aligned_quad *q, int) {
    if (!xpos || !ypos || !q) return;
    q->x0 = *xpos; q->y0 = *ypos; q->x1 = *xpos + 10; q->y1 = *ypos + 10;
    q->s0 = 0; q->t0 = 0; q->s1 = 1; q->t1 = 1;
    *xpos += 10;
}

#endif

#ifdef __cplusplus
}
#endif

#endif
