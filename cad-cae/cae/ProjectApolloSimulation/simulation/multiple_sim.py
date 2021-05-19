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

vinfo=sys.version_info
print('version:{}'.format(vinfo))
assert vinfo.major >= 3 and vinfo.minor>=6
print('plt {}'.format(matplotlib.__version__))
#NOTE: matplotlib 3.2 needs python 3.6, at least for easy pip install
'''
This performs multiple simulations
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
''')

parser.add_argument("--outdir", type=str, help="output directory", default='./')
parser.add_argument("--ofile", type=str, help="file to place results, will add (.log, .csv)", default=None)
parser.add_argument('-t',"--test", action='store_true', help='do not store results to database')
parser.add_argument('-c',"--cycles", type=int, help='number of cycles',default=19)
parser.add_argument('-v',"--verbose", action='store_true', help='verbose debug info')
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
  
print('git revision:{}'.format(util.get_git_commit()))
class AttrDict(dict):
  def __init__(self, *args, **kwargs):
    super(AttrDict, self).__init__(*args, **kwargs)
    self.__dict__ = self
    
for i in range(2):
  mods=AttrDict()
  if i==0:
    mods.L=1
    mods.D=.0508
    mods.feed_pressure=3.0
  elif i==1:
    mods.L=0.33
    mods.D=.0886
    mods.feed_pressure=3.0
  mods.cycles=options.cycles
  print('doing {}'.format(mods))
  ret,param=psa.simulate(mods,plots=True,pause=False,
                         outdir=options.outdir,
                         verbose=options.verbose)
