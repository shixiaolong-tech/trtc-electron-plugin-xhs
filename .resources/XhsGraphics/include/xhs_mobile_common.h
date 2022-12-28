//
//  xhs_mobile_common.h
//  XHSRenderEngine
//
//  Created by 李萌 on 2018/8/20.
//  Copyright © 2018年 李萌. All rights reserved.
//

#ifndef xhs_mobile_common_h
#define xhs_mobile_common_h

#include <stdio.h>

#ifdef __ANDROID__
#include <jni.h>
#else

#endif

#define SDK_EXPORTS

//test
#ifdef _MSC_VER
#   ifdef __cplusplus
#       ifdef XHS_STATIC_LIB
#           define XHS_SDK_API  extern "C"
#       else
#           ifdef SDK_EXPORTS
#               define XHS_SDK_API extern "C" __declspec(dllexport)
#           else
#               define XHS_SDK_API extern "C" __declspec(dllimport)
#           endif
#       endif
#   else
#       ifdef XHS_STATIC_LIB
#           define XHS_SDK_API
#       else
#           ifdef SDK_EXPORTS
#               define XHS_SDK_API __declspec(dllexport)
#           else
#               define XHS_SDK_API __declspec(dllimport)
#           endif
#       endif
#   endif
#else /* _MSC_VER */
#   ifdef __cplusplus
#       ifdef SDK_EXPORTS
#           define XHS_SDK_API extern "C" __attribute__((visibility ("default")))
#       else
#           define XHS_SDK_API extern "C"
#       endif
#   else
#       ifdef SDK_EXPORTS
#           define XHS_SDK_API __attribute__((visibility ("default")))
#       else
#           define XHS_SDK_API
#       endif
#   endif
#endif


#define XHS_GLES_STRINGIZE(x)   #x
#define XHS_GLES_STRINGIZE2(x)  XHS_GLES_STRINGIZE(x)
#define XHS_GLES_STRING(x)      XHS_GLES_STRINGIZE2(x)

#define MT_MOST_PLANT 8

#define XHS_MOST_TITLE_PLANT 8

#define XHS_MOST_PICTURE_PLANT 8

#define XY_EFFECT_MOST_PARAM 8

#define XY_MOST_SMALL_WIDTH 16
#define XY_MOST_SMALL_HEIGHT 16

///  handle declearation
typedef void* xhs_handle_t;

///result
typedef int  xhs_result_t;

#define XY_OK                           0   ///< 正常运行
#define XY_E_ENGINE_HANDLE              -2  ///<引擎句柄出错
#define XY_E_MANAGER_HANDLE             -3  ///< manager 句柄出错
#define XY_E_FILTER_HANDLE              -4  ///< filter 句柄出错
#define XY_E_CREATE_ENGINE_HANDLE       -10 ///< create 接口时引擎句柄出错
#define XY_E_CREATE_MANAGER_HANDLE      -11 ///< create 接口时 manager 句柄出错

#define XY_E_INVALIDARG                 -2001  ///< 无效参数
#define XY_E_HANDLE                     -2002  ///< 句柄错误
#define XY_E_NO_SELF_EFFECT             -2003  ///< 无自研特效效果

typedef enum {
    XHS_FILTER_ERROR_CODE_FILTER_PARAM_NULL = -41,
    XHS_FILTER_ERROR_CODE_FILTER_NULL = -40,
    XHS_FILTER_ERROR_CODE_MANAGER_NULL = -30,
    XHS_FILTER_ERROR_CODE_GL_DRIVER_NOT_SUPPORT = -3,
    XHS_FILTER_ERROR_CODE_LICENSE_EXPIRED = -2,
    XHS_FILTER_ERROR_CODE_LICENSE_INVALID = -1,
    XHS_FILTER_ERROR_CODE_SUCCESS = 0,
} xhs_filter_error_code;

typedef enum {
    XHS_SPE_EFFECT_INTERNAL,      /// 自研效果
    XHS_SPE_EFFECT_EXTERNAL      /// 第三方效果
} xhs_special_type;

typedef enum {
    XHS_GLCONTEXT_EXTERNAL, /// 外部产生并管理gl环境  暂时表示image mode
    XHS_GLCONTEXT_INTERNAL  /// 内部产生并管理gl环境  暂时表示video mode
} xhs_context_type;

/// image rotate type definition
typedef enum {
    XHS_CLOCKWISE_ROTATE_0 = 0,  ///< 图像不需要旋转,图像中的人脸为正脸
    XHS_CLOCKWISE_ROTATE_90 = 1, ///< 图像需要顺时针旋转90度,使图像中的人脸为正
    XHS_CLOCKWISE_ROTATE_180 = 2,///< 图像需要顺时针旋转180度,使图像中的人脸为正
    XHS_CLOCKWISE_ROTATE_270 = 3 ///< 图像需要顺时针旋转270度,使图像中的人脸为正
} xhs_rotate_type;

//商汤
//#define SENSETIME_MAKEUP_KEY "Makeup_Face_Extra"
//#define SENSETIME_ATTRIBUTE_MTNET_KEY "Attribute_MTNet"
//#define SENSETIME_HAND_KEY "Hand"
//#define SENSETIME_Iris_KEY "Iris"

typedef enum {
    XHS_FEATURES_BEAUTY,          /// 美颜
    XHS_FEATURES_FILTER,          /// 滤镜
    XHS_FEATURES_COLORFUL,        /// 色彩调整，例如：饱和度等
    XHS_FEATURES_STICKER,         /// 贴纸
    XHS_FEATURES_ATTRIBUTES,      /// 属性
    XHS_FEATURES_TRACK,           /// 追踪
    XHS_FEATURES_MAKEUP
} xhs_features_type;


typedef enum {
    XHS_COLORFUL_BRIGHT,      ///亮度
    XHS_COLORFUL_CONTRAST,    ///对比度
    XHS_COLORFUL_WHITEBALANCE,///色温
    XHS_COLORFUL_SATURATION,  ///饱和度
    XHS_COLORFUL_GRAININESS   ///颗粒度
} xhs_colorful_type;


//蒙层滤镜
typedef enum {
    XHS_CONSUM_SPECIAL_FILTER_NORMAL,           //null
    XHS_CONSUM_SPECIAL_FILTER_3DLUT,            //lut                             only lut
    XHS_CONSUM_SPECIAL_FILTER_EDGEBURN,         //lut + edgeburn                  first lut, second edgeburn(*),
    XHS_CONSUM_SPECIAL_FILTER_TONECURVE,        //tooncurve
    XHS_CONSUM_SPECIAL_FILTER_METAL,            //lut + metal                     first lut, second metal, third softlight
    XHS_CONSUM_SPECIAL_FILTER_EDGE_TONE_METAL,  //edgeburn + tooncurve + metal
    XHS_CONSUM_SPECIAL_FILTER_EDGE_METAL,       //lut + edgeburn + metal    first lut, second edgeburn(*), third metal, fourth softlight
    XHS_CONSUM_SPECIAL_FILTER_METAL_TONE,       //metal + curve
    XHS_CONSUM_SPECIAL_FILTER_NOISE,            //lut + noise
    XHS_CONSUM_SPECIAL_FILTER_THREE_LAYER,      //three layer
    XHS_CONSUM_SPECIAL_FILTER_FOUR_LAYER,       //four layer
    XHS_CONSUM_SPECIAL_FILTER_FIVE_LAYER,       //five layer, first lut,  second,third,fourth,fifth:alpha
    XHS_CONSUM_SPECIAL_FILTER_SIX_LAYER,        //six layer
    XHS_CONSUM_SPECIAL_FILTER_FIVE_TWOBLEND,    //five layer, first lut, second blend(0.8,漏光等), third,fourth,fifth:alpha
    XHS_CONSUM_SPECIAL_FILTER_FIVE_THREEBLEND,   //five layer, first lut(色彩), second blend(柔光叠加，现有强度为0.25，噪声), third 叠加(Overlay，现在的强度为0.4， 暗角), fourth（screen， 强度为1.0， 漏光）,fifth （alpha，杂质）
    XHS_CONSUM_SPECIAL_FILTER_FIVE_THREEBLEND_EDGE,   //five layer, first lut, second burn(*), third mix(0.15,噪声等), fourth,fifth:alpha
    XHS_CONSUM_SPECIAL_FILTER_BLUR_SOFTLIGHT,
    XHS_CONSUM_SPECIAL_FILTER_VSCO

} xhs_consum_special_filter_type;



//滤镜种类
typedef enum {
    XY_GRAPHIC_FILTER_NORMAL,               // null
    XY_GRAPHIC_FILTER_LUT3D,                // lut
    XY_GRAPHIC_FILTER_SHARPEN,              // 锐化
    XY_GRAPHIC_FILTER_CLARITY,              // 清晰度
    XY_GRAPHIC_FILTER_GRAIN,                // 噪点
    XY_GRAPHIC_FILTER_VIGNETTE,             // 暗角
    XY_GRAPHIC_FILTER_BLEND_SOURCEOVER,     //alpha叠加
    XY_GRAPHIC_FILTER_BLEND_SOFTLIGHT,      //柔光(Soft Light)模式
    XY_GRAPHIC_FILTER_BLEND_OVERLAY,        //叠加(Overlay)模式
    XY_GRAPHIC_FILTER_BLEND_SCREEN,         //滤色(Screen)模式
    XY_GRAPHIC_FILTER_BLEND_MULTIPLY,       //正片叠底(Multiply)模式
    XY_GRAPHIC_FILTER_BLEND_NORMAL,         //正常(Normal)模式
    XY_GRAPHIC_FILTER_BLEND_LIGHTER_COLOR,  //浅色(Lighter Color)模式
    XY_GRAPHIC_FILTER_BLEND_ADD,            //增加模式(Lighter Color)
    XY_GRAPHIC_FILTER_BLEND_DARKEN,         //变暗(Darken)模式
    XY_GRAPHIC_FILTER_BLEND_COLOERBURN,     //颜色加深(Clolor Burn)模式
    XY_GRAPHIC_FILTER_BLEND_LINEARBURN,     //线性加深(Linear Burn)模式
    XY_GRAPHIC_FILTER_BLEND_LIGHTEN,        //变亮(Lighten)模式
    XY_GRAPHIC_FILTER_BLEND_DARKERCOLOR,    //深色(Lighten)模式
    XY_GRAPHIC_FILTER_BLEND_COLORDODGE,     //颜色减淡(Clolor Dodge)模式
    XY_GRAPHIC_FILTER_BLEND_LINEARDODGE,    //线性减淡(Linear Dodge)模式
    XY_GRAPHIC_FILTER_BLEND_HARDLIGHT,      //强光(Hard Light)模式
    XY_GRAPHIC_FILTER_BLEND_VIVIDLIGHT,     //亮光(Vivid Light)模式
    XY_GRAPHIC_FILTER_BLEND_LINEARLIGHT,    //线性光(Linear Light)模式
    XY_GRAPHIC_FILTER_BLEND_PINLIGHT,       //点光(Pin Light)模式
    XY_GRAPHIC_FILTER_BLEND_HARDMIX,        //实色混合(hard mix)模式
    XY_GRAPHIC_FILTER_BLEND_DIFFERENCE,     //差值(Diference)模式
    XY_GRAPHIC_FILTER_BLEND_EXCLUSION,      //排除(Exclusion)模式
    XY_GRAPHIC_FILTER_BLEND_SUBTRACT,       //减去(Subtract)模式
    XY_GRAPHIC_FILTER_BLEND_DIVIDE,         //划分(divide)模式
    XY_GRAPHIC_FILTER_BLEND_HUE,            //色相(Hue)模式
    XY_GRAPHIC_FILTER_BLEND_SATURATION,     //饱和度(Saturation)模式
    XY_GRAPHIC_FILTER_BLEND_COLOR,          //颜色(Color)模式
    XY_GRAPHIC_FILTER_BLEND_LUMINOSITY,     //亮度(Luminosity)模式
    XY_GRAPHIC_FILTER_BLEND_MASK,           //图层遮罩(mask)
    XY_GRAPHIC_FILTER_TRANSFORM2D,          //
    XY_GRAPHIC_FILTER_PREMULTY_RECOVER,     //
    XY_GRAPHIC_FILTER_MOTION_BLUR,          //
    XY_GRAPHIC_FILTER_DISPERSION,           //
    XY_GRAPHIC_FILTER_WATER_REFLECT,        //
    XY_GRAPHIC_FILTER_GLITCH,               //
    XY_GRAPHIC_FILTER_STRIPE,               //
    XY_GRAPHIC_FILTER_SPLIT,                //分屏，基本的分屏操作，2、3、4、6、9分屏
    XY_GRAPHIC_FILTER_SPLIT_MODE_1,         //
    XY_GRAPHIC_FILTER_SPLIT_MODE_2,         //分屏
    XY_GRAPHIC_FILTER_SOFT_LIGHT,           //柔光
    XY_GRAPHIC_FILTER_DETECT,               //detect，检测高光点
    XY_GRAPHIC_FILTER_BLINGBLING,           //blingbling
    XY_GRAPHIC_FILTER_FOCUS_BLUR_AREA,      //局部模糊
    XY_GRAPHIC_FILTER_FOCUS_BOXBLUR_AREA,   //局部box模糊
    XY_GRAPHIC_FILTER_EXTRACT_HIGHLIGHT,    //抽取高光
    XY_GRAPHIC_FILTER_RADIAL_BLUR,          //径向模糊
    XY_GRAPHIC_FILTER_SEGMENT_BACKGROUND_EFFECT, //抠图
    XY_GRAPHIC_FILTER_HAIR_DYE_EFFECT,       //头发染色
    XY_GRAPHIC_FILTER_HAIR_SHARPEN_EFFECT,   //突出头发细节和XY_GRAPHIC_FILTER_EYE_DYE_EFFECT一块使用
    XY_GRAPHIC_FILTER_PICTURE_RENDER,       //picture render
    XY_GRAPHIC_FILTER_LUMINACE,             //灰度图
    XY_GRAPHIC_FILTER_SOBEL_DETECT,         //sobel detect
    XY_GRAPHIC_FILTER_CARTOON,              //黑白漫画
    XY_GRAPHIC_FILTER_COLORFUL_CARTOON,     //彩色漫画
    XY_GRAPHIC_FILTER_BOXBLUR_X,            //box blur x
    XY_GRAPHIC_FILTER_BOXBLUR_Y,            //box blur y
    XY_GRAPHIC_FILTER_BILATERAL,            //bilateral filter
    XY_GRAPHIC_FILTER_FIXEDFRAME,           //Fixed frame 定帧操作
    XY_GRAPHIC_FILTER_MASK_AREA,             //mask区域划分
    XY_GRAPHIC_FILTER_SEGMENT_PROP          //背景分割跟脸道具
} xy_graphic_filter_type;

typedef enum {
    XY_AR_MAKEUP_MODE_ROUND = 1001,           //圆心透视模版
    XY_AR_MAKEUP_MODE_GRID_UP_DOWN = 1002,    //双输入模版
    XY_AR_MAKEUP_MODE_GRID_FOUR = 1003       //四宫格模版
} xy_ar_makeup_template_type;


//静态贴纸动画效果
//一级效果
typedef enum {
    XHS_STATIC_PICTURE_ANIMATION_MAIN_NORMAL,
    XHS_STATIC_PICTURE_ANIMATION_MAIN_ALPHA
} xhs_static_picture_animation_main_type;
//二级效果
typedef enum {
    XHS_STATIC_PICTURE_ANIMATION_SECONDARY_NORMAL
} xhs_static_picture_animation_secondary_type;

//裁剪变换
typedef enum {
    XHS_TRANSFORM_CROP_MAIN_NORMAL,
    XHS_TRANSFORM_CROP_MAIN_CROP,
    XHS_TRANSFORM_CROP_MAIN_FIT
} xhs_transform_crop_main_type;



typedef enum {
    XHS_PIX_FMT_GRAY8,   ///< Y    1        8bpp ( 单通道8bit灰度像素 )
    XHS_PIX_FMT_YUV420P, ///< YUV  4:2:0   12bpp ( 3通道, 一个亮度通道, 另两个为U分量和V分量通道, 所有通道都是连续的. 只支持I420)
    XHS_PIX_FMT_NV12,    ///< YUV  4:2:0   12bpp ( 2通道, 一个通道是连续的亮度通道, 另一通道为UV分量交错 )
    XHS_PIX_FMT_NV21,    ///< YUV  4:2:0   12bpp ( 2通道, 一个通道是连续的亮度通道, 另一通道为VU分量交错 )
    XHS_PIX_FMT_BGRA8888,///< BGRA 8:8:8:8 32bpp ( 4通道32bit BGRA 像素 )
    XHS_PIX_FMT_BGR888,  ///< BGR  8:8:8   24bpp ( 3通道24bit BGR 像素 )
    XHS_PIX_FMT_RGBA8888,///< RGBA 8:8:8:8 32bpp ( 4通道32bit RGBA 像素 )
    XHS_PIX_FMT_RGB888   ///< RGB  8:8:8   24bpp ( 3通道24bit RGB 像素 )
} xhs_pixel_format;


typedef struct VideoData {
    unsigned char* data = nullptr;
    xhs_pixel_format format;
    int width;
    int height;
    int stride;
    int input_texID;
    int output_texID;
}VideoData;


typedef struct Filter_imageData {
    unsigned char* data;
    xhs_pixel_format format;
    int width;
    int height;
    int image_texID;
}Filter_imageData;

///sensetime
typedef enum {
    XHS_ST_EFFECT_DETECT, /// 人脸检测
    XHS_ST_EFFECT_BEAUTY, /// 美颜
    XHS_ST_EFFECT_STICKER, /// 贴纸
    XHS_ST_EFFECT_FILTER, /// 滤镜
    XHS_ST_EFFECT_MAKEUP  ///  美妆
} xhs_st_effect_type;


///@brief 美颜参数类型
typedef enum {
    XHS_BEAUTIFY_REDDEN_STRENGTH = 1,    /// 红润强度, [0,1.0], 默认值0.36, 0.0不做红润
    XHS_BEAUTIFY_SMOOTH_STRENGTH = 3,    /// 磨皮强度, [0,1.0], 默认值0.74, 0.0不做磨皮
    XHS_BEAUTIFY_WHITEN_STRENGTH = 4,    /// 美白强度, [0,1.0], 默认值0.30, 0.0不做美白
    XHS_BEAUTIFY_ENLARGE_EYE_RATIO = 5,  /// 大眼比例, [0,1.0], 默认值0.13, 0.0不做大眼效果
    XHS_BEAUTIFY_SHRINK_FACE_RATIO = 6,  /// 瘦脸比例, [0,1.0], 默认值0.11, 0.0不做瘦脸效果
    XHS_BEAUTIFY_SHRINK_JAW_RATIO = 7,    /// 小脸比例, [0,1.0], 默认值0.10, 0.0不做小脸效果
    XHS_BEAUTIFY_CONTRAST_STRENGTH = 8,  /// 对比度强度, [0,1.0], 默认值0.05, 0.0不做对比度处理
    XHS_BEAUTIFY_SATURATION_STRENGTH = 9, /// 饱和度强度, [0,1.0], 默认值0.50, 0.0不做饱和度处理

    XHS_BEAUTIFY_DEHIGHLIGHT_STRENGTH = 10,  /// 去高光强度, [0,1.0], 默认值0.0, 0.0不做去高光
    XHS_BEAUTIFY_NARROW_FACE_STRENGTH = 11,  /// 窄脸强度, [0,1.0], 默认值0.0, 0.0不做窄脸

    XHS_BEAUTIFY_3D_NARROW_NOSE_RATIO = 20,     /// 瘦鼻比例，[0, 1.0], 默认值为0.0，0.0不做瘦鼻
    XHS_BEAUTIFY_3D_NOSE_LENGTH_RATIO = 21,     /// 鼻子长短比例，[-1, 1], 默认值为0.0, [-1, 0]为短鼻，[0, 1]为长鼻
    XHS_BEAUTIFY_3D_CHIN_LENGTH_RATIO = 22,     /// 下巴长短比例，[-1, 1], 默认值为0.0，[-1, 0]为短下巴，[0, 1]为长下巴
    XHS_BEAUTIFY_3D_MOUTH_SIZE_RATIO = 23,      /// 嘴型比例，[-1, 1]，默认值为0.0，[-1, 0]为放大嘴巴，[0, 1]为缩小嘴巴
    XHS_BEAUTIFY_3D_PHILTRUM_LENGTH_RATIO = 24, /// 人中长短比例，[-1, 1], 默认值为0.0，[-1, 0]为长人中，[0, 1]为短人中
    XHS_BEAUTIFY_3D_HAIRLINE_HEIGHT_RATIO = 25, /// 发际线高低比例，[-1, 1], 默认值为0.0，[-1, 0]为低发际线，[0, 1]为高发际线
    XHS_BEAUTIFY_3D_THIN_FACE_SHAPE_RATIO = 26 /// 瘦脸型比例， [0,1.0], 默认值0.0, 0.0不做瘦脸型效果
} xhs_st_beautify_type;

///@brief 美装参数类型
typedef enum
{
    XHS_MAKEUP_TYPE_NORMAL = 0,       //无效果
    XHS_MAKEUP_TYPE_EYE = 1,          //眼部美妆
    XHS_MAKEUP_TYPE_FACE = 2,         //腮部美妆
    XHS_MAKEUP_TYPE_LIP = 3,          //唇部美妆
    XHS_MAKEUP_TYPE_NOSE = 4,         //修容美妆
    XHS_MAKEUP_TYPE_BROW = 5,         //眉部美妆
    XHS_MAKEUP_TYPE_EYELINER = 6,     //眼线美妆
    XHS_MAKEUP_TYPE_EYELASH = 7,      //眼睫毛美妆
    XHS_MAKEUP_TYPE_EYEBALL = 8       //眼球美化
}xhs_st_makeup_type;

///@brief 贴纸参数类型
typedef enum
{
    XHS_STICKER_TYPE_NORMAL = 0,       //无效果
}xhs_st_sticker_type;



typedef struct STTotalModelPath {
    char* mainModelPath;
    char* makeupModelPath;
    char* handModelPath;
    char* irisModelpPath;
    char* mtNetModelPath;

#ifdef __ANDROID__
    STTotalModelPath() {
        mainModelPath = nullptr;
        makeupModelPath = nullptr;
        handModelPath = nullptr;
        irisModelpPath = nullptr;
        mtNetModelPath = nullptr;
    }
#else

#endif


}STTotalModelPath;


//licence
typedef struct XHSLicense {
    const char* license_buf;
    int license_size;
    const char* activation_code;
    int activation_code_len;
} XHSLicense;

//model path
typedef struct XHSModelPath {
    //path
    int type;
    char* action_modelPath;

    //buffer
    unsigned char* buffer;
    unsigned int buffer_size;
    unsigned int config;
#ifdef __ANDROID__
    XHSModelPath() {
        action_modelPath = nullptr;
        buffer = nullptr;
        buffer_size = 0;
        config = 0;
        type = 0;
    }
#else

#endif
} XHSModelPath;

//license path and model path
typedef struct ExtraSource {
    XHSLicense* xhsLicense;
    XHSModelPath* st_mainModelPath;
    XHSModelPath* st_makeupModelPath;
    XHSModelPath* st_irisModelPath;
    XHSModelPath* st_handModelPath;
    XHSModelPath* st_mtNetModelPath;

    //render mode,  image 0; video 1;
    int render_mode;
#ifdef __ANDROID__
    JNIEnv* env;
    jobject context;

    ExtraSource() {
        st_mainModelPath = nullptr;
        st_makeupModelPath = nullptr;
        st_irisModelPath = nullptr;
        st_handModelPath = nullptr;
        st_mtNetModelPath = nullptr;
        render_mode = 1;
    }
#else


#endif
} ExtraSource;


typedef struct STEffectParam {
    xhs_st_effect_type st_effect_type;
    int st_type;
    float value;
}STEffectParam;

typedef struct STBeautyParam {
    xhs_st_beautify_type beauty_type;
    float value;
}STBeautyParam;

typedef struct STMakeupParam {
    xhs_st_makeup_type makeup_type;
    int isChangePath;
    char* Path;
    int isChangeValue;
    int isChangeValue2;
    float value;
    //    STMakeupParam() {
    //        makeup_type = XHS_MAKEUP_TYPE_NORMAL;
    //        isChangePath = 0;
    //        Path = nullptr;
    //        isChangeValue = 0;
    //        value = 0.0f;
    //    }
}STMakeupParam;

typedef struct XYFilterlayerParam {
    char* layerNames;
    int width;
    int height;
    float strength;
    int layerEffectType;

    int isSupportColorFrame;
    int colorFrameValue[3];
    int colorFrameLength;
}XYFilterlayerParam;


//滤镜置信区域参数
typedef struct XYFilterRoiParam {
    float roi_point_x;     // 置信区域起点x坐标 像素坐标，相对于整个纹理的像素坐标
    float roi_point_y;     // 置信区域起点y坐标 像素坐标，相对于整个纹理的像素坐标
    float roi_width;       // 置信区域宽度 像素宽度
    float roi_height;      // 置信区域高度 像素高度

    int roi_color_flag;    // 色彩效果是否在置信区域， 只在置信区域 1；在全图区域 0
    int roi_picture_flag;  // 边框效果是否在置信区域， 只在置信区域 1；在全图区域 0

//    XYFilterRoiParam () {
//        roi_color_flag = 0;
//        roi_picture_flag = 0;
//    }
}XYFilterRoiParam;


typedef struct GLTexture {
    unsigned int texture;
    int width;
    int height;
    xhs_pixel_format format;
    int rotation;
}GLTexture;


typedef struct XYTextureRoi {
    float roi_point_x;
    float roi_point_y;
    float roi_width;
    float roi_height;
    xhs_transform_crop_main_type type;          //fit mode类型
}XYTextureRoi;


typedef enum {
    XY_MULTY_FUSION_NORMAL,
    XY_MULTY_FUSION_SPLIT,
    XY_MULTY_FUSION_COVER,
}xy_texture_multy_fusion_type;


typedef enum {
    XHS_SKIN_WHITENING = 0,//美白
    XHS_SKIN_SMOOTH,//磨皮
    XHS_THIN_FACE,//瘦脸
    XHS_NARROW_FACE,//窄脸
    XHS_SHORT_FACE,//短脸
    XHS_BIG_EYE,//大眼
    XHS_THIN_JAW_BONE,//瘦颚骨
    XHS_SHRINKAGE_PHILTRUM,//缩人中
    XHS_THIN_NOSE,//瘦鼻
    XHS_GROWTH_NOSE,//长鼻
    XHS_SHORT_CHIN,//收下巴
    XHS_EYE_DISTANCE,//眼距
    XHS_MOUTH_RESHAPE,//嘴形
    XHS_OPEN_EYE_CORNER,//开眼角
    XHS_ROUND_EYE,//圆眼
    XHS_FOREHEAD_PLUMP,//额头
    XHS_RHINOPLASTY,//侧脸隆鼻
    XHS_CHEEKBONES,//苹果肌
    XHS_SMILE_CORNER_MOUTH,//微笑嘴角
    XHS_SPARKLE_EYE,//亮眼
    XHS_WHITEN_TEENTH,//美牙
    XHS_EYEBROWS_THICK,//眉毛厚度
    XHS_EYEBROWS_POSITION,//眉毛位置
    XHS_EYE_ROTATE,//眼睛角度
    XHS_LIP_RUDDY,//红唇
    XHS_LOCAL_EXFOLIATING_BLACK_EYE,//黑眼圈
    XHS_LOCAL_EXFOLIATING_NASOLABIAL_FOLDS//祛法令纹
} FaceBeautyType;

typedef enum {
    XY_BLUSH_EFFECT,//腮红
    XY_PUPIL_EFFECT,//美瞳
    XY_EYEBROW_EFFECT,//美眉
    XY_EYELASH_EFFECT,//眼睫毛
    XY_EYELINER_EFFECT,//眼线
    XY_FACE_SHADOW_EFFECT,//脸部高光
    XY_LIP_EFFCET//唇彩
} MakeupType;



#endif /* xhs_mobile_common_h */