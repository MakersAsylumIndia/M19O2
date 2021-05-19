from bonsai_ai import Brain, Config
from bonsai_gym import GymSimulator
import numpy as np

from gym import core, spaces
from gym.utils import seeding
import sys
import util
import psa2 as psa
import importlib
from gym_folder.env import ApolloEnv
import gym
import time

class BonsaiApollo(GymSimulator):

    use_string = False
    environment = ApolloEnv
    simulator_name = 'ApolloSimulator'    # name of the simulation in the inkling file
    environment_name = 'ApolloEnv'  # name of the OpenAI Gym environment

    def __init__(self, brain, iteration_limit=0, skip_frame=1):
        """ initialize the GymSimulator with a bonsai.Config,
            the class variables will be used to setup the environment
            and simulator name as specified in inkling
        """
        super(GymSimulator, self).__init__(brain, self.simulator_name)

        # create the gym environment
        if (self.use_string):
            self._env = gym.make(self.environment_name)
        else:
            self._env = self.environment()

        # parse optional command line arguments
        cli_args = self._parse_arguments()
        if cli_args is None:
            return

        # optional parameters for controlling the simulation
        self._headless = cli_args.headless
        self._iteration_limit = iteration_limit    # default is no limit
        self._skip_frame = skip_frame    # default is to process every frame

        # random seed
        self._env.seed(20)
        self._env.reset()

        # book keeping for rate status
        self._log_interval = 10.0  # seconds
        self._last_status = time.time()

    def gym_to_state(self, observation):
        state = {'P': observation['P'][-1],
                 'T': observation['T'][-1],
                 'Tw':    observation['Tw'][-1],
                 'xA': observation['xA'][-1],
                 'xB': observation['xB'][-1],
                 'yA': observation['yA'][-1],
                 'yB': observation['yB'][-1],
                 }
        return state

    def action_to_gym(self, action):
        return [action['real_cycle_time'], action['vent_time_fract']]



if __name__ == '__main__':
    # create a brain, openai-gym environment, and simulator
    config = Config(sys.argv)
    brain = Brain(config)
    sim = BonsaiApollo(brain)
    sim.run_gym()
