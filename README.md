# 小红书美颜库接入层工程

## 代码说明
```
./
│  
├─.resources                                  // 该目录这是存放小红书提供资源，实际代码工程不会用到
│  ├─beautify                                 // 小红书提供的美颜资源：模型、美颜、滤镜资源       
│  └─XhsGraphics                              // 小红书提供的 Win64 美颜动态库
│      
├─common                                      // C/C++ 源代码
│  └─rapidjson                                // C++ JSON 处理代码
│              
├─win-proj                                    // Windows 工程目录
│  │  build-win64.bat                         // TRTC 美颜插件动态库构建脚本（Win64)
│  │  TRTCElectronBeautyPlugin.sln            // Windows Visual Studio 2015 C++ 工程配置文件
│  │  
│  ├─dist-temp                                // 本地构建后，生成的动态库临时存放目录
│  │      
│  ├─TRTCElectronBeautyPlugin                 // Windows Visual Studio 2015 C++ 工程配置文件
│              
├─XHS-beauty-lib                              // 小红书美颜动态库
│  └─win
│     ├─include                               // 头文件
│     └─win64                                 // Win64 的动态库
│
│  ITRTCPlugin.h                              // TRTC Electron SDK 美颜插件头文件
│  IVideoFramePlugin.h                        // TRTC Electron SDK 美颜插件头文件
│  XHSBeautyPlugin.cpp                        // 小红书美颜插件实现文件
│  XHSBeautyPlugin.h                          // 小红书美颜插件头文件
│  glad.c
│  README.md
│  parameter_schema.json                      // 参数设置的JSON结构示例
                

```

## 构建动态库

1. 工程类型：Visual Studio 2015，工程文件：** win-proj\TRTCElectronBeautyPlugin.sln **

2. 构建 Win64 美颜插件动态库

可以使用 Visual Studio 2015 + 版本的构建工具，也可以通过命令行，执行 ** win-proj\build-win64.bat ** 脚本。

```cmd
REM 需要在命令行支持 msbuild 和 7z 命令
.\build-win64.bat
```

构建好的动态库位于 ** win-proj\dist-temp ** 目录下，也会生成一个压缩文件，位于 ** win-proj\release-win64.zip **。

## Electron应用中使用示例

1. 开启美颜
```javascript
import TRTCCloud from "trtc-electron-sdk";

// 获取资源文件路径，这里实现会因工程存在差异，以自身项目为准，仅做参考.
const path = require('path');
const libPath = path.resolve(APP_PATH, 'plugin-XHS/win64/TRTCElectronBeautyPlugin.dll');
const APP_PATH = app.getAppPath(); // 注意 app 只能在 Electron 主进程中使用，一般通过 Electron IPC 接口从主进程传入渲染进程
const aiModelPath = path.resolve(APP_PATH, 'plugin-XHS/win64/resources/slim-320.face_kpt_v2.mouth.eyebrow.bin');
const beautyResPath =path.resolve(APP_PATH, 'plugin-XHS/win64/resources/Beauty_Res');
const filterResPath =path.resolve(APP_PATH, 'plugin-XHS/win64/resources/Lut_Res');

// 小红书美颜参数
const params = {
  aiModelPath,
  beauty: {
    enable: true,
    resourcePath: beautyResPath,
    types: [{
      type: 0,
      enable: true,
      intensity: 0.9,
    }, {
      type: 26,
      enable: true,
      intensity: 0.1,
    }]
  },
  filter: {
    enable: false,
    resourcePath: filterResPath,
    intensity: 0.9,
    type: 'Forest',
  },
  colorful: {
    enable: true,
    types: [{
      type: 0,
      intensity: 0.9,
    }, {
      type: 2,
      intensity: 0.1,
    }]
  }
};

// 创建 TRTC 示例
const trtcCloud = new TRTCCloud();
// 初始化插件管理器
trtcCloud.initPluginManager();
// 添加摄像头美颜插件，支持多次调用，需要不同的 id 和 deviceId 组合，否则会相互覆盖
const cameraBeautyPlugin = trtcCloud.addPlugin({
  id: 'xiaohongshu',
  deviceId: '<camera ID>',
  path: libPath,
  type: 1,
});
// 设置美颜参数
cameraBeautyPlugin.setParameter(JSON.stringify(params));
// 启动美颜
cameraBeautyPlugin.enable();
```

2. 关闭美颜
```javascript
// ...
// 承接上一步定义的变量

// 禁用美颜插件
cameraBeautyPlugin.disable();
// 删除美颜插件
trtcCloud.removePlugin(cameraBeautyPlugin.id, cameraBeautyPlugin.deviceId);

cameraBeautyPlugin = null;
// 销毁插件管理器
trtcCloud.destroyPluginManager();
```

## 注意:
1. 美颜插件目前在代码中固定设置为 debug 模式，会在工程下写 `xhs_debug.log` 日志。可在 XHSBeautyPlugin.cpp 文件中创建美颜插件的构造函数中去掉 debug 模式。相关代码如下：
```
  enableDebug(true, "xhs_debug.log");
```

## 参考文档
