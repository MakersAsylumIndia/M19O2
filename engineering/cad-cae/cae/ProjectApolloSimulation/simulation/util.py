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

import subprocess
import os
import errno

def get_git_commit():
    #return latest git commit hash
    #NOTE: could use: git show -s HEAD --format="%h %ci" for commit and datetime
    x=['git','show', '-s', 'HEAD', '--format="%h %ci"']
    #x=['git','rev-parse','--short','master']
    result=subprocess.run(x, stdout=subprocess.PIPE)
    return result.stdout.decode('utf-8')

def cm_to_lpm(x):
  #convert Ncm3/s to LPM
  return x/1000*60

def lpm_to_cm(x):
  #convert LPM to Ncm3/s
  return x*1000/60

def conc_to_bar(c):
  #convert mol/cm3 to bar (ATM)
  return c/4.105e-5

def bar_to_conc(p):
  #convert pressure bar to mol/cm3
  return p*4.105e-5

def safe_mkdir(outdir):  
  try:
    os.makedirs(outdir)
  except OSError as e:
    if e.errno != errno.EEXIST:
      raise

def limit(x,a,b):
  #limit array-like x to fall between and a and b.  Use None for no limit
  #in that direction
  y=x.copy()
  if a is not None:
    y[y<a]=a
  if b is not None:
    y[y>b]=b
  return y
