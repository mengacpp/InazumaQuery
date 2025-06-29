#pragma once

#if defined(_WIN32)
#ifdef INAZUMA_EXPORTS
#define INAZUMA_API __declspec(dllexport)
#else
#define INAZUMA_API __declspec(dllimport)
#endif
#else
#define INAZUMA_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    INAZUMA_API void inazuma_hello();

#ifdef __cplusplus
}
#endif