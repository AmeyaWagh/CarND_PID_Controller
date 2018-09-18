#include "Utils.h"
#include "json.hpp"

using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }



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

//class
CarController::CarController(double Kp, double Ki, double Kd,
                             double Kp_speed, double Ki_speed, double Kd_speed){
    pid.Init(Kp,Ki,Kd);
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
        pid.UpdateError(cte);
        steer_value = -1*pid.TotalError();

        // update throttle pid controller
        speed_cte = speed - MAX_SPEED*cos(deg2rad(angle))+0.1;
        speed_pid.UpdateError(speed_cte);
        dynamic_throttle = speed_pid.TotalError();

        // clip values to operational limits
        clip_steer_value();
        clip_throttle_value();


        // log values to stdout
//        std::cout << "[CTE]" << cte << " [Steering Value]" << steer_value << std::endl;
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
    if(dynamic_throttle > MAX_SPEED){
        dynamic_throttle=MAX_SPEED;
    }
    else if(dynamic_throttle< 0){
        dynamic_throttle = -0.05;
    }
}

bool
CarController::isTelemetry(){
    return is_telemetry;
}
