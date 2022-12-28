#pragma once
#include <string>
#include "XYCGWindowsEngine.h"
#include "json.hpp"

typedef enum {
    XHS_PLUGIN_COMMAND_INVALID,
    XHS_PLUGIN_BEAUTY_FILTER_SWITCH,       /// beauty filter switch. buf should be a bool
    XHS_PLUGIN_BEAUTY_TYPE,                /// beauty type filter params set. buf should be BeautyFilterAid
    XHS_PLUGIN_COLOR_FILTER_SWITCH,        /// color filter switch. buf should be a bool
    XHS_PLUGIN_COLOR_TYPE,                 /// color type filter params set. buf should be ColorFilterAid
    XHS_PLUGIN_LUT_FILTER_SWITCH,          /// lut filter switch. buf should be a bool
    XHS_PLUGIN_LUT_FILTER_TYPE,            /// lut filter params set, include path of lut&json files. buf should be LutFilterAid
    XHS_PLUGIN_COMMAND_COUNT
} xhs_Command_type;

xhs_Command_type getKeyCommandType(const char* key);

NLOHMANN_JSON_SERIALIZE_ENUM(FaceBeautyType, {
    {XHS_SKIN_WHITENING, "skin_whitening"},
    {XHS_SKIN_SMOOTH, "skin_smooth"},
    {XHS_THIN_FACE, "thin_face"},
    {XHS_NARROW_FACE, "narrow_face"},
    {XHS_SHORT_FACE, "short_face"},
    {XHS_BIG_EYE, "big_face"},
    {XHS_THIN_JAW_BONE, "thin_jaw_bone"},
    {XHS_SHRINKAGE_PHILTRUM, "shrinking_philtrum"},
    {XHS_THIN_NOSE, "thin_nose"},
    {XHS_GROWTH_NOSE, "growth_nose"},
    {XHS_SHORT_CHIN, "short_chin"},
    {XHS_EYE_DISTANCE, "eye_distance"},
    {XHS_MOUTH_RESHAPE, "mouth_reshape"},
    {XHS_OPEN_EYE_CORNER, "open_eye_corner"},
    {XHS_ROUND_EYE, "round_eye"},
    {XHS_FOREHEAD_PLUMP, "forehead_plump"},
    {XHS_RHINOPLASTY, "thinoplasty"},
    {XHS_CHEEKBONES, "cheek_bone"},
    {XHS_SMILE_CORNER_MOUTH, "smile_corner_mouth"},
    {XHS_SPARKLE_EYE, "sparkle_eye"},
    {XHS_WHITEN_TEENTH, "white_teeth"},
    {XHS_EYEBROWS_THICK, "eyebrows_thick"},
    {XHS_EYEBROWS_POSITION, "eyebrows_position"},
    {XHS_EYE_ROTATE, "eye_rotate"},
    {XHS_LIP_RUDDY, "lip_ruddy"},
    {XHS_LOCAL_EXFOLIATING_BLACK_EYE, "local_exfoliating_black_eye"},
    {XHS_LOCAL_EXFOLIATING_NASOLABIAL_FOLDS, "local_exfoliating_nasolabial_folds"},
    })

    struct BeautyFilterAid {
    FaceBeautyType  _type;
    bool            _switch = false;
    float           _beauty_type_intensity = 0.0f;

    std::string to_json() {
        nlohmann::json j = nlohmann::json{ {"_type", _type}, {"_switch", _switch}, {"_beauty_type_intensity", _beauty_type_intensity} };
        return j.dump();
    }

    void from_json(std::string s) {
        auto j = nlohmann::json::parse(s);
        if (j.find("_type") != j.end()) {
            j.at("_type").get_to(_type);
        }
        if (j.find("_switch") != j.end()) {
            j.at("_switch").get_to(_switch);
        }
        if (j.find("_beauty_type_intensity") != j.end()) {
            j.at("_beauty_type_intensity").get_to(_beauty_type_intensity);
        }
    }
};

NLOHMANN_JSON_SERIALIZE_ENUM(xhs_colorful_type, {
    {XHS_COLORFUL_BRIGHT, "colorful_bright"},
    {XHS_COLORFUL_CONTRAST, "colorful_contrast"},
    {XHS_COLORFUL_WHITEBALANCE, "colorful_white_balance"},
    {XHS_COLORFUL_SATURATION, "colorful_saturation"},
    {XHS_COLORFUL_GRAININESS, "colorful_graininess"},
    })

    struct ColorFilterAid {
    xhs_colorful_type   _type;
    bool                _switch = false;
    float               _color_intensity = 0.0f;

    std::string to_json() {
        nlohmann::json j = nlohmann::json{ {"_type", _type}, {"_switch", _switch}, {"_color_intensity", _color_intensity} };
        return j.dump();
    }

    void from_json(std::string s) {
        auto j = nlohmann::json::parse(s);
        if (j.find("_type") != j.end()) {
            j.at("_type").get_to(_type);
        }
        if (j.find("_switch") != j.end()) {
            j.at("_switch").get_to(_switch);
        }
        if (j.find("_color_intensity") != j.end()) {
            j.at("_color_intensity").get_to(_color_intensity);
        }
    }
};

struct LutFilterAid {
    std::string _subPath;
    float       _lut_intensity = 0.0f;

    std::string to_json() {
        nlohmann::json j = nlohmann::json{ {"_subPath", _subPath}, {"_lut_intensity", _lut_intensity} };
        return j.dump();
    }

    void from_json(std::string s) {
        auto j = nlohmann::json::parse(s);
        if (j.find("_subPath") != j.end()) {
            j.at("_subPath").get_to(_subPath);
        }
        if (j.find("_lut_intensity") != j.end()) {
            j.at("_lut_intensity").get_to(_lut_intensity);
        }
    }
};


struct EngineInitParamsAid {
    std::string _license = "";
    std::string _userId = "";
    std::string _aiModelPath = "";
    std::string _beautyResPath = "";

    std::string to_json() {
        nlohmann::json j = nlohmann::json{ {"_license", _license}, {"_userId", _userId}, {"_aiModelPath", _aiModelPath}, {"_beautyResPath", _beautyResPath} };
        return j.dump();
    }

    void from_json(std::string s) {
        auto j = nlohmann::json::parse(s);
        if (j.find("_license") != j.end()) {
            j.at("_license").get_to(_license);
        }
        if (j.find("_userId") != j.end()) {
            j.at("_userId").get_to(_userId);
        }
        if (j.find("_aiModelPath") != j.end()) {
            j.at("_aiModelPath").get_to(_aiModelPath);
        }
        if (j.find("_beautyResPath") != j.end()) {
            j.at("_beautyResPath").get_to(_beautyResPath);
        }
    }
};