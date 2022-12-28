#ifndef _PLUGIN_ITRTCPLUGIN_
#define _PLUGIN_ITRTCPLUGIN_

#if defined(_WIN32)
#define EXPORTS  __declspec(dllexport)
#else
#define EXPORTS
#endif

typedef void (*PluginErrorCallbackFunc)(int error, const char* msg, void* param);

enum class TRTC_PLUGIN_TYPE:int {
    TRTC_PLUGIN_UNKNOWN = 0,
    TRTC_PLUGIN_VIDEO = 1,
};

class ITRTCPlugin {
public:
    virtual ~ITRTCPlugin() {}
    virtual bool init() = 0;
    virtual bool uninit() = 0;
    virtual bool load(const char* path) = 0;
    virtual bool unload() = 0;
    virtual bool enable(bool enabled) = 0;
    virtual bool setParameter(const char* param) = 0;
    virtual TRTC_PLUGIN_TYPE getPluginType() {
        return TRTC_PLUGIN_TYPE::TRTC_PLUGIN_VIDEO;
    }
    void setErrorCallback(PluginErrorCallbackFunc callback, void* param) {
        error_callback_ = callback;
        param_ = param;
    }
    void onError(int error, const char* msg) {
        if (error_callback_) {
            error_callback_(error, msg, param_);
        }
    }

  private:
      PluginErrorCallbackFunc error_callback_ = nullptr;
      void* param_ = nullptr;
};

extern "C" EXPORTS ITRTCPlugin* createTRTCPlugin();
extern "C" EXPORTS void destoryTRTCPlugin(ITRTCPlugin*);
extern "C" EXPORTS void enableDebug(bool, const char*);

#endif
