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
Using the params parameter file, trying to simulate our model PSA
'''

'''
This is the command line interface that calls the simulation in psa.py
'''
#Local modules
import orifice
#import params_farooq as params
#import params
import difference
import mylog
import util
import psa

np.set_printoptions(precision=5, edgeitems=6, linewidth=90, suppress=False)
plt.ion()
pp=pprint.PrettyPrinter(indent=2)

parser = argparse.ArgumentParser(
    description="Pressure Swing Adsorption Oxygen Concentration simulation",
    epilog='''
''')

parser.add_argument("-t", "--time", type=float, help="time to end sim", default=None)
parser.add_argument("-c", "--cycles", type=float, help="number of cycles", default=None)
parser.add_argument("-N", type=int, help='number of spatial cells', default=None)
parser.add_argument("--stdout", action='store_true', help='send debug output to stdout')
parser.add_argument("-s", '--show', action='store_true', help='interactive - display charts')
parser.add_argument("--mode", type=int, help='1=UDS,3=vanleer,4=weno', default=1)
parser.add_argument("--adsorb", dest='adsorb', help='adsorb on ',action='store_true')
parser.add_argument("--no-adsorb", dest='adsorb', help='adsorb on ',action='store_false')
parser.set_defaults(adsorb=True)
parser.add_argument("--cycle_time", type=float, help='cycle time',default=None)
parser.add_argument("--real_cycle_time", type=float, help='real cycle time',default=None)
parser.add_argument("--vent_time", type=float, help='vent time',default=None)
parser.add_argument("--real_vent_time", type=float, help='vent time',default=None)
options = parser.parse_args()

if options.time is not None and options.cycles is not None:
  sys.stdout.write('Please use only 1 of --time and --cycles')
  sys.exit(1)

outdir='Nelder-Mead2'
if options.stdout:
  outdir=None
  
print('options=',options)
print('git revision:{}'.format(util.get_git_commit()))

class AttrDict(dict):
  def __init__(self, *args, **kwargs):
    super(AttrDict, self).__init__(*args, **kwargs)
    self.__dict__ = self

mods=AttrDict()
mods=AttrDict()
mods.cycles=31
mods.input_orifice=2.78
mods.vent_orifice=1.56
mods.blowdown_orifice=3.5
mods.vent_time_fract=0.75
mods.real_cycle_time=15
mods.real_vent_time=mods.vent_time_fract*mods.real_cycle_time
roi=None
pars={**options.__dict__,**mods}
ret=psa.simulate_and_plot(pars,
                          do_plots=True,
                          pause=options.show,
                          outdir=outdir,
                          params_file='params',
                          roi=roi)


