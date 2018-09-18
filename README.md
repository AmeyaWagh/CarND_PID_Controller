# PID Controller
Self-Driving Car Engineer Nanodegree Program

---

Video

## PID Controller
The Proportional Integral derivative controller is a highly robust controller used
in large number of control systems applications.

![PID_controller](https://upload.wikimedia.org/wikipedia/commons/thumb/4/43/PID_en.svg/400px-PID_en.svg.png)

where
`r(t)` is the input/desired state

`y(t)` is the current state

`u(t)` is the control signal

`e(t) = r(t)-y(t)` is the error in states

Following is the PID equation

![PID_equations](http://wiki.theuavguide.com/images/thumb/7/7d/PIDController_Equation.png/300px-PIDController_Equation.png)

Here we need to tune `Kp`, `Ki`, `Kd` to achieve a step response

![pid_response](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a3/PID_varyingP.jpg/320px-PID_varyingP.jpg)

checkout [wikipedia](https://en.wikipedia.org/wiki/PID_controller) for more detail

## Code
This code talks with the Udacity Simulator over webSocket over port 1234

### `src/main.cpp`
This is the main file which has web Socket handlers and creates a client and connects to the simulator

### `src/Utils.cpp`
This file contains util functions and a *CarController* class.
The *CarController* class implements two pid controllers, one for steering and one
for throttle. It takes a json object and parses the states obtained from the simulator.
the *processController* is a higher level controller method which uses both the pid
controllers to generate a `u(t)` as a json.

### `src/PID.cpp`
This file implements a simple pid controller which is then used by CarController.
