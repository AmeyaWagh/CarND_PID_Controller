# PID Controller
Self-Driving Car Engineer Nanodegree Program

---


![pid_gif](./docs/giphy.gif)




[youtube_link](https://www.youtube.com/watch?v=vJW2Oj18J2g&feature=youtu.be)




## PID Controller
The Proportional Integral derivative controller is a highly robust controller used
in large number of control systems applications.

![PID_controller](./docs/PID_eqn.png)

where
`r(t)` is the input/desired state

`y(t)` is the current state

`u(t)` is the control signal

`e(t) = r(t)-y(t)` is the error in states

Following is the PID equation

![PID_equations](./docs/eqn.png)

Here we need to tune `Kp`, `Ki`, `Kd` to achieve a step response

![pid_response](./docs/response.jpg)

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
