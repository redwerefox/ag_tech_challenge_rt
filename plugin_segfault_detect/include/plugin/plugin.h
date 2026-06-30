#pragma once

#ifdef _WIN32
#ifdef PLUGIN_BUILDING
#define DETECTOR_API __declspec(dllexport)
#else
#define DETECTOR_API __declspec(dllimport)
#endif
#else
#define DETECTOR_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

DETECTOR_API void detector_init();
DETECTOR_API void detector_shutdown();
}
