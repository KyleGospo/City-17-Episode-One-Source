/*
#ifndef TEST_APP_UTILS_H
#define TEST_APP_UTILS_H

#include "stdafx.h"

#define THROW_ON_ERROR(x) \
{ \
    smReturnCode result = (x); \
    if (result < 0) \
    { \
        std::stringstream s; \
        s << "API error code: " << result; \
        throw std::runtime_error(s.str()); \
    } \
}

extern float radToDeg(float rad);
extern float degToRad(float deg);
extern void STDCALL receiveLogMessage(void *, const char *buf, int buf_len);
extern bool processKeyPress(smEngine engine, HWND video_display);

#endif
 */

#ifndef TEST_APP_UTILS_H
#define TEST_APP_UTILS_H

#include "stdafx.h"

#define THROW_ON_ERROR(x) \
{ \
    smReturnCode result = (x); \
    if (result < 0) \
    { \
        std::stringstream s; \
        s << "API error code: " << result; \
        throw std::runtime_error(s.str()); \
    } \
}

extern float radToDeg(float rad);
extern float degToRad(float deg);
void STDCALL receiveLogMessage(void *, const char *buf, int /*buf_len*/);
bool processKeyPress(smEngineHandle engine, HWND video_display);

#endif
