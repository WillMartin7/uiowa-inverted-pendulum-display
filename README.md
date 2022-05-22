# University of Iowa Inverted Pendulum Display

Work completed for ME:4086- Senior Design Project at the University of Iowa

--- 

### Documentation

- [`documentation.pdf`](./documentation.pdf) : Details on dynamics model, controller design, and mechanical and electronical system

### Simulation Code

- [`inverted_pendulum_control_simulations.ipynb`](./simulation_code/inverted_pendulum_control_simulations.ipynb): Simulation of nonlinear and linear controllers using ode solver

### Hardware Code

- [`homing_sequence.ino`](./hardware_code/homing_sequence/homing_sequence.ino): Arduino code to send pendulum cart to center of track using limit switches
- [`read_encoder.ino`](./hardware_code/read_encoder/read_encoder.ino): Arduino code to read the encoder connected to the Arduino. Additionally, numerically differentiates the encoder reading and applies mean filter.
- [`cart_acceleration_test.ino`](./hardware_code/cart_acceleration_test/cart_acceleration_test.ino): Arduino code to send control signal u = cart acceleration = _A *sin(w*t)_ with a given amplitude _A_ and frequency _w_.
