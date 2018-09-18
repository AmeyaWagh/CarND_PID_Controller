#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include "Utils.h"
#include <math.h>

// for convenience
using json = nlohmann::json;


int main()
{
  uWS::Hub h;

  // car_controller implements a higher level PID controller
  // for both steering as well as throttle

  // car_controller takes steer kp,ki,kd
  //                and   speed kp,ki,kd
  CarController car_controller(0.08,0.0005,1.00,
                               0.40,0.0020,15.8);

  h.onMessage([&car_controller](uWS::WebSocket<uWS::SERVER> ws,
              char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's
    // a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      // auto mode
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {

          // process the json string to generate action using controller
          json control_action = car_controller.processController(s);
          if(car_controller.isTelemetry()){
            auto msg = "42[\"steer\"," + control_action.dump() + "]";
            #ifdef debug
                std::cout << msg << std::endl;
            #endif
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

  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req,
                     char *data, size_t, size_t) {
    const std::string s = "<h1>PID Controller!</h1>";
    if (req.getUrl().valueLength == 1){
      res->end(s.data(), s.length());
    }
    else{
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](  uWS::WebSocket<uWS::SERVER> ws,
                        uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws,
                    int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });



  int port = 4567;
  if (h.listen(port)){
    std::cout << "Listening to port " << port << std::endl;
  }
  else{
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
