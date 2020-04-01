
## PID Controller Project

The goals / steps of this project are the following:

* Implement a program to steer vehicle to the center
* Arrive at appropriate Kp, Kd, Ki coefficients using Twiddle
* Manage the speed of the vehicle with respect to the road trajectory possibly using PID control

[//]: ## (Image References)

[video1]: ./pid_video_output.mp4 "Output Video"


#### Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  
[Rubric](https://review.udacity.com/#!/rubrics/1972/view)

---

### Writeup / README

#### 1. Implementation: The PID procedure follows what was taught in the lessons.

##### Parameter Search
For my implementation, I followed the twiddle algorithm to arrive at the appropriate gains.

The error(cte) is accumulated over 10 readings in accum_cte and compared with the previous accumulation prev_accum_cte.

If accumulated error is less than previous error, increase dp by 10% and move to next parameter to add its dp from some previous calculation.
If accumulated error is greater than prev_error, take off the previous dp addition.
If it is still greater add 0.9*dp and move to the next parameter.

##### Storing errors for coefficients
For calculating steer angle we need the error, its derivative and its integral.
perror is set as the original error cte. derror which is the derivative is the difference between current and previous cte. The integrated error is calculated by summing errors over the last 200 steps. It is done by adding new errors to the top of a stack while removing values at the bottom from 200 time steps in the past.

##### For Calculating Steer Angle
The PID equation is used for calculating the steer angle. Since the range of the steer value is -1 to 1 anything outside the range is rounded to the nearest limit.


#### 2. Reflection
##### Role of P,I and D components.
The proportional component of the equation drives the change in steering angle proportional to the error. However, it does not reorient itself as the error goes on reducing.
The derivative component helps with this by countering steer component in the opposite direction. This slows down the aggressive turn due to the proportional component and reorients the car as it goes closer to the center.
The integral error removes the steady state offset in the car's trajectory.

The speed of the car plays a major role in this since a huge derivative error can lead to the car spiralling out of control. in this case it is essential to slow down so that the proportional component can takeover and stabilize it.

##### Arriving at the appropriate P,I and D coefficients
The initial parameter values in the p vector were set to 0. The step size for the search was 0.1 for the Kp and Kd components and 0.0001 for the Ki component because a huge initial value will destabilize the vehicle.
The approximate values of the parameters were Kp=0.4, Kd = 1.01, Ki = 0.00033

The speed values ranged from 10mph on the turns to ~15mph on straight stretches.

##### Final Parameters 
The vehicle did fairly well to avoid going over the lines and stayed relatively stable except for one sharp turn where the speed is to high going into the turn. This can be handled by the vision component which can see and upcoming turn and slow down. 

#### 3. Simulation

The implementation of the algorithm for one lap can be seen in the output_video.mp4 in the main directory.
![alt text][video1]
