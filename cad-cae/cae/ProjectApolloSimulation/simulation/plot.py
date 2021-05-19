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
import importlib

vinfo=sys.version_info
print('version:{}'.format(vinfo))
assert vinfo.major >= 3 and vinfo.minor>=6
print('plt {}'.format(matplotlib.__version__))
#NOTE: matplotlib 3.2 needs python 3.6, at least for easy pip install
'''
Single simulation
'''

'''
This is the command line interface that calls the simulation in psa.py
'''
#Local modules
#import params_farooq as params
#import params
import difference
import mylog
import util
import pickle

import plots

np.set_printoptions(precision=5, edgeitems=6, linewidth=90, suppress=False)
plt.ion()
pp=pprint.PrettyPrinter(indent=2)

def parseIntRange(string):
  if '-' in string:
    x=string.split('-')
    return (int(x[0]),int(x[1]))
  else:
    return int(string)

parser = argparse.ArgumentParser(
    description="Plots for Pressure Swing Adsorption Oxygen Concentration simulation",
    epilog='''
''')

parser.add_argument("files", nargs='+',type=str, help="pickle files to plot", default=None)
parser.add_argument('-c', '--cycles', type=parseIntRange, help="cycle or range of cycles to plot, .e.g. 4 or 0-2", default=None)
parser.add_argument('-n', '--nopause', action='store_true', help="do not pause after generating plots")
options = parser.parse_args()

print('git revision:{}'.format(util.get_git_commit()))

  
class AttrDict(dict):
  def __init__(self, *args, **kwargs):
    super(AttrDict, self).__init__(*args, **kwargs)
    self.__dict__ = self

for file in options.files:
  with open(file, 'rb') as fp:
    data=pickle.load(fp)
  print('loaded {}'.format(file))
  print('cyles {}'.format(options.cycles))
  head,tail=os.path.split(file)
  print('head={} tail={}'.format(head,tail))
  #take the part up to the dash in tail, and add something
  parts=tail.split('-')
  out_place=os.path.join(head,parts[0]+'-replot')
  print('out_place={}'.format(out_place))
  plots.plot(data, out_place=out_place, pause=not options.nopause,
             cycles=options.cycles, text=parts[0])

