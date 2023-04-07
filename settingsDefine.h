#ifndef SETTINGSDEFINE_H
#define SETTINGSDEFINE_H

#define SETTING_FIRST_GROUP       ("Settings")
#define SENSOR_COUNT              ("SensorCount")
#define SENSOR_POINT_SIZE         ("SensorPointSize")
#define SENSOR_POINT_COLOR        ("SensorPointColor")
#define INTERPOLATION_POINT_SIZE  ("InterpolationPointSize")
#define INTERPOLATION_POINT_COLOR ("InterpolationPointColor")
#define INTERPOLATION_COUNT       ("InterpolationCount")
#define MAX_DEVIATION             ("MaxDeviation")
#define ANTENNA_LENGTH            ("AntennaLength")
#define ANTENNA_VISIBILITY        ("AntennaVisibility")
#define SENSOR_VISIBILITY         ("SensorVisibility")
#define INTERPOLATION_VISIBILITY  ("InterpolationVisibility")
#define MODEL_COUNT               ("ModelCount")
#define MODEL_POINT_SIZE          ("ModelPointSize")
#define MODEL_POINT_COLOR         ("ModelPointColor")


namespace BasicSettingValues {
    constexpr int BASIC_SENSOR_COUNT = 4;
    constexpr int BASIC_SENSOR_POINT_COLOR = 0;
    constexpr int BASIC_MODEL_COUNT = 20;
    constexpr int BASIC_MODEL_POINT_COLOR = 0;
    constexpr int BASIC_INTERPOLATION_POINT_COLOR = 0;
    constexpr int BASIC_INTERPOLATION_COUNT = 20;
    constexpr double BASIC_ANTENNA_LENGHT = 10.0f;
    constexpr double BASIC_SENSOR_POINT_SIZE = 0.1f;
    constexpr double BASIC_INTERPOLATION_POINT_SIZE = 0.05f;
    constexpr double BASIC_MODEL_POINT_SIZE = 0.04f;
    constexpr bool BASIC_ANTENNA_VISIBILITY = true;
    constexpr bool BASIC_SENSOR_VISIBILITY = true;
    constexpr bool BASIC_INTERPOLATION_VISIBILITY = true;
}

#endif // SETTINGSDEFINE_H
