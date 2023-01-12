#ifndef TRTC_ELECTRON_PLUGIN_XHS_BEAUTY_H_
#define TRTC_ELECTRON_PLUGIN_XHS_BEAUTY_H_

#include "IVideoFramePlugin.h"
// #include "xhs_mobile_common.h"
// #include "XYBaseFilter.h"
#include "XYCGWindowsEngine.h"
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#ifdef _WIN32
// #include "windows.h"
#else
#include <OpenGL/OpenGL.h>
#endif
#include "common/rapidjson/document.h"
#include "common/rapidjson/writer.h"

using namespace rapidjson;

class XHSBeautyPlugin: public IVideoFramePlugin {
  public:
    XHSBeautyPlugin();
    virtual ~XHSBeautyPlugin();
    virtual void onProcessVideoFrame(TRTCVideoFrame *srcFrame, TRTCVideoFrame * dstFrame);

    virtual bool init() override;
    virtual bool uninit() override;
    virtual bool load(const char* path) override;
    virtual bool unload() override;
    virtual bool enable(bool enabled) override;
    virtual bool setParameter(const char* param) override;
    virtual TRTC_PLUGIN_TYPE getPluginType() {
      return TRTC_PLUGIN_TYPE::TRTC_PLUGIN_VIDEO;
    }

  protected:
    bool isLoaded = false;
    bool isEnabled = false;
    bool isBeautyEngineInited = false;
    std::string sLicense = "";
    std::string sUserId = "";
    CG::XYCGWindowsEngine* m_pBeautyEngine = nullptr;
    std::string filterResourceBasePath = "";
    void handleAiModePathParam(const std::string path);
    void handleBeautyParam(Value& beauty);
    void handleFilterParam(Value& filter);
    void handleColorfulParam(Value& colorfulAdjustment);
    
    std::mutex mMutex;
    bool isGlInit = false;
    bool initOpenGL();
    bool uninitOpenGL();
#if defined(_WIN32)
    HWND mHWin = NULL;
    HDC mHDc = NULL;
    HGLRC mHRc = NULL;
#else
    CGLContextObj mCglContext;
#endif
};

#endif // TRTC_ELECTRON_PLUGIN_XHS_BEAUTY_H_
