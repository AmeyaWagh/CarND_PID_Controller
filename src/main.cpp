#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include "Utils.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

//// For converting back and forth between radians and degrees.
//constexpr double pi() { return M_PI; }
//double deg2rad(double x) { return x * pi() / 180; }
//double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
//std::string hasData(std::string s) {
//  auto found_null = s.find("null");
//  auto b1 = s.find_first_of("[");
//  auto b2 = s.find_last_of("]");
//  if (found_null != std::string::npos) {
//    return "";
//  }
//  else if (b1 != std::string::npos && b2 != std::string::npos) {
//    return s.substr(b1, b2 - b1 + 1);
//  }
//  return "";
//}

int main()
{
  uWS::Hub h;

//  PID pid;
  // TODO: Initialize the pid variable.
//  pid.Init(0.15,0.0002,3.8);

  CarController car_controller(0.2,0.0005,3.0,
                               0.15,0.0002,4.8);

  h.onMessage([&car_controller](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
          json control_action = car_controller.processController(s);
          if(car_controller.isTelemetry()){
            auto msg = "42[\"steer\"," + control_action.dump() + "]";
            std::cout << msg << std::endl;
            ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          }
      }
      else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
