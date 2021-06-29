"""Apollo task"""
import numpy as np
from env import ApolloEnv
import random

#Start Env
env = ApolloEnv()
print()
print('###################')
print()
#Loop many times
for i in range(10):

  #Reset environment
  state = env.reset()

  #Some random action. Action is in range [-1, 1] which gets translated by the env to the min and max ranges for that particuarly action channel automatically (see env.py)
  action = [(2.0 * random.random()) - 1.0 for _ in range(2)]

  #Step into the env
  next_state, reward, done, info = env.step(action)

  print()
  print('###################')
  print()
  print('Iteration', i, 'Reward', reward)
  print()
  print('###################')
  print()


