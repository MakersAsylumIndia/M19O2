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
import itertools
#from sqlite import SqliteDict

vinfo=sys.version_info
print('version:{}'.format(vinfo))
assert vinfo.major >= 3 and vinfo.minor>=6
print('plt {}'.format(matplotlib.__version__))
#NOTE: matplotlib 3.2 needs python 3.6, at least for easy pip install
'''
This performs a search over multiple parameters for best performance
of a PSA Oxygen generator.

See README for more details.
'''
#Local modules
#import params
import difference
import mylog
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

parser.add_argument("--outdir", type=str, help="output directory", default='./')
parser.add_argument("--ofile", type=str, help="file to place results, will add (.log, .csv)", default=None)
parser.add_argument('-t',"--test", action='store_true', help='do not store results to database')
parser.add_argument('-c',"--cycles", type=int, help='number of cycles',default=19)
#parser.add_argument("-N", type=int, help='number of spatial cells', default=None)
#parser.add_argument("-s", '--show', action='store_true', help='interactive - display charts')
#parser.add_argument("--mode", type=int, help='1=UDS,3=vanleer,4=weno', default=1)
#parser.add_argument("--adsorb", dest='adsorb', help='adsorb on ',action='store_true')
#parser.add_argument("--no-adsorb", dest='adsorb', help='adsorb on ',action='store_false')
#parser.set_defaults(adsorb=True)
options = parser.parse_args()

last_path=os.path.basename(os.path.normpath(options.outdir))
if options.ofile is None:
  output_filename=last_path
else:
  output_filename=options.ofile
logfile=output_filename+'.log'
csvfile=output_filename+'.csv'
if os.path.exists(logfile):
  print('file {} already exists, use different --outdir'.format(logfile))
  sys.exit(1)
  
#if not options.test:
#  mydb=SqliteDict('mydb.sqlite',autocommit=True)
  
print('git revision:{}'.format(util.get_git_commit()))

class AttrDict(dict):
  def __init__(self, *args, **kwargs):
    super(AttrDict, self).__init__(*args, **kwargs)
    self.__dict__ = self
    

best_fom=0
best_mods=None
best_ret=None
ofile=open(logfile,'wt')
data=[]   # store the results so we can sort them later list of [mods, res]
#orifice=[1.6, 1.8, 2.0, 2.2, 2.4, 2.6]
orifice=[1.7,2.0]
cycles=options.cycles
vo=[1.0,1.5,2.0]
bdo=[2.8]
rp=[.70,1.00,1.41] # particle radius
feed=[2.6,3.0,3.5]

ct=[20,24,28,32,36]
vtf=[0.60,0.64,0.68]  # of the ct
lens=[.33,1.0]
count=0
#z=[ct, vtf, vo, orifice,bdo]
z=[ct,feed,vtf,lens]
lengths=[len(x) for x in z]
total=np.prod(lengths)
#Do cartesian product of list z - all choices of the variables, orifice iterating
#most rapidly
for x in itertools.product(*z):
  cycle_time,feed,vent_time_fraction,l = x
  vent_time=vent_time_fraction*cycle_time
  if vent_time >= cycle_time:
    continue
  mods=AttrDict()
  #mods.blowdown_orifice=blowdown_orifice
  #mods.input_orifice=input_orifice
  mods.product_orifice=0.30
  mods.cycle_time=cycle_time
  mods.vent_time=vent_time
  mods.feed_pressure=feed
  #mods.vent_orifice=vent_orifice
  mods.cycles=cycles
  mods.L=l
  if l>.99:
    mods.D=.0508
  else:
    mods.D=.0886
  print(mods)
  #print('doing {}'.format(mods))
  ret,param=psa.simulate(mods,plots=True,pause=False,outdir=options.outdir)
  #pickle={'time'=datetime.datetime.now(),'ret':ret,'param':param}
  #if not options.test:
  #  mydb[pickle['time']]=pickle
  #fom=ret['prod_y'][2]  # average y output
  fom=ret['container_y'][-1][2]  # max value in the output side of the container
  data.append([mods,ret])
  out=''
  if fom > best_fom:
    out='*'
    best_fom=fom
    best_mods=mods
    best_ret=ret
  res_str='{}/{}) {:6.3f} {} {}'.format(count,total,fom, out, {**mods,**ret})
  ofile.write(res_str + '\n')
  ofile.flush()
  print(res_str)
  count+=1
#sort
ofile.close()
data.sort(reverse=True, key=lambda x: x[1]['prod_y'][2])
#make a list of the values we want to print after prod_y and logcode
show=['input_orifice',
      'blowdown_orifice',
      'vent_orifice',
      'real_cycle_time',
      'real_vent_time',
      'cycle_time',
      'vent_time',
      'norm_t0',
      'avg_prod_pressure',
      'container_y_max',
      'container_y_avg']

with open(csvfile,'wt') as ofile:
  ofile.write('prod_y N,logcode,{}\n'.format(','.join(show)))
  for el in data:
    z={**el[0], **el[1]}
    #add in special ones
    z['real_cycle_time']=z['cycle_time']*z['norm_t0']
    z['real_vent_time']=z['vent_time']*z['norm_t0']
    z['avg_prod_pressure']=z['prod_pressure'][-1][2]
    z['container_y_max']=z['container_y'][-1][1]
    z['container_y_avg']=z['container_y'][-1][2]
    ofile.write('{:8.3f},{}'.format(z['prod_y'][-1][2],z['logcode']))
    for x in show:
      ofile.write(',{:8.3f}'.format(z[x]))
    ofile.write('\n')

#if not options.test:
#  mydb.close()
