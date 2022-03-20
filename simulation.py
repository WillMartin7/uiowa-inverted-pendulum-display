# https://github.com/openai/gym/blob/master/gym/envs/classic_control/cartpole.py
# file:///Users/willmartin/Downloads/sharif2013(1)-1.pdf
# https://raw.githubusercontent.com/nielsSkov/masterThesisReport/master/report/thesisNielsSkovVestergaard.pdf

import numpy as np
import gym
import math

def apply_state_controller(state_vector, linear):

    x, x_dot, theta, theta_dot = state_vector

    E_0 = 0
    E = (1/2)*J*(theta_dot**2)+mass_pendulum*g*L*(math.cos(theta)-1)
    omega = mass_pendulum*L*theta_dot*math.cos(theta)*(E_0-E)

    eta = 5.4
    K_linear_bound = np.asarray([4.0, 6.0])
    u = -eta*np.sign(omega)
    u -= K_linear_bound@[x, x_dot]

    if linear:
        K_linear = np.array([-56.83602134, -10.38689076,  -3.16227766,  -4.93567592])
        u = float(-K_linear@np.array([[theta], [theta_dot], [x], [x_dot]]))

    if u > 0:
        return 1, abs(u) # if force > 0 -> move cart right
    else:
        return 0, abs(u) # if force <= 0 -> move cart left

def run_env():
    env = gym.make('CartPole-v1')
    env.env.seed(1)
    env.env.theta_threshold_radians = 100 # arbitarily high
    # env.env.x_threshold  = 10
    env.env.masscart = mass_cart
    env.env.masspole = mass_pendulum
    env.env.length = L
    env._max_episode_steps = 10000

    obs = env.reset()
    env.env.state = (0,0,math.pi,0)

    linear = False

    for i in range(10000):
        env.render()

        ang = env.env.state[2] - (env.env.state[2]//math.pi)*math.pi*2
        if (ang < math.pi/9) and (ang > -math.pi/9):
            linear = True

        # get force direction (action) and force value (force)
        action, force = apply_state_controller(obs, linear)
        env.env.force_mag = force
        obs, _, done, _ = env.step(action)

        if done:
            print(f'Terminated after {i+1} iterations.')
            break

    env.close()

if __name__ == "__main__":
    mass_cart = 2
    mass_pendulum = .1
    L = .25
    J = mass_pendulum*(L**2)
    g = 9.81
    linear_thesh = math.pi/9

    run_env()
