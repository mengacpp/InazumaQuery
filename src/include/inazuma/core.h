#ifndef INAZUMA_CORE_H_
#define INAZUMA_CORE_H_

#if defined(_WIN32)
#ifdef INA_EXPORTS
#define INA_API __declspec(dllexport)
#else
#define INA_API __declspec(dllimport)
#endif
#else
#define INA_API
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define INA_THREAD_LOCAL _Thread_local
#elif defined(__GNUC__) || defined(__clang__)
#define INA_THREAD_LOCAL __thread
#else
#error "Thread-local storage unsupported"
#endif

#define INA_SUCCESS 1
#define INA_FAILURE 0

#ifdef __cplusplus
#define BEGIN_EXTERN_C                                                         \
    extern "C"                                                                 \
    {
#define END_EXTERN_C }
#else
#define BEGIN_EXTERN_C
#define END_EXTERN_C
#endif

#define INA_DATA_PLAYER_IE3_DIR INA_DATA_DIR "/players/inazuma_eleven_3.csv"
#define INA_DATA_PLAYER_IE2_DIR INA_DATA_DIR "/players/inazuma_eleven_2.csv"
#define INA_DATA_PLAYER_IE_DIR INA_DATA_DIR "/players/inazuma_eleven.csv"

#define INA_NOT_IMPLEMENTED                                                    \
    fprintf(stderr, "ERROR: function %s not implemented yet!\n", __func__);

#endif // INAZUMA_CORE_H_