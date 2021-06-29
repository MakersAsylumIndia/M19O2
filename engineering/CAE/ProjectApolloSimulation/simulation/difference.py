import numpy as np
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

'''
different functions to compute f[j+0.5]
use: vanleer or weno

'''
#helper functions
def phi(x):
  #phi function for van Leer Finite Volume TVD
  return (x+np.abs(x))/(1+np.abs(x))

#these are choices for f[j+.5] computation (f5_function)
def vanleer(f,fplus1,fminus1):
  r=(f-fminus1+1e-10)/(fplus1-f+1e-10)
  ret=f+0.5*phi(r)*(fplus1-f)
  return ret

def weno(f,fplus1,fminus1):
  delta=1e-10
  alpha0=2/3/(fplus1-f+delta)**4
  alpha1=1/3/(f-fminus1+delta)**4
  a=alpha0+alpha1
  ret=alpha0/a/2*(f+fplus1) + alpha1/a*(3/2*f-fminus1/2)
  return ret

def uds_forward(f,fplus1,fminus1):
  #simple Upwind Difference Scheme (UDS) for forward flow
  return f

def uds_backward(f,fplus1,fminus1):
  #simple Upwind Difference Scheme (UDS) for backward flow 
  return fplus1

##This generates the function requested
def gen_j5_functions(mode=1, forward=True):
  #returns jplus5 and jminus5 functions
  #mode: 1=UDS, 2=vanleer, 3=superbee, 4=weno
  if mode==1:
    if forward:
      f5_function=uds_forward
    else:
      f5_function=uds_backward
  elif mode==2:
    f5_function=vanleer
  elif mode==3:
    f5_function=superbee
  elif mode==4:
    f5_function=weno
  else:
    raise 'unknown mode {} should be 1-4'.format(mode)

  def jplus5(f,f5,fN5):
    #returns array of f(j+.5), where f5=f(.5) and fN5=f(N+.5)
    #f is [j=1,2,3,...N]
    #jplus is [1.5,2.5,...N+.5]
    #are the boundary condition values that need to be provided
    #we will do the vectorized math, then fixup the first and last entries
    #ret=[1.5,2.5,...,N+.5]
    #first we add f0 onto the front of f so that the math can be vectorized
    f1=f[0]
    f0=f1-2*(f1-f5)
    fN=f[-1]
    fN1=fN+2*(fN5-fN)
    #                             f=[f1,f2,...,fN]
    fplus1=np.append(f[1:],[0])   # [f2,f3,...,fN, fN+1]
    fminus1=np.append(f0,f[:-1])  # [f0,f1,...,fN-1]
    ret=f5_function(f,fplus1,fminus1)
    #these are the boundary condition values
    ret[-1]=fN5
    return ret
  def jminus5(f,f5,fN5):
    #returns array of f(j-.5), where f5=f(.5) and fN5=f(N+.5)
    #f is [j=1,2,3,...N]
    #jminus5 is [.5,1.5,2.5,...N-.5]
    #are the boundary condition values that need to be provided
    #we will do the vectorized math, then fixup the first and last entries
    #first we add f0 onto the front of f so that the math can be vectorized
    f1=f[0]
    f0=f1-2*(f1-f5)
    fN=f[-1]
    fN1=fN+2*(fN5-fN)
    #                             f=[f1,f2,...,fN]
    fplus1=np.append(f[1:],fN1)   # [f2,f3,...,fN, fN+1]
    fminus1=np.append(f0,f[:-1])  # [f0,f1,...,fN-1]
    ret=f5_function(f,fplus1,fminus1)
    #these are the boundary condition values
    #first one is special formula
    ret=np.insert(ret,0,f5)
    return ret[:-1]   # get rid of the last value
  return jplus5, jminus5
  

def diffplus(x, fN5):
  #returns array of x(j+1)-x(j)
  x1=np.roll(x,-1)
  ret=x1-x
  ret[-1]=2*(fN5-x[-1])
  return ret
  
def diffminus(x,f5):
  #returns array of x(j)-x(j-1)
  x1=np.roll(x,1)
  ret=x-x1
  ret[0]=2*(x[0]-f5)
  return ret

  
  

  
