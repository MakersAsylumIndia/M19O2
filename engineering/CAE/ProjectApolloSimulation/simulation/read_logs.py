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
import matplotlib.pyplot as plt
import numpy as np
import scipy
import pprint
import argparse
import time
import datetime
import os
import re
import copy
import collections

vinfo=sys.version_info
print('version:{}'.format(vinfo))
assert vinfo.major >= 3 and vinfo.minor>=6
'''
Read the params and results from log files
'''
np.set_printoptions(precision=5, edgeitems=6, linewidth=90, suppress=False)
pp=pprint.PrettyPrinter(indent=2)

parser = argparse.ArgumentParser(
    description="Log file reader for psa output",
    epilog='''
''')


parser.add_argument("files", nargs='+', type=str, help="files to process")
parser.add_argument("-d","--detail", action='store_true', help='more detail on values')
#parser.add_argument("--adsorb", dest='adsorb', help='adsorb on ',action='store_true')
#parser.add_argument("--no-adsorb", dest='adsorb', help='adsorb on ',action='store_false')
#parser.set_defaults(adsorb=True)
options = parser.parse_args()
#print('options files = {}'.format(options.files))
reobj_param=re.compile('.*PARAM ({.*})')
reobj_result=re.compile('.*RESULT ({.*})')
start=time.time()
param_list=[]
res_list=[]
file_list=[]

def show_changes(d):
  #d is a list of dict's.  Determine which keys have a differences and print those
  #out
  #Process: make a copy and then delete each key that has the same value in each
  #entry
  #use the first one for the entries
  dc=copy.deepcopy(d)
  keys=d[0].keys()
  for k in keys:
    li=[a[k] for a in d]
    #print('li=',li)
    if len(set(li))==1:
      #get rid of this key
      #print('removing', k)
      for x in dc:
        del x[k]
  return dc
  
def one_file(file):
  print('processing {}'.format(file))
  with open(file, 'rt') as fp:
    param=None
    res=None
    for line in fp:
      if 'PARAM' in line:
        m=re.search(reobj_param, line)
        if m:
          s=m.group(1)
          param=eval(s)
      if 'RESULT' in line:
        m=re.search(reobj_result, line)
        if m:
          s=m.group(1)
          res=eval(s)
    if param is None:
      print('ERROR, no params found')
      return
    if res is None:
      print('ERROR, no results found')
      return
  file_list.append(file)
  param_list.append(param)
  res_list.append(res)
  
for file in options.files:
  try:
    one_file(file)
  except TypeError as e:
    print('a Error {}'.format(e))
  except KeyError as e:
    print('b Error {}'.format(e))
  #print('took {} sec'.format(time.time()-start))
dc=show_changes(param_list)
#Now go through and print out the param changes for each one and the results
for file,param, res in zip(file_list,dc,res_list):
  print('***')
  print('for file {}'.format(file))
  pp.pprint(param)
  print('each odd cycle, container_y: min, max, mean')
  pp.pprint(res['container_y'][-1])
  print('prod_y')
  pp.pprint(res['prod_y'][-1])
  print('product pressure')
  pp.pprint(res['prod_pressure'][-1])
  print('product flow rate (LPM)')
  pp.pprint(res['prod_flow_rate'][-1])

