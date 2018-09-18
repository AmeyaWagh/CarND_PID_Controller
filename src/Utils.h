#ifndef UTILS_H
#define UTILS_H
#include "json.hpp"
#include "PID.h"

using json = nlohmann::json;


// For converting back and forth between radians and degrees.
constexpr double pi();
double deg2rad(double x);
double rad2deg(double x);


std::string hasData(std::string s);


class CarController{
private:
    double cte;
    double speed_cte;
    double speed;
    double angle;
    double steer_value;
    double dynamic_throttle;
    PID pid;
    PID speed_pid;
    json control_msg;
    bool is_telemetry;
    double MAX_SPEED;

public:
    CarController(double Kp, double Ki, double Kd,
                  double Kp_speed, double Ki_speed, double Kd_speed);
    json processController(std::string socket_msg);
    bool isTelemetry();
    void clip_steer_value();
    void clip_throttle_value();
};

#endif
