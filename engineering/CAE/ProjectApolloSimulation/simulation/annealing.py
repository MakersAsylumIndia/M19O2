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
import termios
import tty
import pprint
import argparse
import time
import datetime
import os
import random
import copy

vinfo=sys.version_info
print('version:{}'.format(vinfo))
assert vinfo.major >= 3 and vinfo.minor>=6
'''
This performs a search over multiple parameters for best performance
of a PSA Oxygen generator.

Uses a simulated annealing technique or threshold accepting

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

parser.add_argument("--outdir", type=str, help="output directory and log file name", default='./annealing')
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

def limit_point(val, stats):
  #given a list of the values in the vals structure, makes sure they are within limits
  ret=copy.copy(val)
  for i, (v, x) in enumerate(zip(val, stats)):
    if v < x[1]:
      ret[i]=x[1]
    if v > x[2]:
      ret[i]=x[2]
  return ret

#Here are the parameter values we will be searching over
#real_vent_time is a fraction on the real_cycle_time
#the numbers are min, max and step size
stats=[
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
fom=0
prev_fom=0
print('writing output to {} and in directory {}'.format(logfile,outdir))
fp=open(logfile,'at')
#current=[2.6,1.3,3.1,17,.62]
#current=[3.1,1.4,3.5,19.5,.62]  #from monte carlo, gave us 0.829
current=[3.07283,1.24332,3.5,20.0,.6286679]  #from SA, gave us 0.8396
#now we take random steps
count=0
#want T0 to give us 80% chance of taking the average neg improvement,
#determined from initial simulations
avg_neg_improve=-.0065
T0=-abs(avg_neg_improve)/math.log(0.80)
print('Starting with T0 {}'.format(T0))
T=T0  # temperature
neg_imp_tot=0
neg_imp_count=0
reduct=0.65  # fraction to reduce the SD of the gaussian dist
while True:
  #pick a random delta from current position
  mods=AttrDict()
  step=[]
  for x in stats:
    step.append(random.gauss(0, x[3]*reduct))   # standard dev of is step size
  new_point=[a+b for a,b in zip(step, current) ]
  print('new_point = {}'.format(new_point))
  new_point=limit_point(new_point,stats)
  print('after limits new_point = {}'.format(new_point))
  mods.cycles=9
  for v,x in zip(new_point,stats):
    name=x[0]
    mods[name]=v
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
  improve=fom-prev_fom  # a bad step could be .92 to .91
  if fom>best_fom:
    best_fom=fom
    txt='IMPROVED'
  fp.write('{} yAN={:8.6f} improve={:8.6f} T={:6.3f} mods={}\n'.format(txt,fom,improve,T,mods))
  fp.flush()
  if count>0 and count%25==0:
    T=T*.90
  if improve<0:
    neg_imp_tot+=improve
    neg_imp_count+=1
  if improve>0:
    current=new_point
    print('improvement {} taking new improved point {}'.format(improve,current))
    prev_fom=fom
  elif random.random()< math.exp(-abs(improve)*10/T):
    print('improvement {} taking anyway {}'.format(improve,current))
    current=new_point
    prev_fom=fom
    #accept it anyway
  else:
    print('improvement {} keeping old point {}'.format(improve,current))
    #don't change prev_fom
  if neg_imp_count>0:
    print('after {} neg impr, mean {}'.format(neg_imp_count,
                                              neg_imp_tot/neg_imp_count))
  count+=1
    
    
