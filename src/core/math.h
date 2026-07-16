#ifndef MATH_H
#define MATH_H

inline void makeOrtho(float* m, float aspect) {
    for (int i = 0; i < 16; ++i) m[i] = 0.0f;
    m[0]  = 1.0f / aspect;   // x scale
    m[5]  = 1.0f;            // y scale
    m[10] = -1.0f;           // z passthrough (GL clip z is flipped)
    m[15] = 1.0f;
}

#endif