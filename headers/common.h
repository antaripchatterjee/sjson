#ifndef __COMMON_H__
#define __COMMON_H__

#if defined(_WIN32) && defined(BUILD_TYPE) && BUILD_TYPE == 3
#define DLLEXPORT __declspec(dllexport)
#elif defined(__GNUC__) && defined(BUILD_TYPE) && BUILD_TYPE == 3
#define DLLEXPORT __attribute__((visibility("default")))
#else
#define DLLEXPORT
#endif

#ifndef SJSON__ERROR_MSG_SIZE
#define SJSON__ERROR_MSG_SIZE 512
#endif // SJSON__ERROR_MSG_SIZE

#define SJSON_DECL_ERR_V(V) char V[SJSON__ERROR_MSG_SIZE+1] = { 0 }

#endif // __COMMON_H__
