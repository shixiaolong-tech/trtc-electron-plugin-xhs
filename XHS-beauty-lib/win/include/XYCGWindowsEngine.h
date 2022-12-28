#pragma once


#include<iostream>
#include<Windows.h>
#include<glad.h>

#define USE_CV_DEBUG
//#define DRAW_AI_DEBUG
//#define RSA_LICENSE
//#define USE_TIME_DEBUG
//#define WRITE_OUTPUT_TEXTURE

#ifdef APIEXPORT
#define DllExport __declspec(dllexport)
#else
#define DllExport __declspec(dllimport)
#endif

#include "xhs_mobile_common.h"
#include<vector>
//#include<XYBaseFilter.h>


class AiSystem;
namespace CG{
	static const int COLOR_UNKNOWN = 0;
	static const int COLOR_BGR = 2;              // [bgrbgrbgr]
	static const int COLOR_BGRA = 6;             // [bgrabgrabgra]
	static const int COLOR_NV21 = 8;             // NV21 [yyyyvuvu]
	static const int COLOR_NV12 = 9;             // NV12 [yyyyuvuv]
	static const int COLOR_RGBA = 11;            // [rgbargbargba]
	static const int COLOR_RGB = 3;           // [rgbrgbrgb]

class DllExport XYCGWindowsEngine {
public:
	XYCGWindowsEngine();
	~XYCGWindowsEngine();
	
	/// <summary>
	/// 初始化opengl windows环境以及渲染引擎
	/// </summary>
	/// <returns>返回0为成功，其他为失败</returns> 
	int initWindowsEngine(std::string license, std::string userId);
	
	/// <summary>
	/// 销毁opengl windows环境以及渲染引擎
	/// </summary>
	/// <returns>返回0为成功，其他为失败</returns>
	int destroyWindowsEngine();

	/// <summary>
	/// 加载AI模型，用于人脸检测
	/// </summary>
	/// <param name="path">模型路径</param>
	/// <returns>返回0为成功，其他为失败</returns>
	int loadAIModel(const char *path);
	
	/// <summary>
	/// 获取美颜、滤镜等处理后的数据(RGB格式)用于上屏
	/// </summary>
	/// <returns>返回值为buffer数据</returns>
	/// 
	void* getOutputData();

	/// <summary>
	/// 获取美颜、滤镜等处理后的数据(YUV-I420格式)用于上屏
	/// </summary>
	void getOutputYUVData(uint8_t* y, uint8_t* u, uint8_t* v);

	/// <summary>
	/// 每一帧的数据处理接口，包含人脸检测+渲染，在每一帧人脸检测后会自动调用渲染接口
	/// </summary>
	/// <param name="data">原始数据</param>
	/// <param name="pixelFormat">原始数据格式，详见COLOR_*</param>
	/// <param name="data_width">原始数据所对应的图像的宽</param>
	/// <param name="data_height">原始数据所对应的图像的高</param>
	/// <param name="stride">原始数据的stride</param>
	/// <param name="data_len">原始数据的字节数</param>
	void process(unsigned char *data, int pixelFormat, int data_width, int data_height, int stride, int data_len, float rotation);
	
	/// <summary>
	/// 处理yuv数据
	/// </summary>
	/// <param name="y"></param>
	/// <param name="u"></param>
	/// <param name="v"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void processYUV(uint8_t* y, uint8_t* u, uint8_t* v, int width, int height, float rotation);

	/// <summary>
	/// 设置美颜资源路径
	/// </summary>
	/// <param name="path">资源路径</param>
	/// <returns>0为成功，其他为失败</returns>
	int setBeautyResourcePath(const char * path);

	/// <summary>
	/// 设置滤镜资源路径
	/// </summary>
	/// <param name="path">资源路径</param>
	/// <returns>0为成功，其他为失败</returns>
	int setFilterResourcePath(const char * path);

	/// <summary>
	/// 是否开启一级美颜（包含磨皮，美白，瘦脸等）
	/// </summary>
	/// <param name="enable">true,开启；false，关闭；</param>
	void enableBeauty(bool enable);

	/// <summary>
	/// 是否开启二级美颜，磨皮or美白or瘦脸
	/// </summary>
	/// <param name="beautyType">可见定义FaceBeautyType</param>
	/// <param name="enable">true,开启;false，关闭;</param>
	void enableBeautyType(FaceBeautyType beautyType,bool enable);

	/// <summary>
	/// 是否开启滤镜
	/// </summary>
	/// <param name="enable">true,开启;false,关闭;</param>
	void enableFilter(bool enable);

	/// <summary>
	/// 是否开启颜色调整
	/// </summary>
	/// <param name="enable">true,开启;false,关闭;</param>
	void enableColorfulAdjustment(bool enable);

	/// <summary>
	/// 设置二级美颜类型所对应的强度
	/// </summary>
	/// <param name="beautyType">可见定义FaceBeautyType</param>
	/// <param name="intensity">美颜类别所对应的强度</param>
	void setBeautyTypeIntensity(FaceBeautyType beautyType, float intensity);

	/// <summary>
	/// 设置滤镜所对应的强度
	/// </summary>
	/// <param name="intensity">强度</param>
	void setFilterIntensity(float intensity);

	/// <summary>
	/// 设置颜色调整类型以及其所对应的强度
	/// </summary>
	/// <param name="colorfulType">颜色调整类型，详见@xhs_colorful_type</param>
	/// <param name="intensity">强度</param>
	void setColorfulTypeIntensity(xhs_colorful_type colorfulType, float intensity);

	/// <summary>
	/// 设置滤镜效果所应用的有效区间
	/// </summary>
	/// <param name="roi_point_x">置信区间起点x坐标，像素坐标</param>
	/// <param name="roi_point_y">置信区间起点y坐标，像素坐标</param>
	/// <param name="roi_width">置信区间宽度，像素宽度</param>
	/// <param name="roi_height">置信区间高度，像素高度</param>
	/// <param name="roi_color_flag">色彩效果是否在置信区域，只在置信区域1，在全图区域0</param>
	/// <param name="roi_picture_flag">边框效果是否在置信区域，只在置信区域1，在全图区域0</param>
	void setFilterIntervalParam(float roi_point_x, float roi_point_y, float roi_width, float roi_height, int roi_color_flag, int roi_picture_flag);

private:
	void render();
private:
	HGLRC m_renderContext;
	GLuint* m_inputTexture = nullptr;
	GLuint* m_tmpTexture1 = nullptr;
	GLuint* m_tmpTexture2 = nullptr;
	GLuint* m_TextureY = nullptr;
	GLuint* m_TextureU = nullptr;
	GLuint* m_TextureV = nullptr;

	GLuint* m_outputTexture = nullptr;
	GLuint* m_uploadPboId = nullptr;
	GLuint* m_downloadPboId = nullptr;
	GLuint* m_outputFboId = nullptr;
	int m_inputWidth = 0;
	int m_inputHeight = 0;
	int m_oldInputSize = 0;
	int m_frameIndex = 0;
	int m_inputPixelFormat;
	bool m_inputSizeDirty = false;
	bool m_ready = false;
	bool m_beautyEnable = false;
	bool m_filterEnable = false;
	bool m_colorfulEnable = false;
	void* m_inputData = nullptr;
	void* m_inputYUVData = nullptr;
	void* m_outputData = nullptr;
	xhs_handle_t m_filterEngineHandle = nullptr;
	XYFilterRoiParam m_roi = {0};
	bool m_licenseValid = false;

	AiSystem* m_AiSystem = nullptr;
	std::vector<float> m_points;
};
};