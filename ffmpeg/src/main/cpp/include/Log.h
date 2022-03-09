//
// Created by kevin on 2022/2/25.
//

#ifndef ANDROIDFFMPEGPLAYER_LOG_H
#define ANDROIDFFMPEGPLAYER_LOG_H

#include <android/log.h>

#ifndef NDEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#if DEBUG

#ifdef __ANDROID__
#define TAG "kxffmpeg" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型
#else
#define LOGD(...) printf(...)
#define LOGI(...) printf(...)
#define LOGW(...) printf(...)
#define LOGE(...) printf(...)
#define LOGF(...) printf(...)
#endif

#else

#define LOGD(...)
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)
#define LOGF(...)

#endif

#endif //ANDROIDFFMPEGPLAYER_LOG_H

