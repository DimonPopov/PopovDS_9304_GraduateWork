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

namespace BasicSettingValues {
    constexpr int BASIC_SENSOR_COUNT = 4;
    constexpr int BASIC_SENSOR_POINT_COLOR = 0;
    constexpr int BASIC_INTERPOLATION_POINT_COLOR = 0;
    constexpr int BASIC_INTERPOLATION_COUNT = 20;
    constexpr double BASIC_ANTENNA_LENGHT = 10.0f;
    constexpr double BASIC_SENSOR_POINT_SIZE = 0.3f;
    constexpr double BASIC_INTERPOLATION_POINT_SIZE = 0.3f;
    constexpr bool BASIC_ANTENNA_VISIBILITY = false;
    constexpr bool BASIC_SENSOR_VISIBILITY = false;
    constexpr bool BASIC_INTERPOLATION_VISIBILITY = false;
}

#endif // SETTINGSDEFINE_H
