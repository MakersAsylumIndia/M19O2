#!/usr/bin/env python
#Use python 3
import sys
import math
import collections
import matplotlib.pyplot as plt
import numpy as np
import scipy
import scipy.integrate
import matplotlib
import termios
import tty
import pprint

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

NOTE: this version tries to follow: Haghpanah,...Farooq 2013 "Multiobjective
Optimization of a Four-Step Adsorption Process for Postcombustion CO2 Capture 
Via Finite Volume Simulation"  Ind. Eng. Chem. Research 2013, 52, pp 4249-4265.

This file does simulation of a single container with set input pressure and input 
and output orifices.

See psa_sim.py for a simulation of 2 tank PSA system over multiple cycles


'''

'''
Start with simulation of just 1 bed over a pressurization cycle
'''
#Local modules
import orifice
import params

np.set_printoptions(precision=5, edgeitems=6, linewidth=90, suppress=False)
plt.ion()
pp=pprint.PrettyPrinter(indent=2)

#ATMmol=4.105e-5  # 1 atm in mol/cm3
#constants
#liters_in_m3=1000
#liters_in_cm3=.001

class AttrDict(dict):
  def __init__(self, *args, **kwargs):
    super(AttrDict, self).__init__(*args, **kwargs)
    self.__dict__ = self
    
#These are attributes of state variable (keys also)    
state_names=['P','T','Tw','xA','xB','yA','yB']
#constants of the system
class Obj:
  pass


#All variables are nondimensionalized, normalized by proper values
def create_norm():
  norm=Obj()
  #The normalization values, from Table 1 in Supplementary Info for the Ref paper
  norm.P0=param.PH           # 300kPa      [Pa]
  norm.T0=293.15       #Room temp = 20C, [K]
  norm.qs0=param.qBs*1000   #The normalization amount mol/kg
  norm.xA=norm.qs0     # qs,0  [mol/kg]
  norm.xB=norm.qs0     # qs,0  [mol/kg]
  norm.v0=param.norm_v0            #  [m/s]
  norm.z0=param.L      # Length of cylinder [m]
  norm.t0=norm.z0/norm.v0   # so t=time/norm.t0
  norm.alphaA=param.kA*param.L/norm.v0
  norm.alphaB=param.kB*param.L/norm.v0
  return norm

def create_dgroups(norm, param, state):
  #these are the dimensionless coefficients needed, from SI page 1
  #NOTE that some (like omega's) must be calculated at each state position and
  #are not arrays j=1..N
  g=Obj()
  #DL=1.0e-4*1e4   # axial dispersion [cm2/s]
  g.Pe=norm.v0*param.L/param.DL
  #NOTE: probably a typo in Peh definition, units are wrong in paper
  #NOTE: Need to calculate rho_g from the pressure by gas Law - TODO
  g.Peh=(param.epsilon*norm.v0*param.L)/(param.Kz/param.rho_g/param.Cpg)
  g.phi=param.R*norm.T0*norm.qs0/param.PH*param.epsb*param.rho_pellet
  num=param.Kz/(norm.v0/param.epsilon/param.L)
  paren1=param.rho_s*(param.Cps+norm.qs0*param.Cpa*(state.xA+state.xB))
  g.omega1=param.Kz/(norm.v0*param.epsilon*param.L)/(param.epsb*paren1)
  g.omega2=param.Cpg*norm.P0/param.R/norm.T0/(param.epsb*paren1)
  g.omega3=param.Cpa*norm.qs0/paren1
  g.omega4=2*param.hin*param.L/(param.rin*norm.v0)/(1-param.epsilon)/paren1
  g.sigmaA=-norm.qs0/norm.T0*param.HA/(paren1)
  g.sigmaB=-norm.qs0/norm.T0*param.HB/(paren1)
  g.pi1=param.Kw/(param.rho_w*param.Cpw*norm.v0*param.L)
  g.pi2=(2*param.rin*param.hin/(param.rout**2-param.rin**2)*
         param.L/(param.rho_w*param.Cpw*norm.v0))
  g.pi3=(2*param.rout*param.hout/(param.rout**2-param.rin**2)*
         param.L/(param.rho_w*param.Cpw*norm.v0))
  
  return g
  

def compute_alpha(c,qs,verbose=False):
  #compute the alpha for a given c [mol/m3] and qs [saturation mol/m3] at the
  #given pressures
  #this would typically be called twice, once for A, once for B
  #k is the constant with dimensions 1/sec
  #k=(c/qs)*(15*param.epsilonp*param.Dp/param.rp**2)
  k=3
  alpha=k*param.L/norm.v0
  if verbose:
    print('dynamics: k {} 1/sec  alpha {}'.format(k,alpha))
  #alpha is the dimensionless constant
  return alpha
  
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

#def integral(q, L):
#  #integrate discretized data q over its entire length, which is L units
#  assert q.ndim==1
#  N=q.size
#  return q.sum()*L/N

#def integrate(grad, y0, L):
#  #integrate the vector grad along z for distance L. starting at y0
#  assert grad.ndim==1
#  N=grad.size
#  ret=grad.cumsum()*L/N  + y0
#  return ret

def adsorp_equilibrium(yA,yB,P):
  #for O2, N2
  #given yA (normalized) returns adsorption equilibrium in normalized amount
  #for those concentrations
  #convert to atm partial pressures
  cA=yA*P*norm.P0/1e5          #in atm
  cB=yB*P*norm.P0/1e5          #in atm
  fA=param.bA*cA/(1+param.bA*cA+param.bB*cB)
  fB=param.bB*cB/(1+param.bA*cA+param.bB*cB)
  #equilibrium values at these pressures
  qAeq=fA*param.qAs   # mol/g
  qBeq=fB*param.qBs   # mol/g
  #convert to mol/kg and then normalize
  return qAeq*1000/norm.xA, qBeq*1000/norm.xB  # renormalize

#these are the state variables at each time step t
def init(param,norm):
  #return the initialized state object
  #assume 1ATM, equilibrium concentrations of adsorbed components
  print('initializing state variables')
  state=AttrDict()
  R=param.R
  N=param.N
  state.yA=np.ones((N,))*param.feed_O2     #fraction of gas phase component A (Oxygen)
  state.yB=1-state.yA        #fraction of gas phase component B (Nitrogen)
  state.P=np.ones((N,))*1e5/norm.P0     #Pressure (total gas phase) 1.0=1 ATM
  state.T=np.ones((N,))*param.Ta/norm.T0     #Temp of column   # 1.0 = Room Temp (293K)
  C=(state.P[0]*norm.P0)/R/(state.T[0]*norm.T0)*1e-6  # total conc [mol/cm3]
  cA=C*state.yA[0]
  cB=C*state.yB[0]
  a,b=adsorp_equilibrium(state.yA,state.yB,state.P)
  state.xA=a    #solid phase
  state.xB=b    #solid phase
  state.Tw=np.ones((N,))    #Wall temperature
  #print('state={}'.format(state))
  return state

#NOTE:  A,B = component (O2, N2)
#We will use separate state objects for each container
def diffplus(x, fN5):
  #returns array of x(j+1)-x(j)
  #print('diffplus {} {}'.format(x,fN5))
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
  
def phi(x):
  #phi function for van Leer Finite Volume TVD
  return (x+np.abs(x))/(1+np.abs(x))

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
  r=(f-fminus1+1e-10)/(fplus1-f+1e-10)
  ret=f+0.5*phi(r)*(fplus1-f)
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
  r=(f-fminus1+1e-10)/(fplus1-f+1e-10)
  ret=f+0.5*phi(r)*(fplus1-f)
  #these are the boundary condition values
  #first one is special formula
  ret=np.insert(ret,0,f5)
  return ret[:-1]   # get rid of the last value

def rate_of_change(state,vin,vout):
  #compute the rates of change at current state
  #v0=input velocity of feed gas (dimensionless)
  #vL=output velocity of gas  (dimensionless)
  #Note, because paper has j=1...N, we need need subtract 1 from indices
  #Also, paper has i=0 <==> A; i=1 <==> B for gas components
  #Component Mass Balances
  #Note that all * and / are element-wise by default
  #First, we need to compute dxA, dxB, dP, dT (meaning dT/dt)
  
  deltaZ=param.deltaZ
  
  #Compute the input and output velocities from the orifices and pressures
  #P[0] and P[-1].  The boundary conditions will be modified to make this be
  #the correct velocity, and should be close to accurate. For now, we assume input
  #and output velocities - just for initial debug phase
  
  #calculate velocity profile.  There will be 2 versions:
  #j+.5 j=[1.5,2.5,...,N+.5] and
  #j-.5 j=[.5, 1.5, ..., N-.5]
  #we these instead of evaluating vj at the .5 points with the finite volume
  #calculations
  
  #next calculate boundary conditions
  P1=state.P[0]
  PN=state.P[-1]
  T1=state.T[0]
  TN=state.T[-1]

  #Boundary condition at Z=0
  yAfeed=param.feed_O2
  yBfeed=param.feed_N2
  
  Pz0=P1+(vin*deltaZ/2)/(4/150*param.epst**2*param.rp**2*norm.P0/param.mu/norm.v0/param.L)
  yAz0=(state.yA[0]+yAfeed*vin*dg.Pe*deltaZ/2)/(1+vin*dg.Pe*deltaZ/2)
  yBz0=(state.yB[0]+yBfeed*vin*dg.Pe*deltaZ/2)/(1+vin*dg.Pe*deltaZ/2)
  Tz0=T1
  Twz0=param.Ta/norm.T0
  #print('yAz0={} yA[0]={} yAfeed={}'.format(yAz0,state.yA[0],yAfeed))
  
  #Boundary condition at Z=1 (L m)
  PzL=PN-(vout*deltaZ/2)/(4/150*param.epst**2*param.rp**2*norm.P0/param.mu/norm.v0/param.L)
  yAzL=state.yA[-1]
  yBzL=state.yB[-1]
  TzL=TN
  TwzL=param.Ta/norm.T0
  #compute the velocities - and double check that pressure difference gives same answer
  vplus5=-1/deltaZ*4/150*param.epst**2*param.rp**2*norm.P0/param.mu/norm.v0/param.L*diffplus(state.P,PzL)
  #print('comparing vplus5[-1] {} to vout {}'.format(vplus5[-1],vout))
  #vplus5[-1]=vout
  vminus5=-1/deltaZ*4/150*param.epst**2*param.rp**2*norm.P0/param.mu/norm.v0/param.L*diffminus(state.P,Pz0)
  #print('comparing vminus5[0] {} to vin {}'.format(vminus5[0],vin))
  #vminus5[0]=vin
  
  #now calculate the derivatives with respect to time
  #total mass balance
  
  g=create_dgroups(norm, param, state)  #some things depend on current state
  #solid phase
  if param.adsorption:
    alphaA=compute_alpha(1,1)  #FIX
    alphaB=compute_alpha(1,1)  #FIX
    #
    xAs,xBs=adsorp_equilibrium(state.yA,state.yB, state.P)
    dxA=alphaA*(xAs-state.xA)
    dxB=alphaB*(xBs-state.xB)
  else:
    dxA=np.zeros_like(state.xA)
    dxB=np.zeros_like(state.xB)
  
  dT=np.zeros_like(state.T)
  dTw=np.zeros_like(state.T)
  
  dP=(-state.T/deltaZ*(jplus5(state.P/state.T,Pz0/Tz0,PzL/TzL)*vplus5-
                       jminus5(state.P/state.T,Pz0/Tz0,PzL/TzL)*vminus5)
      -dg.phi*state.T*(dxA+dxB)
      +state.P/state.T*dT)
  #term_p=(-state.T/deltaZ*(jplus5(state.P/state.T,Pz0/Tz0,PzL/TzL)*vplus5-
  #                    jminus5(state.P/state.T,Pz0/Tz0,PzL/TzL)*vminus5))
  #print('\nterm_p {}'.format(term_p))
  #Column energy balance
  #dT=np.zeros_like(state.T)
  #dT=(dg.omega1/deltaZ*(diffplus(state.T)/deltaZ-diffminus(state.T)/deltaZ)-
  #    dg.omega2/deltaZ*(state.v*jplus5(state.P)-state.v*jminus5(state.P))-
  #    dg.omega3*state.T*(dxA+dxB)+
  #    (dg.sigmaA*dxA+dg.sigmaB*dxB)-
  #    dg.omega4*(state.T-state.Tw)-
  #    dg.omega2*dP)
      
  #Wall energy balance
  #dTw=(dg.pi1/deltaZ*(diffplus(state.Tw,TwzL)/deltaZ-diffminus(state.Tw,Twz0)/deltaZ)+
  #     dg.pi2*(state.T-state.Tw)-
  #     dg.pi3*(state.Tw-param.Ta/norm.T0))
  
  dyA=(1/dg.Pe*state.T/state.P/deltaZ*(
    jplus5(state.P/state.T,Pz0/Tz0,PzL/TzL)*diffplus(state.yA,yAzL)/deltaZ-
    jminus5(state.P/state.T,Pz0/Tz0,PzL/TzL)*diffminus(state.yA,yAz0)/deltaZ)
       -state.T/state.P/deltaZ*(
         jplus5(state.yA*state.P/state.T,yAz0*Pz0/Tz0,yAzL*PzL/TzL)*vplus5 -
         jminus5(state.yA*state.P/state.T,yAz0*Pz0/Tz0,yAzL*PzL/TzL)*vminus5)-
       dg.phi*state.T/state.P*dxA
       -state.yA/state.P*dP
       +state.yA/state.T*dT)

  #term1=1/dg.Pe*state.T/state.P/deltaZ*(
  #  jplus5(state.P/state.T,Pz0/Tz0,PzL/TzL)*diffplus(state.yA,yAzL)/deltaZ-
  #  jminus5(state.P/state.T,Pz0/Tz0,PzL/TzL)*diffminus(state.yA,yAz0)/deltaZ)
  #term2=-state.T/state.P/deltaZ*(
  #  jplus5(state.yA*state.P/state.T,yAz0*Pz0/Tz0,yAzL*PzL/TzL)*vplus5 -
  #  jminus5(state.yA*state.P/state.T,yAz0*Pz0/Tz0,yAzL*PzL/TzL)*vminus5)
  #term3=-dg.phi*state.T/state.P*dxA
  #term4=-state.yA/state.P*dP
  #term5=state.yA/state.T*dT
  #print('\nstate.yA')
  #pp.pprint(state.yA)
  #print('diffplus {}'.format(diffplus(state.yA,yAzL)))
  #print('diffminus {}'.format(diffminus(state.yA,yAz0)))
  #print('terms')
  #pp.pprint(term1)
  #pp.pprint(term2)
  #pp.pprint(term3)
  #pp.pprint(term4)
  #pp.pprint(term5)
  #print('dyA:')
  #pp.pprint(dyA)
  
  dyB=(1/dg.Pe*state.T/state.P/deltaZ*(
    jplus5(state.P/state.T,Pz0/Tz0,PzL/TzL)*diffplus(state.yB,yBzL)/deltaZ-
    jminus5(state.P/state.T,Pz0/Tz0,PzL/TzL)*diffminus(state.yB,yBz0)/deltaZ)-
  state.T/state.P/deltaZ*(
    jplus5(state.yB*state.P/state.T,yBz0*Pz0/Tz0,yBzL*PzL/TzL)*vplus5 -
    jminus5(state.yB*state.P/state.T,yBz0*Pz0/Tz0,yBzL*PzL/TzL)*vminus5)-
  dg.phi*state.T/state.P*dxB-
  state.yB/state.P*dP+
  state.yB/state.T*dT)

  return dP,dT,dTw,dxA,dxB,dyA,dyB
  
#def bottle_volume(p, bottle_vol=container_volume):
#  #how much Ncm^3 is in a bottle of size vol at pressure p (Pa)
#  return bottle_vol*p/ATM

#def bottle_pressure(v, bottle_vol=container_volume):
#  #pressure if v Ncm^3 in bottle of size vol
#  return v/bottle_vol*ATM
count=0
def feed_rates(P):
  #return vin, vout (normalized)
  #orifice_flow2(p1, p2, d, C=0.70, T=T_room):
  #input
  P_feed=param.feed_pressure*1e5   # feed_pressure in bar, convert to Pa abs
  P_1=P[0]*norm.P0           #denormalize, convert to Pa
  P_N=P[-1]*norm.P0
  P_exit=1e5                       #1ATM
  #compute flows in sN/m3
  #print('P_feed {} P_1 {} P_N {} P_exit {}'.format(P_feed,P_1,P_N,P_exit))
  in_flow=orifice.orifice_flow2(P_feed/1000,P_1/1000,param.input_orifice,T=param.Ta)
  out_flow=orifice.orifice_flow2(P_N/1000,P_exit/1000,param.output_orifice,T=param.Ta)
  #now convert to velocity in m/s
  in_vel=in_flow*(1e5/P_1)/param.area    # [m/s]
  out_vel=out_flow*(1e5/P_N)/param.area  # [m/s]
  #convert to dimensionless
  #print('flow rates, m/s {} {}'.format(in_vel, out_vel))
  return in_vel/norm.v0, out_vel/norm.v0
                                
def myfun(t, x):
  #function used by the ODE solver
  #must return dx/dt
  global count
  count+=1
  N=param.N
  state=x_to_state(x)
  #Compute input and output velocities
  #For now, just use the pressures at input and output relative to
  #feed pressure and 1 ATM (output pressure)
  vin,vout=feed_rates(state.P)
  if count % 100 == 0:
    print('myfun {:6.3f} vin {:8.2f} vout {:8.2f}'.format(t,vin,vout))
  rates=rate_of_change(state,vin,vout)
  ret=np.hstack(rates)
  
  return ret


def x_to_state(x):
  state=AttrDict()
  N=param.N
  for i,name in zip(range(7),state_names):
    state[name]=x[N*i:N*i+N]
  return state

def v_to_mps(x):
  #velocity conversions between normalized and meters/sec
  return x*norm.v0

def mps_to_v(x):
  #velocity conversions
  return x/norm.v0

def print_state(state):
  print('P:{}'.format(state.P))
  print('T:{}'.format(state.T))
  print('Tw:{}'.format(state.Tw))
  print('xA:{}'.format(state.xA))
  print('xB:{}'.format(state.xB))
  print('yA:{}'.format(state.yA))
  print('yB:{}'.format(state.yB))

param=params.create_param()
norm=create_norm()
state=init(param,norm)  
dg=create_dgroups(norm,param,state)
  
if __name__ =='__main__':
  #output is to 1 ATM pressure (abs)
  x0=np.hstack((state.P,state.T,state.Tw,state.xA,state.xB,state.yA,state.yB))
  #time normalized by norm.t0
  print('norm.t0 = {}'.format(norm.t0))
  #Vectorized means that myfun can take a matrix of values to evaluate dx/dt at,
  #not that x0 is a matrix

  compute_alpha(1,1,verbose=True)
  print('Calling ODE solver')
  #print_state(x_to_state(x0))
  limit=10
  limit_sec=limit*norm.t0
  #limit=limit_sec/norm.t0
  num_time_points=100
  t_span=(0,limit)
  ev=np.arange(0,limit,limit/num_time_points)
  bunch=scipy.integrate.solve_ivp(myfun, t_span, x0, vectorized=False, t_eval=ev,
                                  method='Radau')
  if not bunch.success:
    print(bunch.message)
    sys.exit(1)
  print('evaluated at {} places'.format(bunch.t.size))
  #print(bunch.t)
  
  print('num eval of RHS {}'.format(bunch.nfev))
  print('num eval of Jacobian {}'.format(bunch.njev))
  #t will be array of time points (n_points)
  #y will be array of (n x n_poins) values
  N=param.N
  data=AttrDict()
  #take the output data from ODE solver and place in data.<name> for plotting
  for i,name in zip(range(7),state_names):
    data[name]=bunch.y[N*i:N*i+N,:]

  #compute velocity from Pressure
  r,c=data.P.shape
  V=np.zeros((r+1,c))
  for i in range(c):
    #placeholder for last pressure for now
    P=data.P[:,i]
    vin,vout=feed_rates(P)
    V[1:,i]=-1/param.deltaZ*4/150*(param.epst)**2*param.rp**2*norm.P0/param.mu/norm.v0/param.L*diffplus(P,0)
    V[-1,i]=vout
    V[0,i]=vin
    
  #adsorbed equilibrium
  xAs,xBs=adsorp_equilibrium(data.yA,data.yB, data.P)

  #If too many time points, then decimate the time output
  Pdec=np.copy(data.P)
  yAdec=np.copy(data.yA)
  yBdec=np.copy(data.yB)
  xAdec=np.copy(data.xA)
  xBdec=np.copy(data.xB)
  xAsdec=np.copy(xAs)
  Vdec=np.copy(V)
  tdec=np.copy(bunch.t)
  if bunch.t.size > 10:
    dec=int(bunch.t.size/10)
    Pdec=Pdec[:,::dec]
    yAdec=yAdec[:,::dec]
    yBdec=yBdec[:,::dec]
    xAdec=xAdec[:,::dec]
    xBdec=xBdec[:,::dec]
    Vdec=Vdec[:,::dec]
    tdec=tdec[::dec]
    xAsdec=xAsdec[:,::dec]
  ##########################################3
  ### Plot spatial quantities    
  fig,axs=plt.subplots(3,1,sharex=True)
  ax1,ax2,ax3=axs
  lines=ax1.plot(Pdec, label='Pressure')
  fig.legend(iter(lines), ['t={:4.2f}'.format(t) for t in tdec],
             loc="center right", borderaxespad=0.1)
  ax1.set_title('Pressure',pad=3)
  ax3.set_xlabel('position z')

  #ax2.plot(yAdec, marker='.')
  #ax2.set_title('yA fraction of A in gas phase',pad=3)
  #ax2.set_ylabel('fraction')
  #ax2.set_ylim(bottom=0)

  #ax3.plot(yBdec, marker='.')
  #ax3.set_title('yB fraction',pad=3)
  #ax3.set_ylabel('fraction')
  #ax3.set_ylim(bottom=0)

  #Plot velocity
  #fig,axs=plt.subplots(2,1,sharex=True)
  #ax1,ax2=axs
  #ax2.set_xlabel('position z')
  ax2.plot(Vdec)
  ax2.set_title('velocity',pad=3)
  ax2.set_ylabel('vel [normalized]')
  secax=ax2.secondary_yaxis('right', functions=(v_to_mps,mps_to_v))
  secax.set_ylabel('vel [m/s]')
  plt.subplots_adjust(right=.77)
  
  ax3.plot(xAsdec)
  ax3.set_title('Adsorption Equilibrium - A',pad=3)
  ax3.set_ylim(bottom=0)
  ###############################################
  #### Do spatial components and adsorbed amounts
  fig,axs=plt.subplots(4,1,sharex=True)
  ax1,ax2,ax3,ax4=axs
  
  ax1.plot(yAdec)
  ax1.set_ylim(bottom=0,top=1)
  ax1.set_title('Component A (O2)',pad=1)
  ax4.set_xlabel('position z')

  ax2.plot(yBdec, label='Component B (N2)')
  ax2.set_title('Component B (N2)',pad=1)
  ax2.set_ylim(bottom=0,top=1)
  ax3.plot(xAdec, label='Adsorbed A (O2)')
  ax3.set_title('Adsorbed A (O2)',pad=1)
  ax3.set_ylim(bottom=0)
  ax4.plot(xBdec, label='Adsorbed B (N2)')
  ax4.set_title('Adsorbed B (N2)',pad=0)
  ax4.set_ylim(bottom=0)
  #plt.figure()
  #######################################
  #### Time plots
  mid=int(param.N/2)
  fig,(ax1,ax2,ax3)=plt.subplots(3,1,sharex=True)
  #fig.suptitle('big title')
  #print('inflow {}'.format(in_flow))
  #print('outflow {}'.format(out_flow))
  #ax1.plot(time,in_flow,label='inflow')
  #ax1.plot(time,out_flow,label='outflow')
  #ax1.legend()
  ax1.set_title('flows vs time')
  vin=[]
  vout=[]
  for i in range(bunch.t.size):
    P=data.P[:,i]
    v1,v2=feed_rates(P)
    vin.append(v1)
    vout.append(v2)
  ax1.plot(bunch.t, vin, label='in flow')  #marker='.'
  ax1.plot(bunch.t, vout, label='out flow')
  ax1.plot(bunch.t, V[mid,:], label='mid flow')
  ax1.plot(bunch.t, V[1,:], label='V1 flow')
  ax1.plot(bunch.t, V[-2,:], label='VN flow')
  ax1.legend()
  ax1.set_ylabel('flow rate (normalized)')
  #ax1.set_ylabel('flow Ncm3/s')
  #secax=ax1.secondary_yaxis('right', functions=(cm_to_lpm,lpm_to_cm))
  #secax.set_ylabel('flow (slpm)')
  ax2.plot(bunch.t,data.P[0,:], label='P0')
  ax2.plot(bunch.t,data.P[mid,:], label='Pmid')
  ax2.plot(bunch.t,data.P[-1,:],label='PN')
  ax2.legend()
  ax2.set_title('Pressure')
  
  ax3.plot(bunch.t, data.yA[0,:], marker='.',label='yA0')
  ax3.plot(bunch.t, data.yA[param.N-1,:], marker='.',label='yAN (out)')
  ax3.legend()
  ax3.set_title('yA')
  ax3.set_ylabel('fraction')
  ax3.set_ylim(bottom=0)
  ax3.set_xlabel('time (normalized)')

  ### Plot adsorbed equilibrium
  t_sec=bunch.t*norm.t0
  #fig,(ax1,ax2,ax3,ax4)=plt.subplots(4,1,sharex=True,
  #                                   gridspec_kw={'height_ratios':[3,3,3,1]})
  fig,(ax1,ax2,ax3)=plt.subplots(3,1,sharex=True)
  ax1.plot(t_sec,xAs[1,:], label='xAs 1')
  ax1.plot(t_sec,xAs[mid,:], label='xAs mid')
  ax1.plot(t_sec,xAs[-1,:], label='xAs N')
  ax1.plot(t_sec,data.xA[mid,:],label='xA mid')
  ax1.set_title('Solid Phase A')
  ax1.legend()
  ax1.set_ylim(bottom=0)

  
  ax2.plot(t_sec,xBs[1,:], label='xBs 1')
  ax2.plot(t_sec,xBs[mid,:], label='xBs mid')
  ax2.plot(t_sec,xBs[-1,:], label='xBs N')
  ax2.plot(t_sec,data.xB[mid,:], label='xB mid')
  ax2.set_title('Solid Phase B')
  ax2.set_ylim(bottom=0)
  #ax2.set_xlabel('time (normalized)')

  #fig=plt.figure()
  #ax1=fig.add_axes((0.1,0.3,.8,.6))
  ax3.set_xlabel('time (sec)')
  ax3.plot(t_sec,data.xA[mid,:],label='xA mid')
  #Try second x axis in seconds, but give up for now
  #new_tick_locations=[a for a in ax3.get_xticks() if a>0]
  #print('xticks {}'.format(new_tick_locations))
  #def tick_fxn(x):
  #  return ['{:4.2f}'.format(a*norm.t0) for a in x]
  
  #ax2=fig.add_axes((.1,.1,.8,0))
  #ax4.yaxis.set_visible(False)
  #ax4.set_xticks(new_tick_locations)
  #ax4.set_xticklabels(tick_fxn(new_tick_locations))
  #ax4.set_xlabel('time (sec)')
  #plt.subplots_adjust(hspace=.40)  
  
  
plt.draw()
input('press Enter...')
#this does a detection of a keypress, but it did not let graphs show
'''
orig=termios.tcgetattr(sys.stdin)
tty.setcbreak(sys.stdin)
x=sys.stdin.read(1)
termios.tcsetattr(sys.stdin, termios.TCSADRAIN, orig)
'''

#Show the plots

#All pressures are absolute
#container_volume_liters=2
#container_volume=container_volume_liters/liters_in_cm3   # 2L in cm^3



  
  
    
  

