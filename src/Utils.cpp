#include "Utils.h"
#include "json.hpp"

using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() {
    return M_PI;
}

double deg2rad(double x) {
    return x * pi() / 180;
}

double rad2deg(double x) {
    return x * 180 / pi();
}

// check if json string has data
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}


CarController::CarController(double Kp, double Ki, double Kd,
                             double Kp_speed, double Ki_speed, double Kd_speed){
    steer_pid.Init(Kp,Ki,Kd);
    speed_pid.Init(Kp_speed,Ki_speed,Kd_speed);
    MAX_SPEED = 0.5;
}

json
CarController::processController(std::string socket_msg){

    auto j = json::parse(socket_msg);
    is_telemetry=false;
    std::string event = j[0].get<std::string>();
    if (event == "telemetry") {
        is_telemetry = true;

        // read inputs
        cte = std::stod(j[1]["cte"].get<std::string>());
        speed = std::stod(j[1]["speed"].get<std::string>());
        angle = std::stod(j[1]["steering_angle"].get<std::string>());

        // update error for steering pid controller
        steer_pid.UpdateError(cte);
        steer_value = -1*steer_pid.TotalError();

        // update throttle pid controller
        // desired speed is MAX_SPEED*cos(deg2rad(angle))
        // added some bias to make the car move and not start-stop
        // near zero speed
        speed_cte = speed - MAX_SPEED*cos(deg2rad(angle))+0.08;
        speed_pid.UpdateError(speed_cte);
        dynamic_throttle = speed_pid.TotalError();

        // clip values to operational limits
        clip_steer_value();
        clip_throttle_value();


        // log values to stdout
        printf("[CTE] %2.3f [Steering] %2.3f\n",cte,steer_value);

        // populate control message
        control_msg["steering_angle"] = steer_value;
        control_msg["throttle"] = dynamic_throttle;

    }
    return control_msg;
}

void
CarController::clip_steer_value(){
    // limit steer_value in bounds
    if(steer_value > 1.0){
        steer_value=1.0;
    }
    else if(steer_value < -1.0){
        steer_value=-1.0;
    }
}

void
CarController::clip_throttle_value(){
    // limit throttle from -0.05(some brake)
    // to MAX_SPEED
    if(dynamic_throttle > MAX_SPEED){
        dynamic_throttle = MAX_SPEED;
    }
    else if(dynamic_throttle< 0){
        dynamic_throttle = -0.05;
    }
}

bool
CarController::isTelemetry(){
    // check if message is telemetry
    return is_telemetry;
}
