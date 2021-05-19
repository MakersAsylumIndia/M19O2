#!/usr/bin/env python
#Copyright (c) 2018 Daniel B. Grunberg
#
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.

#Please use python 3.6 or newer
import sys
import math
import collections
import matplotlib.pyplot as plt
import numpy as np
import scipy
import matplotlib
import termios
import tty
import pprint
import argparse
import time
import datetime
import os
import random

vinfo=sys.version_info
print('version:{}'.format(vinfo))
assert vinfo.major >= 3 and vinfo.minor>=6
print('plt {}'.format(matplotlib.__version__))
#NOTE: matplotlib 3.2 needs python 3.6, at least for easy pip install
'''
This performs a search over multiple parameters for best performance
of a PSA Oxygen generator.

Monte-carlo random parameter search

See README for more details.
'''
#Local modules
#import params
import util
import psa

np.set_printoptions(precision=5, edgeitems=6, linewidth=90, suppress=False)
pp=pprint.PrettyPrinter(indent=2)

parser = argparse.ArgumentParser(
    description="PSA Search program",
    epilog='''
    log and csv file name default to name of the directory, unless otherwise specified
    will open sqllite database with sqllitedict to store results
database not implemented yet
''')

parser.add_argument("--outdir", type=str, help="output directory and log file name", default='./monte')
parser.add_argument("--ofile", type=str, help="file to place results, will add (.log, .csv)", default=None)
options = parser.parse_args()

last_path=os.path.basename(os.path.normpath(options.outdir))
output_filename=last_path
logfile=output_filename+'.log'
#if os.path.exists(logfile):
#  print('file {} already exists, use different --outdir'.format(logfile))
#  sys.exit(1)
  
print('git revision:{}'.format(util.get_git_commit()))

class AttrDict(dict):
  def __init__(self, *args, **kwargs):
    super(AttrDict, self).__init__(*args, **kwargs)
    self.__dict__ = self
    
def randrange(a,b,s):
  #return a random number from a to b but with step size s
  N=(b-a)/s
  return random.randint(0,N)*s+a

#Here are the parameter values we will be searching over
#real_vent_time is a fraction on the real_cycle_time
#the numbers are min, max and step size
vals=[
  ['input_orifice',1.0,3.5,0.1],
  ['vent_orifice', 0.8,3.0,0.1],
  ['blowdown_orifice',1.0,3.5,0.1],
  ['real_cycle_time',10,20,0.5],
  ['vent_time_fract',.60,.95,.01]]

random.seed()
outdir=options.outdir
roi=None
best_mods=None
best_fom=0
print('writing output to {} and in directory {}'.format(logfile,outdir))
fp=open(logfile,'at')
while True:
  #pick a random value
  mods=AttrDict()
  mods.cycles=9
  for x in vals:
    name=x[0]
    mods[name]=randrange(x[1],x[2],x[3])
  #compute the vent time
  mods.real_vent_time=mods.real_cycle_time*mods.vent_time_fract
  print(mods)
  #Simulate
  ret,param=psa.simulate_and_plot(mods,
                                  do_plots=True,
                                  pause=False,
                                  outdir=outdir,
                                  params_file='params',
                                  roi=roi)
  fom=ret.container_y[-1][1]
  txt=''
  if fom>best_fom:
    best_fom=fom
    txt='IMPROVED'
  fp.write('{} yAN={} mods={}\n'.format(txt,fom,mods))
  fp.flush()
    
    
