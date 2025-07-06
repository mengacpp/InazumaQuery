#ifndef INAZUMA_CORE_CORE_H_
#define INAZUMA_CORE_CORE_H_


/*
    Export / Import macro
*/
#if defined(_WIN32)
#ifdef INA_EXPORTS
#define INA_API __declspec(dllexport)
#else
#define INA_API __declspec(dllimport)
#endif
#else
#define INA_API
#endif

/*
    Thread local macro
*/
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define INA_THREAD_LOCAL _Thread_local
#elif defined(__GNUC__) || defined(__clang__)
#define INA_THREAD_LOCAL __thread
#else
#error "Thread-local storage unsupported"
#endif

/*
    Extern "C" Macro
*/
#ifdef __cplusplus
#define INA_BEGIN_EXTERN_C                                                     \
    extern "C"                                                                 \
    {
#define INA_END_EXTERN_C }
#else
#define INA_BEGIN_EXTERN_C
#define INA_END_EXTERN_C
#endif

/*
    Util function
*/
#define INA_NOT_IMPLEMENTED                                                    \
    fprintf(stderr, "ERROR: function %s not implemented yet!\n", __func__);

#endif // INAZUMA_CORE_CORE_H_