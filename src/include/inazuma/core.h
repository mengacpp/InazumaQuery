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

#define INA_OK 1
#define INA_ERROR 0

#define INA_DATA_PLAYER_IE3_DIR INA_DATA_DIR "/players/inazuma_eleven_3.csv"
#define INA_DATA_PLAYER_IE2_DIR INA_DATA_DIR "/players/inazuma_eleven_2.csv"
#define INA_DATA_PLAYER_IE_DIR INA_DATA_DIR "/players/inazuma_eleven.csv"

#define INA_NOT_IMPLEMENTED                                                    \
    fprintf(stderr, "ERROR: function %s not implemented yet!\n", __func__);

#endif // INAZUMA_CORE_H_