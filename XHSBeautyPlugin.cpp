#include "XHSBeautyPlugin.h"
#include <stdlib.h>
#ifdef _WIN32
#pragma comment(lib, "xhsGraphicsWin.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#else
#include <dlfcn.h>
#include <unistd.h>
#endif
#include <iostream>
#include <string>

static FILE *g_pf = nullptr;
bool g_debug = false;

void log(const char* fmt, ...){
	if (!g_debug || NULL == fmt)
		return;

	va_list args;
	va_start(args, fmt);
	char szBuffer[1024];
	vsprintf(szBuffer, fmt, args);
	va_end(args);
#ifdef _WIN32
	OutputDebugStringA(szBuffer);
#endif
	if (g_pf)
	{
		fwrite(szBuffer, strlen(szBuffer), 1, g_pf);
		fflush(g_pf);
	} 
}

void enableDebug(bool enable, const char* logFileName) {
	g_debug = enable;
    if (g_pf) {
        fclose(g_pf);
        g_pf = nullptr;
    }
    if (g_debug) {
        if (g_pf) {
            fclose(g_pf);
            g_pf = nullptr;
        }
        try
        {
            g_pf = fopen(logFileName, "w");
        }
        catch(...)
        {
        } 
#ifdef _WIN32
		if (g_pf == NULL)
		{
			int err = GetLastError();
			log("file open error,err=%d", err);
		}
#endif  
    }
}

XHSBeautyPlugin::XHSBeautyPlugin() {
  enableDebug(true, "xhs_debug.log");
  m_pBeautyEngine = new CG::XYCGWindowsEngine();
  if (!m_pBeautyEngine) {
    log("[E]new CG::XYCGWindowsEngine() error\n");
  }
  log("XHSBeautyPlugin create\n");
}

XHSBeautyPlugin::~XHSBeautyPlugin() {
  uninit();
  if (m_pBeautyEngine) {
    m_pBeautyEngine->destroyWindowsEngine();
    m_pBeautyEngine = nullptr;
  }
  sLicense.clear();
  sUserId.clear();
  isBeautyEngineInited = false;
}

bool XHSBeautyPlugin::init() {
  std::lock_guard<std::mutex> mutx(mMutex);
  isGlInit = false;
  return true;
}

bool XHSBeautyPlugin::uninit() {
  std::lock_guard<std::mutex> mutx(mMutex);
  if (isGlInit) {
    uninitOpenGL();
  }
  isGlInit = false;
  return true;
}

bool XHSBeautyPlugin::load(const char* path) {
  isLoaded = true;
  return true;
}

bool XHSBeautyPlugin::unload() {
  isLoaded = false;
  return true;
}

bool XHSBeautyPlugin::enable(bool enabled) {
  isEnabled = enabled;
  return true;
}

void XHSBeautyPlugin::handleAiModePathParam(const std::string path) {
  int is_ok = m_pBeautyEngine->loadAIModel(path.c_str());
  if (is_ok != 0) {
    log("[E]handleAiModePathParam:loadAIModel error:%i\n", is_ok);
  } else {
    log("handleAiModePathParam:loadAIModel success %s \n", path.c_str());
  }
}

void XHSBeautyPlugin::handleBeautyParam(Value& beauty) {
  if (beauty.HasMember("resourcePath")) {
    Value& v_resourcePath = beauty["resourcePath"];
    if (v_resourcePath.IsString()) {
      std::string resourcePath = v_resourcePath.GetString();
      m_pBeautyEngine->setBeautyResourcePath(resourcePath.c_str());
      log("handleBeautyParam: 'resourcePath' is: %s\n", resourcePath.c_str());
    } else {
      log("[E]handleBeautyParam: 'resourcePath' type error\n");
    }
  }

  if (beauty.HasMember("enable")) {
    Value& v_enable = beauty["enable"];
    if (v_enable.IsBool()) {
      bool enable = v_enable.GetBool();
      m_pBeautyEngine->enableBeauty(enable);
    } else {
      log("[E]handleBeautyParam: 'enable' type error\n");
    }
  }

  if (beauty.HasMember("types")) {
    Value& v_types = beauty["types"];
    if (v_types.IsArray()) {
      int length = v_types.Capacity();
      for(int i = 0; i < length; i++) {
        Value& v_type = v_types[i];

        if (v_type.IsObject()) {
          if (v_type.HasMember("type")) {
            Value& v_beauty_type = v_type["type"];

            if (v_beauty_type.IsInt()) {
              if (v_type.HasMember("enable")) {
                Value& v_type_enable = v_type["enable"];
                if (v_type_enable.IsBool()) {
                  m_pBeautyEngine->enableBeautyType((FaceBeautyType)v_beauty_type.GetInt(), v_type_enable.GetBool());
                } else {
                  log("[E]handleBeautyParam: 'types[%d].enable' type error\n", i);
                }
              }

              if (v_type.HasMember("intensity")) {
                Value& v_intensity = v_type["intensity"];
                if (v_intensity.IsNumber()) {
                  const float intensity = v_intensity.GetFloat();
                  m_pBeautyEngine->setBeautyTypeIntensity((FaceBeautyType)v_beauty_type.GetInt(), intensity);
                } else {
                  log("[E]handleBeautyParam: 'types[%d].intensity' type error\n", i);
                }
              }
            } else {
              log("[E]handleBeautyParam: 'types[%d].type' type error\n", i);
            }
          } else {
            log("[E]handleBeautyParam: 'types[%d].type' not exist and be skipped\n", i);
          }
        } else {
          log("[E]handleBeautyParam: 'types[%d]' type error\n", i);
        }
      }
    } else {
      log("[E]handleBeautyParam: 'types' type error\n");
    }
  }
}

void XHSBeautyPlugin::handleFilterParam(Value& filter) {
  if (filter.HasMember("resourcePath")) {
    Value& v_resourcePath = filter["resourcePath"];

    if (v_resourcePath.IsString()) {
      std::string resourcePath = v_resourcePath.GetString();
      log("handleFilterParam: 'resourcePath' is %s\n", resourcePath.c_str());
      filterResourceBasePath = resourcePath;
    } else {
      log("[E]handleFilterParam: 'resourcePath' type error\n");
    }
  }

  if (filter.HasMember("enable")) {
    Value& v_enable = filter["enable"];
    if (v_enable.IsBool()) {
      m_pBeautyEngine->enableFilter(v_enable.GetBool());
    } else {
      log("[E]handleFilterParam: 'enable' type error\n");
    }
  }

  if (filter.HasMember("type")) {
    Value& v_type = filter["type"];
    if (v_type.IsString()) {
      std::string type = v_type.GetString();
      std::string fullPath = filterResourceBasePath + "\\" + type;
      log("handleFilterParam: full filter resource path is %s\n", fullPath.c_str());
      m_pBeautyEngine->setFilterResourcePath(fullPath.c_str());
    } else {
      log("[E]handleFilterParam: 'type' type error\n");
    }
  }

  if (filter.HasMember("intensity")) {
    Value& v_intensity = filter["intensity"];
    if (v_intensity.IsNumber()) {
      m_pBeautyEngine->setFilterIntensity(v_intensity.GetFloat());
    } else {
      log("[E]handleFilterParam: 'intensity' type error\n");
    }
  }

  if (filter.HasMember("interval")) {
    Value& v_interval = filter["interval"];
    if (v_interval.IsObject()) {
      Value& v_x = v_interval["x"], &v_y = v_interval["y"];
      Value& v_width = v_interval["width"], &v_height = v_interval["height"];
      Value& v_colorFlag = v_interval["colorFlag"], &v_pictureFlag = v_interval["pictureFlag"];
      if (v_interval.HasMember("x")) {
        v_x = v_interval["x"];
        if (!v_x.IsNumber()) {
          v_x.SetFloat(0);
        } 
      }

      if (v_interval.HasMember("y")) {
        v_y = v_interval["y"];
        if (!v_y.IsNumber()) {
          v_y.SetFloat(0);
        }
      }

      if (v_interval.HasMember("width")) {
        v_width = v_interval["width"];
        if (!v_width.IsNumber()) {
          v_width.SetFloat(0);
        }
      }

      if (v_interval.HasMember("height")) {
        v_height = v_interval["height"];
        if (!v_height.IsNumber()) {
          v_height.SetFloat(0);
        }
      }

      if (v_interval.HasMember("colroFlag")) {
        v_colorFlag = v_interval["colorFlag"];
        if (!v_colorFlag.IsInt()) {
          v_colorFlag.SetInt(0);
        }
      }

      if (v_interval.HasMember("pictureFlag")) {
        v_pictureFlag = v_interval["picture"];
        if (!v_pictureFlag.IsInt()) {
          v_pictureFlag.SetInt(0);
        }
      }

      if (!v_x.IsNull() && !v_y.IsNull() && !v_width.IsNull() && !v_height.IsNull() && !v_colorFlag.IsNull() && !v_pictureFlag.IsNull()) {
        m_pBeautyEngine->setFilterIntervalParam(
          v_x.GetFloat(), v_y.GetFloat(), v_width.GetFloat(), v_height.GetFloat(), v_colorFlag.GetInt(), v_pictureFlag.GetInt()
        );
      }
    } else {
      log("[E]handleFilterParam: 'interval' type error\n");
    }
  }
}

void XHSBeautyPlugin::handleColorfulParam(Value& colorful) {
  if (colorful.HasMember("enable")) {
    Value& v_enable = colorful["enable"];
    if (v_enable.IsBool()) {
      m_pBeautyEngine->enableColorfulAdjustment(v_enable.GetBool());
    } else {
      log("[E]handleColorfulParam: 'enable' type error\n");
    }
  }

  if (colorful.HasMember("types")) {
    Value& v_types = colorful["types"];
    if (v_types.IsArray()) {
      int length = v_types.Capacity();
      for(int i = 0; i < length; i++) {
        Value& v_type = v_types[i];

        if (v_type.IsObject()) {
          if (v_type.HasMember("type") && v_type.HasMember("intensity")) {
            Value& v_colorfulType = v_type["type"];
            Value& v_intensity = v_type["intensity"];

            if (v_colorfulType.IsInt() && v_intensity.IsNumber()) {
              m_pBeautyEngine->setColorfulTypeIntensity((xhs_colorful_type)v_colorfulType.GetInt(), v_intensity.GetFloat());
            } else {
              if (!v_colorfulType.IsInt()) {
                log("[E]handleColorfulParam: 'types[%d].type' type error\n", i);
              }
              if (!v_intensity.IsNumber()) {
                log("[E]handleColorfulParam: 'types[%d].intensity' type error\n", i);
              }
            }
          }
        } else {
          log("[E]handleColorfulParam: 'types[%d]' type error\n");
        }
      }
    } else {
      log("[E]handleColorfulParam: 'types' type error\n");
    }
  }
}

bool XHSBeautyPlugin::setParameter(const char* param) {
  log("setParameter: %s\n", param);
  std::lock_guard<std::mutex> mutx(mMutex);
  Document d;
  d.Parse(param);

  if (d.HasParseError()) {
    return false;
  }

  if (d.HasMember("license") && d.HasMember("userId")) {
    Value& v_license = d["license"];
    Value& v_userId = d["userId"];
    if (v_license.IsString() && v_userId.IsString()) {
      sLicense = std::string(v_license.GetString(), v_license.GetStringLength());
      sUserId = std::string(v_userId.GetString(), v_userId.GetStringLength());
    } else {
      log("[E]setParameter: 'license' or 'userId' type error\n");
      return false;
    }

    int is_ok = m_pBeautyEngine->initWindowsEngine(sLicense, sUserId);
    if (is_ok != 0) {
      log("[E]initWindowsEngine error:%i\n", is_ok);
      return false;
    }
    isBeautyEngineInited = true;
    log("initWindowsEngine success\n");
  }
  if(!isBeautyEngineInited) {
    return false;
  }

  if (d.HasMember("aiModelPath")) {
    Value& v_aiModelPath = d["aiModelPath"];
    if (v_aiModelPath.IsString()) {
      handleAiModePathParam(v_aiModelPath.GetString());
    } else {
      log("[E]setParameter: 'aiModelPath' type error\n");
    }
  }

  if (d.HasMember("beauty")) {
    Value& v_beauty = d["beauty"];
    if (v_beauty.IsObject()) {
      handleBeautyParam(v_beauty);
    } else {
      log("[E]setParameter: 'beauty' type error\n");
    }
  }

  if (d.HasMember("filter")) {
    Value& v_filter = d["filter"];
    if (v_filter.IsObject()) {
      handleFilterParam(v_filter);
    } else {
      log("[E]setParameter: 'filter' type error\n");
    }
  }

  if (d.HasMember("colorful")) {
    Value& v_colorful = d["colorful"];
    if (v_colorful.IsObject()) {
      handleColorfulParam(v_colorful);
    } else {
      log("[E]setParameter: 'colorful' type error\n");
    }
  }

  return true;
}

bool XHSBeautyPlugin::initOpenGL() {
#ifdef _WIN32
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1u,
        PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW,
        PFD_TYPE_RGBA,
        32u,
        0u, 0u, 0u, 0u, 0u, 0u,
        8u,
        0u,
        0u,
        0u, 0u, 0u, 0u,
        24u,
        8u,
        0u,
        PFD_MAIN_PLANE,
        0u,
        0u, 0u };
    mHWin = CreateWindowExA(
                            0, "EDIT", "", ES_READONLY,
                            0, 0, 1, 1,
                            NULL, NULL,
                            GetModuleHandleA(NULL), NULL);
    if (!mHWin) {
        return false;
    }
    mHDc = GetDC(mHWin);
    if (!mHDc) {
        return false;
    }
    int spf = ChoosePixelFormat(mHDc, &pfd);
    int ret = SetPixelFormat(mHDc, spf, &pfd);
    mHRc = wglCreateContext(mHDc);
    if (!mHRc) {
        return false;
    }
    wglMakeCurrent(mHDc, mHRc);
#else
    CGLPixelFormatAttribute attrib[13] = { kCGLPFAOpenGLProfile,
        (CGLPixelFormatAttribute)kCGLOGLPVersion_Legacy,
        kCGLPFAAccelerated,
        kCGLPFAColorSize, (CGLPixelFormatAttribute)24,
        kCGLPFAAlphaSize, (CGLPixelFormatAttribute)8,
        kCGLPFADoubleBuffer,
        kCGLPFASampleBuffers, (CGLPixelFormatAttribute)1,
        kCGLPFASamples, (CGLPixelFormatAttribute)4,
        (CGLPixelFormatAttribute)0 };
    CGLPixelFormatObj pixelFormat = NULL;
    GLint numPixelFormats = 0;
    CGLChoosePixelFormat(attrib, &pixelFormat, &numPixelFormats);
    CGLError err = CGLCreateContext(pixelFormat, NULL, &mCglContext);
    if (err) {
        return false;
    }
    CGLSetCurrentContext(mCglContext);
    CGLReleasePixelFormat(pixelFormat);
#endif

    log("OpenGL init\n");
    return (bool)gladLoadGL();
}

bool XHSBeautyPlugin::uninitOpenGL() {
#ifdef _WIN32
    if (mHRc) {
        wglDeleteContext(mHRc);
        mHRc = NULL;
    }
    if (mHWin && mHDc) {
        ReleaseDC(mHWin, mHDc);
        mHDc = NULL;
    }
    if (mHWin) {
        DestroyWindow(mHWin);
    }
#else
    CGLSetCurrentContext(NULL);
    if (mCglContext) {
        CGLReleaseContext(mCglContext);
    }
#endif

    log("OpenGL uninit\n");
    return true;
}

class GLContextGuard {
 public:
  GLContextGuard(){
#ifdef _WIN32
    mHRc = ::wglGetCurrentContext();
    mHDc = ::wglGetCurrentDC();

#endif  // _WIN32

  }
  ~GLContextGuard() {
#ifdef _WIN32
    wglMakeCurrent(mHDc, mHRc);
#endif  // _WIN32
  }
 private:
#ifdef _WIN32
  HDC mHDc = NULL;
  HGLRC mHRc = NULL;
#endif  //
};

void XHSBeautyPlugin::onProcessVideoFrame(TRTCVideoFrame *srcFrame, TRTCVideoFrame *dstFrame){
  log("onProcessVideoFrame begin\n");
  if (!srcFrame || !srcFrame->data) {
    return;
  }
  log("frame src: length:%lu width:%lu height:%lu timestamp:%llu videoFormat:%lu pdata:%p\n",
    srcFrame->length, srcFrame->width, srcFrame->height, srcFrame->timestamp, (uint32_t)srcFrame->videoFormat, srcFrame->data);
  log("frame dst: length:%lu width:%lu height:%lu timestamp:%llu videoFormat:%lu pdata:%p\n",
    dstFrame->length, dstFrame->width, dstFrame->height, dstFrame->timestamp, (uint32_t)dstFrame->videoFormat, dstFrame->data);

  // 小红书的美颜库在未开启美颜和滤镜时，会直接 return，此时如果开启美颜插件，dstFrame->data 会是全零数据或者上一次开启时残留的最后一帧数据。
  // 这里复制一次源数据，可以保证这个场景下，渲染出正常的视频，否则会出现绿屏帧或者上次美颜残留数据。
  memcpy(dstFrame->data, srcFrame->data, srcFrame->length); 
  if (sLicense.empty() || sUserId.empty()) {
    // 未设置认证信息，跳过美颜接口调用
    return;
  }

  std::lock_guard<std::mutex> mutx(mMutex);
  GLContextGuard gl_guard;
  do {
    if (!isGlInit) {
      initOpenGL();
      isGlInit = true;
    } else {
#ifdef _WIN32
      if (mHDc && mHRc) {
        wglMakeCurrent(mHDc, mHRc);
      }
#else
      if (mCglContext) {
        CGLError err = CGLSetCurrentContext(mCglContext);
        if (err != kCGLNoError) {
          log("[E]onProcessVideoFrame CGLSetCurrentContext err=%d\n", err);
        }
      }
#endif
      else {
        log("[E]onProcessVideoFrame set gl context error!\n");
        return;
      }
    }

    // pre transcoding
    uint32_t y_length = srcFrame->width * srcFrame->height;
    uint32_t u_v_length = y_length / 4;
    uint8_t *y = (uint8_t*)srcFrame->data;
    uint8_t *u = (uint8_t*)(srcFrame->data + y_length);
    uint8_t *v = (uint8_t*)(srcFrame->data + y_length + u_v_length); 

    uint8_t* y_processed = (uint8_t*)dstFrame->data;
    uint8_t* u_processed = (uint8_t*)(dstFrame->data + y_length);
    uint8_t* v_processed = (uint8_t*)(dstFrame->data + y_length + u_v_length); 
    try {
      // transcoding
      m_pBeautyEngine->processYUV(y, u, v, srcFrame->width, srcFrame->height, 0.0f);
      m_pBeautyEngine->getOutputYUVData(y_processed, u_processed, v_processed);

      // post transcoding
      if (y_processed && u_processed && v_processed) {
        log("onProcessVideoFrame copy transcoded data to dst\n");
      } else {
        log("[E]onProcessVideoFrame return invalid data\n");
      }
    } catch(std::exception& e) {
      log("[E]onProcessVideoFrame processYUV or  error:%s\n", e.what());
    }
  }while(false);

#ifdef _WIN32
  if (mHDc) {
    wglMakeCurrent(mHDc, NULL);
  }
#else
  CGLSetCurrentContext(NULL);
#endif

  log("onProcessVideoFrame end\n");
  return;
}

ITRTCPlugin* createTRTCPlugin() {
  return new XHSBeautyPlugin();
}

void destoryTRTCPlugin(ITRTCPlugin* plugin) {
  if (plugin) {
    delete plugin;
  }
}