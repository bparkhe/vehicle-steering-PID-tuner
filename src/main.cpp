#include <cmath>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  PID pid;
  /**
   * TODO: Initialize the pid variable.
   */
  pid.Init(0,0,0);
  int i=1, countm=0;
  double accum_cte =0,prev_accum_cte =100000,throttle = 0.1;
  
  
  
  h.onMessage([&pid,&i,&countm,&throttle,&accum_cte,&prev_accum_cte](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event

    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */
  
          accum_cte += cte*cte;
          countm++;
          
          if(accum_cte>prev_accum_cte && accum_cte>0.7){

            if(pid.flag == 1){
              pid.p[i] -= pid.dp[i];
              pid.flag = 2;
              throttle *= 0.7;
            }else{
              throttle *= 0.5;
              pid.dp[i] *= 0.9;
              pid.p[i] += pid.dp[i];
              pid.flag = 1;
              i++;
              i=i%3;
            }
            throttle = (throttle<0.1) ? 0.1: throttle;
            prev_accum_cte = accum_cte;
            accum_cte = 0;
            countm = 0;
            
          }else if(countm>10 && accum_cte>0.8){
            pid.dp[i] *= 1.1;
            i++;
            i = i%3;
            pid.p[i] += pid.dp[i];
            prev_accum_cte = accum_cte;
            accum_cte = 0;
            countm = 0;
            pid.flag = 1;
            throttle*=0.95;
            
          }else if(countm>10){
            throttle *=1.05;
            throttle = (throttle>0.25) ? 0.25 : throttle;
            pid.dp[i] *= 0.9;
            prev_accum_cte = accum_cte;
            accum_cte = 0;
            countm = 0;
          }
          
          
          pid.UpdateError(cte);
          steer_value = pid.Steer();
          pid.prev_cte = cte;


     
          // DEBUG

          std::cout<<countm <<" "<<i<<" "<<throttle<<" "<<pid.ierror<<" CTE: "<<cte<<" Kp "<<pid.p[0]<<" Kd "<< pid.p[1]<<" Ki "<<pid.p[2]<<std::endl;
          //std::cout << "CTE: " << cte << " Steering Value: " << steer_value 
            //        << std::endl;
	
          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}