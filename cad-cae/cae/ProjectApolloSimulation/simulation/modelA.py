#!/usr/bin/env python
#Use python 3
import sys
import math
import matplotlib.pyplot as plt
import numpy as np

vinfo=sys.version_info
print('version:{}'.format(vinfo))
assert vinfo.major >= 3 and vinfo.minor>=5
'''
This is a simulation of the PDE covering gas flow through a cylinder 
of a pressure swing adsorption oxygen generator
Based on equations in Farooq, Ruthven, Boniface, Chemical Engineering Science vol 44 No 12 1989
Numerical Simulation of a Pressure Swing Adsorption Oxygen Unit

We discretize the PDE in the spatial dimension, then use an ODE solver from scipy
to solve it

'''

'''
Start with simulation of just 1 bed over a pressurization cycle
'''
#Local modules
import orifice

Ta=20+273.15    #room temperature in K
ATM=1e5     #1 atm in Pa (N/m3)
R=8.314     #J/mol-K
#constants
liters_in_m3=1000

#constants of the system
class Obj:
  pass

param=Obj()
param.L=30    #Length of cylinder (cm)
param.N=100     #discretization steps in spatial direction
param.t=0.010   #time step for ODE
#Note: random sphere packing is 64%, densest is 74%, void fractions=.36, .26
param.epsilon=0.36  # void fraction (fraction of space filled with gas vs. spheres)
param.DL=1.0    #Axial dispersion coefficient cm2/s (1e-4 m2/s)
param.KA=4.7     #Equilibrium constant for O2  ??
param.KB=14.8     #Equilibrium constant for N2 ??
param.kA=62       #LDF constant for O2 1/s
param.kB=19.7     #LDF constant for N2 1/s
param.qAs=5.26e-3  #saturation constant for O2  mol/cm3
param.qBs=5.26e-3  #saturation constant for N2  mol/cm3
param.feed_o2=0.21  #feed gas fraction O2
param.feed_n2=1.0-param.feed_o2  # feed gas fraction N2
#Langmuir constants.  inverse of pressure mol/m3 where we reach 50% of saturation
#Estimated these from chart in Yang book at 1 ATM.  
param.bA=573.5   # cm3/mol, inverse of 42.5 bar
param.bB=2030    # cm3/mol, inverse of 12 bar

#1,2 are the cylinder number
#a,b are the components
#a: oxygen
#b: nitrogen
#helper functions
#For these cA, cB can be scalars or arrays
#return value will match
def adsorp_equilibrium_A(cA,cB):
  #for Oxygen
  #given c in mol/cm3, returns adsorption equilibrium in mol/cm3 of zeolite
  #for those concentrations
  #assume a density of 1 g/cm3 for zeolite
  f=param.bA*cA/(1+param.bA*cA+param.bB*cB)
  return f*param.qAs

def adsorp_equilibrium_B(cA,cB):
  #for Nitrogen
  #given c in mol/cm3, returns adsorption equilibrium in mol/cm3 of zeolite
  #for those concentrations
  #assume a density of 1 g/cm3 for zeolite
  f=param.bB*cB/(1+param.bA*cA+param.bB*cB)
  return f*param.qBs

def mass_transfer_rate(cA,cB,qA,qB):
  #for arrays of cA, cB
  qA_eq=adsorp_equilibrium_A(cA,cB)
  qB_eq=adsorp_equilibrium_B(cA,cB)  
  qA_rate=param.kA2(qA_eq - qA)
  qB_rate=param.kB2(qB_eq - qB)
  return qA_rate,qB_rate

  
state=Obj()

zstep=param.L/param.N  # spatial step size
#these are the state variables at each time step t
state.ca1=np.zeros((param.N,))   # moles/m3 oxygen (component A)
state.cb1=np.zeros((param.N,))   # moles/m3 nitrogen (component B)
state.v1=np.zeros((param.N,1))   #vector of interstitial velocity




#All pressures are absolute
container_volume_liters=2
container_volume=container_volume_liters/liters_in_m3   # 2L in m^3

def bottle_volume(p, bottle_vol=containter_volume):
  #how much Nm^3 is in a bottle of size vol at pressure p (Pa)
  return bottle_vol*p/ATM

def bottle_pressure(v, bottle_vol=container_volume):
  #pressure if v Nm^3 in bottle of size vol
  return v/bottle_vol*ATM



  
  
    
  
