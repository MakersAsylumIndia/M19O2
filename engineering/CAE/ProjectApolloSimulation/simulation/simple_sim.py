#!/usr/bin/env python
#Use python 3
import sys
import math
import matplotlib.pyplot as plt
import numpy as np

print('version:{}'.format(sys.version))
'''
A very simple simulation of orifice flow in a 3 bottle PSA system, but with
no adsorption modeled.  Just for simple testing.
Uses English units
'''

Ta=72  # temperature
R0=459.67   #degrees Rankin at 0F
ATM=14.7    #1 atm in psi

#reference www.tlc.com/global/US/calculator/air-flow-rate-through-orifice
def orifice_flow(p1, p2, d, C=0.70):
  #C=0.7   # discharge coefficient
  #Q=flow rate out in SCFM
  neg=1
  if not p1>=p2:
    #print('orifice warning {} < {}'.format(p1,p2))
    p1,p2=p2,p1
    neg=-1
  #assert(p1>p2)
  Fy=1.0    #specific heat ratio factor, 1.0 for air
  xT=0.72   # pressure differential ratio
  pr=(p1-p2)/p1
  z=pr/(3*Fy*xT)
  thresh=Fy*xT
  #print('pr {} vs {}'.format(pr, thresh))
  if pr < thresh:
    Q=1/60*1360*C*(d/0.183)**2*p1*(1-z)*math.sqrt(pr/(Ta+R0))
  else:        
    Q=1/60*0.667*1360*C*(d/0.183)**2*p1*math.sqrt(Fy*xT/(Ta+R0))
  #print(' <   {}'.format(Q1))
  #print(' >=  {}'.format(Q2))
  return Q*neg

def test_orifice():
  p2=14.7  # 1 atm
  for p1 in [14.7, 16, 18, 20, 22, 25, 30, 35]:
    print('== p1 {} p2 {}'.format(p1, p2))
    orifice_flow(p1,p2,.05)
  

#All pressures are absolute
def bottle_volume(p, bottle_cf=.0706):
  #how much scfm is in a bottle of size cf at pressure p
  return bottle_cf*p/ATM

def bottle_pressure(v, bottle_cf=0.0706):
  #pressure if v cf in bottle
  return v/bottle_cf*ATM

container_volume=.0706   # cu ft, 2L

tstep=0.01
#Parameters
pF=30  # 30 psi feed pressure
oF=0.05 # diameter of feed orifice
oP=oF*math.sqrt(2) # diameter of produce orifice
oV=0.10   # diameter of vent orifice
oCross=0.10   # diameter of cross vent orifice
use_rate_lpm=5
#Timing:
t_cycle=10   # seconds for entire half-cycle
t_cross=8    # time in cycle when the 2-way valve, venting through the other cylinder
#initial conditions
use_rate_cfm=use_rate_lpm*0.0353
pA=15
pB=22
pC=30
qA=bottle_volume(pA)
qB=bottle_volume(pB)
qC=bottle_volume(pC)
#t is in seconds
#Accumulate results in res
res_time=[]
res_pA=[]
res_pB=[]
res_pC=[]
res_cross=[]  # flow from A into B when cross valve open
#we should simulate 2 complete cycles, but only plot the second, so that we have
#the correct initial conditions for the plot - it reaches steady state with 1 cycle
#at least, for the parameters being played with at the moment
times=np.arange(0,4*t_cycle,tstep)
for t in times:
  #which tank is being pressurized
  #find the time in a 2*t_cycle
  t_in_cycle = t - int(t/2/t_cycle)*2*t_cycle
  #print('{} {}'.format(t, t_in_cycle))
  tankA=t_in_cycle<t_cycle
  tankB=not tankA
  #is the cross valve open
  cross_open= (t_in_cycle>=t_cross and t_in_cycle<t_cycle) or (t_in_cycle>=t_cross+t_cycle and t_in_cycle<2*t_cycle)
  #simulate changes at time t
  if tankA:
    rate_qA=orifice_flow(pF, pA, oF)  # in cf per minute
  else:
    rate_qA=-orifice_flow(pA, ATM, oV)  #leaving the tank
  if tankB:
    rate_qB=orifice_flow(pF, pB, oF)  # in cf per minute
  else:
    rate_qB=-orifice_flow(pB, ATM, oV)  # leaving the tank
    
  rate_qC=0
  if pA > pC:
    #check valve opens, gas flows to product tank
    flow=orifice_flow(pA, pC, oP)
    rate_qA-=flow
    rate_qC+=flow
  if pB > pC:
    #check valve opens, gas flows to product tank
    flow=orifice_flow(pB, pC, oP)
    rate_qB-=flow
    rate_qC+=flow
  #if cross valve open, computer flow
  cross_flow=0
  if cross_open:
    cross_flow=orifice_flow(pA, pB, oCross)
    #NOTE: does this flow interfere with the flow going to produce tank?
    rate_qA-=cross_flow
    rate_qB+=cross_flow
  #using gas from C to output device
  rate_qC+=-use_rate_cfm
  #compute new quantities
  qA+=rate_qA*tstep/60   # per minute flow rate
  qB+=rate_qB*tstep/60   # per minute flow rate
  qC+=rate_qC*tstep/60
  #compute new pressures
  pA=bottle_pressure(qA)
  pB=bottle_pressure(qB)
  pC=bottle_pressure(qC)
  if t>= 2*t_cycle:
    res_pA.append(pA)
    res_pB.append(pB)
    res_pC.append(pC)
    res_cross.append(cross_flow)
    res_time.append(t-2*t_cycle)
#choose the times from 2*t_cycle to 4*t_cycle
fig,(ax1,ax2)=plt.subplots(2)
ax1.plot(res_time,res_pA, label='A')
ax1.plot(res_time,res_pB, label='B')
ax1.plot(res_time,res_pC, label='C')
ax2.plot(res_time,res_cross, label='cross flow A:B')
ax2.set_xlabel('time (sec)')
ax1.set_ylabel('pressure (psi)')
ax2.set_ylabel('flow (scfm)')
fig.suptitle('simulated flows with 5 LPM output')
ax1.vlines(t_cycle,15,25,colors='gray',label='5way')
ax1.vlines(t_cross,15,25,colors='lightgray',label='2way')
#legend=ax.legend(loc='upper center')
legend=ax1.legend(ncol=2, fancybox=True)
legend=ax2.legend()
plt.savefig('graph.png')
plt.show()

  
  
    
  
