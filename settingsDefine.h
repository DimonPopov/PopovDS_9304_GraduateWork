#ifndef SETTINGSDEFINE_H
#define SETTINGSDEFINE_H

#define SETTING_FIRST_GROUP        ("Settings")
#define MODEL_LENGTH               ("ModelLength")
#define MODEL_COUNT                ("ModelCount")
#define MODEL_COLOR                ("ModelColor")
#define MODEL_SIZE                 ("ModelSize")
#define MODEL_VISIBILITY           ("ModelVisibility")
#define POSITION_SENSOR_COUNT      ("PositionSensorCount")
#define POSITION_SENSOR_COLOR      ("PositionSensorColor")
#define POSITION_SENSOR_SIZE       ("PositionSensorSize")
#define POSITION_SENSOR_VISIBILITY ("PositionSensorVisibility")
#define POSITION_SENSOR_END        ("PositionSensorEnd")
#define ACOUSTIC_SENSOR_COUNT      ("AcousticSensorCount")
#define ACOUSTIC_SENSOR_COLOR      ("AcousticSensorColor")
#define ACOUSTIC_SENSOR_SIZE       ("AcousticSensorSize")
#define ACOUSTIC_SENSOR_VISIBILITY ("AcousticSensorVisibility")
#define INTERPOLATION_TYPE         ("InterpolarionType")

#define MAX_NOISE_X                ("maxNoiseX")
#define MAX_NOISE_Y                ("maxNoiseY")
#define MAX_NOISE_Z                ("maxNoiseZ")




namespace BasicSettingValues {
    constexpr double BASIC_MODEL_LENGHT               = 10.0;
    constexpr int    BASIC_MODEL_COUNT                = 200;
    constexpr int    BASIC_MODEL_COLOR                = 1;
    constexpr double BASIC_MODEL_SIZE                 = 0.04;
    constexpr bool   BASIC_MODEL_VISIBILITY           = true;
    constexpr int    BASIC_POSITION_SENSOR_COUNT      = 5;
    constexpr int    BASIC_POSITION_SENSOR_COLOR      = 0;
    constexpr double BASIC_POSITION_SENSOR_SIZE       = 0.1;
    constexpr bool   BASIC_POSITION_SENSOR_VISIBILITY = true;
    constexpr bool   BASIC_POSITION_SENSOR_END        = false;
    constexpr int    BASIC_ACOUSTIC_SENSOR_COUNT      = 150;
    constexpr int    BASIC_ACOUSTIC_SENSOR_COLOR      = 2;
    constexpr double BASIC_ACOUSTIC_SENSOR_SIZE       = 0.05;
    constexpr bool   BASIC_ACOUSTIC_SENSOR_VISIBILITY = true;
    constexpr int    BASIC_INTERPOLATION_TYPE         = 0;

    constexpr double BASIC_MAX_NOISE_X                = 0.0;
    constexpr double BASIC_MAX_NOISE_Y                = 0.0;
    constexpr double BASIC_MAX_NOISE_Z                = 0.0;
}

#endif // SETTINGSDEFINE_H
