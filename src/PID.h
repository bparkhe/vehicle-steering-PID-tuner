#include <vector> 
#ifndef PID_H
#define PID_H

class PID {
 public:
  /**
   * Constructor
   */
  PID();

  /**
   * Destructor.
   */
  virtual ~PID();

  /**
   * Initialize PID.
   * @param (Kp_, Ki_, Kd_) The initial PID coefficients
   */
  void Init(double Kp, double Kd, double Ki);

  /**
   * Update the PID error variables given cross track error.
   * @param cte The current cross track error
   */
  void UpdateError(double cte);

  /**
   * Calculate the total PID error.
   * @output The total PID error
   */
  
  double Steer();
  std::vector<double> p;
  std::vector<double> dp;
  double prev_cte;
  int flag;
  std::vector<double> error_vec;
  double perror;
  double ierror;
  double derror;
  
 private:
  /**
   * PID Errors
   */



  /**
   * PID Coefficients
   */ 
//   double Kp;
//   double Ki;
//   double Kd;
};

#endif  // PID_H