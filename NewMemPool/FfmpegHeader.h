#ifndef _FFMPEG_HEADER_H__
#define _FFMPEG_HEADER_H__

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libpostproc/postprocess.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"

#ifdef __cplusplus
};
#endif//__cplusplus

#ifdef _WIN32

#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "postproc.lib")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "swscale.lib")

#endif//_WIN32

#endif//_FFMPEG_HEADER_H__
