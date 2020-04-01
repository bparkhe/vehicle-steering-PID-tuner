#include "PID.h"
#include<numeric>
#include<vector>

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {

  
  
  std::vector<double> p;
  std::vector<double> dp;
  std::vector<double> error_vec;
  double prev_cte;
  double perror;
  double derror;
  double ierror;
  int flag;
  

}

PID::~PID() {}

void PID::Init(double Kp, double Kd, double Ki) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */

  p.push_back(Kp);
  p.push_back(Kd); 
  p.push_back(Ki);
  dp = {0.1,0.2,0.0001};
  perror = 0;
  derror = 0;
  ierror = 0;
  prev_cte = 0;
  flag = 1;
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  perror = cte;
  derror = cte - prev_cte;
  
  if (error_vec.size()>200)
    error_vec.erase(error_vec.begin());
  
  error_vec.push_back(cte);
  ierror = accumulate(error_vec.begin(),error_vec.end(),0);
    
}


double PID::Steer(){
  double steer = -p[0]*perror - p[1]*derror - p[2]*ierror;
  if(steer>1) 
    steer =1;
  else if(steer <-1) 
    steer = -1;
  
  return steer;
}