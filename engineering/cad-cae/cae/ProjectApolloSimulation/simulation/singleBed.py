#!/usr/bin/env python
#Use python 3
import sys
import math
import matplotlib.pyplot as plt
import numpy as np
import matplotlib

vinfo=sys.version_info
print('version:{}'.format(vinfo))
assert vinfo.major >= 3 and vinfo.minor>=6
print('plt {}'.format(matplotlib.__version__))
#NOTE: matplotlib 3.2 needs python 3.6, at least for easy pip install
'''
This is a simulation of the PDE covering gas flow through a cylinder 
of a pressure swing adsorption oxygen generator
Based on equations in Farooq, Ruthven, Boniface, Chemical Engineering Science vol 44 No 12 1989
Numerical Simulation of a Pressure Swing Adsorption Oxygen Unit

We discretize the PDE in the spatial dimension, then use an ODE solver from scipy
to solve it

Start with simulation of just 1 bed over a pressurization cycle.
This is superceded by psa.py and single_sim.py
'''
#Local modules
import orifice

#set default options for array printouts
np.set_printoptions(precision=5, edgeitems=6, linewidth=90, suppress=False)

Ta=20+273.15    #room temperature in K
#ATM=1e5      #1 atm in Pa (N/m3)
ATMmol=4.105e-5  # 1 atm in mol/cm3
R=8.314     #J/mol-K
#constants
liters_in_m3=1000
liters_in_cm3=.001

#constants of the system
class Obj:
  pass

param=Obj()
#Simulation parameters
param.N=20     #discretization steps in spatial direction
param.tstep=0.010   #time step for ODE
param.T=20      #total time to simulate
param.adsorption=True   #set to True to have adsorption happen

#System parameters
#Picked these sizes for a volume of about 2000 cm3
param.L=28    #Length of cylinder (cm)
param.D=9.5    #Diameter of cylinder (cm)
param.A=(param.D/2)**2*math.pi
param.container_vol=param.A*param.L #volume of reactor (cm3)
param.input_orifice=2.0    # dia mm
param.output_orifice=0.5    # dia mm
param.feed_pressure=2.0     #pressure in bar (1e5 Pa), absolute

#Note: random sphere packing is 64%, densest is 74%, void fractions=.36, .26
param.epsilon=0.36  # void fraction (fraction of space filled with gas vs. spheres)
param.DL=1.0        #Axial dispersion coefficient cm2/s (=1e-4 m2/s)
param.KA=4.7        #Equilibrium constant for O2  ?? what is this
param.KB=14.8       #Equilibrium constant for N2 ?? what is this
#These are the correct ones
#param.kA=62       #LDF constant for O2 1/s
#param.kB=19.7     #LDF constant for N2 1/s
#Slower ones to see sim working
param.kA=10       #LDF constant for O2 1/s
param.kB=10       #LDF constant for N2 1/s

param.qAs=5.26e-3  #saturation constant for O2  mol/cm3 (at infinite pressure)
param.qBs=5.26e-3  #saturation constant for N2  mol/cm3 (at infinite pressure)
param.feed_O2=0.21  #feed gas fraction O2
param.feed_N2=1.0-param.feed_O2  # feed gas fraction N2
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
  assert cA.ndim==1
  assert cB.ndim==1
  qA_eq=adsorp_equilibrium_A(cA,cB)
  qB_eq=adsorp_equilibrium_B(cA,cB)
  print('\nAdsorbed:')
  print('qA_eq={}'.format(qA_eq))
  print('qA={}'.format(qA))
  print('qB={}'.format(qB))
  print('qB_eq={}'.format(qB_eq))
  print()
  qA_rate=param.kA*(qA_eq - qA)
  qB_rate=param.kB*(qB_eq - qB)
  if param.adsorption:
    return qA_rate,qB_rate
  else:
    return qA_rate*0,qB_rate*0
  
def integral(q, L):
  #integrate discretized data q over its entire length, which is L units
  assert q.ndim==1
  N=q.size
  return q.sum()*L/N

def integrate(grad, y0, L):
  #integrate the vector grad along z for distance L. starting at y0
  assert grad.ndim==1
  N=grad.size
  ret=grad.cumsum()*L/N  + y0
  return ret

def first_derivative(q):
  #first spatial derivative
  d=np.diff(q, append=q[-1])  # what is correct thing to append to the end?
  N=q.size
  return d/param.L*N

def second_derivative(q):
  #second spatial derivative
  d=first_derivative(q)
  return first_derivative(d)
  
state=Obj()

zstep=param.L/param.N  # spatial step size
#these are the state variables at each time step t
#We assume constant pressure at each time 
state.C=ATMmol                       # total mol/cm3 at current state
state.cA=np.ones((param.N,))*ATMmol*param.feed_O2   # moles/cm3 oxygen (component A)
#track cB as C-state.cA
state.cB=state.C-state.cA   # moles/cm3 nitrogen (component B)
#This is computed each time, so not really part of the state
state.v=np.zeros((param.N,))   #vector of interstitial velocity cm/s
#Should start adsorbed N2/O2 at the equilibrium for 1 ATM
#adsorbed A (O2) mol/cm3 
state.qA=np.ones((param.N,))*adsorp_equilibrium_A(state.cA[0],state.cB[0])
#adsorbed B (N2) mol/cm3
state.qB=np.ones((param.N,))*adsorp_equilibrium_B(state.cA[0],state.cB[0])

print('initial cA {}'.format(state.cA))
print('initial cB {}'.format(state.cB))

print('initial qA {}'.format(state.qA))
print('initial qB {}'.format(state.qB))

print()
#NOTE:  A,B = component (O2, N2)
#We will use separate state objects for each container

def rate_of_change(state,v0,vL):
  #compute the rates of change at current state
  #v0=input velocity of feed gas
  #vL=output velocity of gas
  #Overall material balance
  L=param.L
  C=state.C
  void_ratio=(1-param.epsilon)/param.epsilon
  state.cB=C-state.cA
  qA_rate, qB_rate=mass_transfer_rate(state.cA,state.cB,state.qA,state.qB)
  #slow down for simulation study:
  qA_rate*=.2
  qB_rate*=.2
  #print('qA_rate = {}'.format(qA_rate))
  total_change_in_solid_phase=integral(qA_rate+qB_rate, L)
  print('total change in solid phase {}'.format(total_change_in_solid_phase))

  #units of this equation are mol/cm3/s:
  C_rate=(1/L)*(-void_ratio*total_change_in_solid_phase - C*(vL-v0))
  #compute the gradient of v along the z axis
  grad_v=(1/C)*(-C_rate-void_ratio*(qA_rate+qB_rate))
  print('grad_v {}'.format(grad_v))
  #get the v profile from v0 to vL by integrating
  v=integrate(grad_v, v0, L)
  #print('v {}'.format(v))
  #compute the rate of change for the cA component
  grad_cA=first_derivative(state.cA)
  grad2_cA=second_derivative(state.cA)
  cA_rate= (param.DL*grad2_cA
             -v*grad_cA
             -state.cA*grad_v
             -void_ratio*qA_rate)
  return cA_rate,qA_rate,qB_rate,C_rate, v
  
#Now we can do time steps

steps=int(param.T/param.tstep)

v_data=[]
cA_data=[]
qA_data=[]
time=[]
C_data=[]
in_flow=[]
out_flow=[]
for i in range(steps):
  print('\n*****')
  t=i*param.tstep
  #compute v0, vL
  f_input, fi=orifice.orifice_flow(bar_to_conc(param.feed_pressure),state.C,param.input_orifice)
  v0=f_input/param.A
  #output is to 1 ATM pressure (abs)
  f_output,fo=orifice.orifice_flow(state.C,bar_to_conc(1),param.output_orifice)  
  vL=fo*(ATMmol/state.C)/param.A
  flow_lps=v0*param.A*state.C*1461/60
  Catm=conc_to_bar(state.C)
  print('t={:6.2f} C={:9.6f} mol/cm3 {:9.6f} bar v0={:6.2f}cm/s,{:8.2f} L/s vL={:6.2f}'.format(t,state.C,Catm,v0,flow_lps,vL))
  print('cA={}'.format(state.cA))
  print('cB={}'.format(state.cB))
  cA_rate, qA_rate,qB_rate,C_rate, v=rate_of_change(state,v0,vL)
  #update the state
  state.v=v
  state.cA=state.cA + cA_rate*param.tstep
  state.cB=state.C - state.cA
  state.C=state.C + C_rate*param.tstep
  state.qA=state.qA + qA_rate*param.tstep
  state.qB=state.qB + qB_rate*param.tstep
  
  print('v {}'.format(v))
  print('C_rate={}'.format(C_rate))
  print('cA_rate={}'.format(cA_rate))
  steps_per_sec=int(1/param.tstep)
  if i % steps_per_sec == 0:
    #arrays at each step
    v_data.append(v)
    cA_data.append(state.cA)
    qA_data.append(state.qA)
    #scalars at each step
    time.append(t)
    C_data.append(state.C)
    in_flow.append(fi)
    out_flow.append(fo)

fig,axs=plt.subplots(3,1,sharex=True)
ax1,ax2,ax3=axs
d=np.stack(v_data,axis=1)
ax1.plot(d, label='velocity')
ax1.set_title('velocity',pad=3)
ax3.set_xlabel('position z')

d=np.stack(cA_data,axis=1)
ax2.plot(d/ATMmol, label='cA',marker='.')
ax2.set_title('cA',pad=3)
ax2.set_ylabel('cA (bar)')
ax2.set_ylim(bottom=0)

d=np.stack(qA_data,axis=1)
ax3.plot(d, label='qA',marker='.')
ax3.set_title('qA',pad=3)
ax3.set_ylabel('mol/cm3')
ax3.set_ylim(bottom=0)

#plt.figure()
#Time plots
fig,(ax1,ax2)=plt.subplots(2,1,sharex=True)
#fig.suptitle('big title')
print('inflow {}'.format(in_flow))
print('outflow {}'.format(out_flow))
ax1.plot(time,in_flow,label='inflow')
ax1.plot(time,out_flow,label='outflow')
ax1.legend()
ax1.set_title('flows vs time')
ax1.set_ylabel('flow Ncm3/s')
secax=ax1.secondary_yaxis('right', functions=(cm_to_lpm,lpm_to_cm))
secax.set_ylabel('flow (slpm)')

g=np.array(C_data)
ax2.plot(time, g/ATMmol, marker='.')
ax2.set_title('C vs time')
ax2.set_ylabel('C (bar)')
ax2.set_ylim(bottom=0)

#Show the plots
plt.show()

#All pressures are absolute
container_volume_liters=2
container_volume=container_volume_liters/liters_in_cm3   # 2L in cm^3

def bottle_volume(p, bottle_vol=container_volume):
  #how much Ncm^3 is in a bottle of size vol at pressure p (Pa)
  return bottle_vol*p/ATM

def bottle_pressure(v, bottle_vol=container_volume):
  #pressure if v Ncm^3 in bottle of size vol
  return v/bottle_vol*ATM



  
  
    
  
