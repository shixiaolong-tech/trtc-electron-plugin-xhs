#ifndef ELECTRON_PLUGINS_FACEUNITYPLUGIN_IVIDEOFRAMEPLUGIN_H_
#define ELECTRON_PLUGINS_FACEUNITYPLUGIN_IVIDEOFRAMEPLUGIN_H_

#include "ITRTCPlugin.h"
#pragma once
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

/**
* 视频数据结构类型
*/
enum TRTCVideoBufferType
{
	TRTCVideoBufferType_Unknown = 0,
	TRTCVideoBufferType_Buffer = 1,       ///< 二进制Buffer类型
	TRTCVideoBufferType_Texture = 3,      ///< 纹理类型
};

/**
* 视频帧的格式
*/
enum TRTCVideoPixelFormat
{
	TRTCVideoPixelFormat_Unknown = 0,
	TRTCVideoPixelFormat_I420 = 1,        ///< I420
	TRTCVideoPixelFormat_Texture_2D = 2,  ///< OpenGL 2D 纹理
	TRTCVideoPixelFormat_BGRA32 = 3,      ///< BGRA32
};

/**
* 视频画面旋转方向
*/
enum TRTCVideoRotation
{
	TRTCVideoRotation0 = 0,               ///< 顺时针旋转0度
	TRTCVideoRotation90 = 1,              ///< 顺时针旋转90度
	TRTCVideoRotation180 = 2,             ///< 顺时针旋转180度
	TRTCVideoRotation270 = 3,             ///< 顺时针旋转270度
};

/**
* 视频帧数据
*/
struct TRTCVideoFrame
{
	TRTCVideoPixelFormat videoFormat;       ///< 视频帧的格式
	TRTCVideoBufferType bufferType;         ///< 视频数据结构类型
	char * data;                            ///< 视频数据，字段 bufferType 是 LiteAVVideoBufferType_Buffer 时生效
	int textureId;                          ///< 视频纹理 ID，字段 bufferType 是 LiteAVVideoBufferType_Texture 时生效
	uint32_t length;                        ///< 视频数据的长度，单位是字节，对于 i420 而言， length = width  * height  * 3 / 2，对于BGRA32而言， length = width  * height  * 4
	uint32_t width;                         ///< 画面的宽度
	uint32_t height;                        ///< 画面的高度
	uint64_t timestamp;                     ///< 时间戳，单位 ms
	TRTCVideoRotation rotation;             ///< 画面旋转角度

	TRTCVideoFrame()
		: videoFormat(TRTCVideoPixelFormat_Unknown)
		, bufferType(TRTCVideoBufferType_Unknown)
		, data(nullptr)
		, textureId(-1)
		, length(0)
		, width(640)
		, height(360)
		, timestamp(0)
		, rotation(TRTCVideoRotation0)
	{

	}
};


class IVideoFramePlugin : public ITRTCPlugin {
public:
    virtual void onProcessVideoFrame(TRTCVideoFrame *srcFrame, TRTCVideoFrame *dstFrame) =0;
};

#endif  // ELECTRON_PLUGINS_FACEUNITYPLUGIN_IVIDEOFRAMEPLUGIN_H_
