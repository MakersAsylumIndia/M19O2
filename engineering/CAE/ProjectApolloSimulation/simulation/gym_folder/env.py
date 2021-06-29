"""Apollo task"""
import numpy as np

from gym import core, spaces
from gym.utils import seeding

import psa2 as psa
import importlib

"""
For the control actions, are these ranges, data structure, and variable names correct?
 'input_orifice': [Range = 1.0, 3.5] Continous
  'vent_orifice', Range = [0.8,3.0] Continous
  'blowdown_orifice',[Range =1.0, 3.5] Continous
  'real_cycle_time': [Range =10, 20] Discrete
  'vent_time_fract': [Range = 0.60, 0.95] Continous

Farmed from the monte_sim.py
"""

class ApolloEnv(core.Env):

   
    def __init__(self):
        self.viewer = None

        #TODO VERIFY THAT THIS CONSTRAINT IS TRUE OR IF WE HAVE A LARGER RANGE TO CHOOSE FROM
        #Action Ranges
        self.min_action = np.array([10, 0.6])
        self.max_action = np.array([20, 0.95])
        self.action_space = spaces.Box(
            low=self.min_action,
            high=self.max_action,
        )
        #State/Observation Varible Range
        self.low_state = -float('inf')
        self.high_state = float('inf')
        self.observation_space = spaces.Box(
            low=self.low_state,
            high=self.high_state,
            shape = (7,),
            dtype=np.float32
        )

        #TODO Wrap PSA in a class such as to avoid the following hack
        #Defult values for params and norm needed to init_state
        self.params=importlib.import_module('params')
        dummy_action = get_dummy_action()
        self.params = self.params.create_param(dummy_action, lambda *args : None)
        self.norm = create_norm(self.params)

    def seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]

    def reset(self):
        state = psa.init(self.params, self.norm)
        return state 

    def step(self, action, outdir='test'):

        """
        Action is a list of 6 inputs that is in the continous range [-1.0, 1.0]
        This is constrained so that we can use a hyperbolic tangent output activation for our opolicy network.
        This is particualarly helpful for stochastic policies (Gaussain) often used in SAC
        """

        #TODO Clean this routine
        #Translate action (list) to override params dict
        #Translation is min + ([-1,1] --> [0,1]) * (mx - min)
        mods = AttrDict()
        mods.real_cycle_time= self.min_action[0] + (0.5 * (action[0]+1)) * (self.max_action[0] - self.min_action[0]) 
        mods.vent_time_fract= self.min_action[1] + (0.5 * (action[1]+1)) * (self.max_action[1] - self.min_action[1]) 

        #Hardcoded
        mods.cycles=31
        mods.real_vent_time=mods.vent_time_fract*mods.real_cycle_time
        mods.input_orifice=2.78
        mods.vent_orifice=1.56
        mods.blowdown_orifice=3.5
        
        #Simulate
        data, ret, param, pickle_name,out_place= psa.simulate(mods, 
                                                        outdir=outdir,
                                                        params_file='params', 
                                                        verbose=False)

        #TODO Verify if we care about prod_y or counter_y? Also [-1] refers to the last timestep but why [1]? We care about the middle cylinder?
        reward = ret.prod_y[-1][1] 

        #New state
        state = psa.init(self.params, self.norm)

        return state, reward, True, {}


    def render(self, mode='human'):
       #TODO Add a viz
       pass

    def close(self):
        pass

class AttrDict(dict):
  def __init__(self, *args, **kwargs):
    super(AttrDict, self).__init__(*args, **kwargs)
    self.__dict__ = self

def create_norm(param):
  norm=AttrDict()
  #The normalization values, from Table 1 in Supplementary Info for the Ref paper
  norm.P0=param.PH           # 300kPa      [Pa]
  norm.T0=293.15       #Room temp = 20C, [K]
  norm.qs0=param.qBs*1000   #The normalization amount mol/kg
  norm.xA=norm.qs0     # qs,0  [mol/kg]
  norm.xB=norm.qs0     # qs,0  [mol/kg]
  norm.v0=param.norm_v0            #  [m/s]
  norm.z0=param.L      # Length of cylinder [m]
  norm.t0=param.norm_t0      # so t=time/norm.t0
  #norm.t0=norm.z0/norm.v0   # so t=time/norm.t0
  #norm.alphaA=param.kA*param.L/norm.v0
  #norm.alphaB=param.kB*param.L/norm.v0
  return norm

def get_dummy_action():
  mods=AttrDict()
  mods.cycles=31
  mods.input_orifice=2.78
  mods.vent_orifice=1.56
  mods.blowdown_orifice=3.5
  mods.vent_time_fract=0.75
  mods.real_cycle_time=15
  mods.real_vent_time=mods.vent_time_fract*mods.real_cycle_time
  return mods

