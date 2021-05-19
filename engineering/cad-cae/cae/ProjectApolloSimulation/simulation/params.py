import math
import orifice
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
Parameters for the finite-volume simulation
of an oxygen concentrator

'''
#constants of the system
class AttrDict(dict):
  def __init__(self, *args, **kwargs):
    super(AttrDict, self).__init__(*args, **kwargs)
    self.__dict__ = self

def create_param(mods, print=print):
  #mods is a dict to override the parameter defaults set here
  #override print function if needed for debug statements to a log file
  #any keys with value of None are ignored
  #real_cycle_time and real_vent_time will override cycle_time and vent_time
  #cycles and time are used to set the simulate end time
  param=AttrDict()
  #Physical constants
  param.R=8.314     #J/mol-K
  #Simulation parameters
  #the mode for the difference scheme
  param.mode=1
  param.N=20   #discretization steps in spatial direction
  #tstep will be increased if cycle_time is long (no more than 100 steps per cycle)
  param.tstep=0.20        #time step for ODE (dimensionless)
  param.adsorb=True    #set to True to have adsorption happen
  #cycle_time, vent_time
  param.cycle_time=36       #time for a half-cycle in dimensionless time units
  param.vent_time=23.04        #from beginning of cycle in dimensionless time units
  #Physical system parameters
  param.Ta=20+273.15    #room temperature in K
  #Approximate Inviracare XL cylinders
  param.D=0.083     #Diameter of cylinder [m]
  param.L=0.355      #Length of cylinder (m)  - 13"
  #Soda Bottle 2L
  #param.D=0.10     #Diameter of cylinder [m]
  #param.L=0.33      #Length of cylinder (m)  - 13"
  param.product_tank_volume=2.0/1000     # 2L in m3
  #we set an orifice so that we can 5LPM at a certain pressure (3bar abs) and
  #try to get an output pressure sits around that 3 bar.
  #param.epsilon=0.36  # void fraction (fraction of space filled with gas vs. spheres)
  param.epsilon=0.37
  param.epsilonp=0.35    #particle voidage
  param.rp=0.7e-3          #particle radius [m]
  param.tauprime=3.0     # tortuosity
  #flow params
  param.input_orifice=2.2     # dia mm - pressure feed
  param.output_orifice=1.40    # dia mm - to output tank
  #we use an orifice that will provide 5LPM at 3 bar abs product tank
  param.product_orifice=0.475   # dia mm - output from product tank to patient
  param.blowdown_orifice=2.80     # dia mm
  param.vent_orifice=1.0        # dia mm
  param.feed_pressure=3.5        #pressure in bar (1e5 Pa), absolute
  param.PH=param.feed_pressure*1e5   #Pa, will be the normalization pressure
  param.feed_O2=0.21  #feed gas fraction O2
  param.product_pressure=2     # bar in output product tank (not used)
  #Note: random sphere packing is 64%, densest is 74%, void fractions=.36, .26
  #param.DL=1.0        #Axial dispersion coefficient (m2/s)
  #Mol wt O=16, N=14
  #air is about 14e-3 kg/mol
  #These are for my calculation, note different from paper (paper was doing
  #CO2 and N2 separation, and used a more complex Langmuir dual-site model
  #param.qAs=5.26e-3  #saturation constant for O2  mol/cm3 (at infinite pressure)
  #param.qBs=5.26e-3  #saturation constant for N2  mol/cm3 (at infinite pressure)
  #Langmuir constants.  inverse of pressure mol/m3 where we reach 50% of saturation
  #Estimated these from chart in Yang book at 1 ATM.
  
  #param.bA=573.5   # cm3/mol, inverse of 42.5 bar
  #param.bB=2030    # cm3/mol, inverse of 12 bar
  #From Mofahari 2013 paper, for Zeolite 13X
  #param.bA=0.042    # 1/bar @ Room temp
  #param.bB=0.616    # 1/bar @ Room temp
  #From Jee paper, same as Mofahari but *0.10 for k3!!
  #adding correct exponent on Pressure 
  param.bA=0.0042    # 1/bar @ Room temp
  param.bB=0.0616    # 1/bar @ Room temp
  param.qAs=0.0025  #saturation constant for O2  mol/g (at infinite pressure)
  param.qBs=0.0073  #saturation constant for N2  mol/g (at infinite pressure)
  param.nA=1.006    #exponent on bar pressure O2
  param.nB=0.830    #exponent on bar pressure N2
  param.kA=0.620
  param.kB=0.197
  #From Sircar ch22 paper @ 25C
  #param.bA=0.0295    # 1/bar @ Room temp
  #param.bB=0.107    # 1/bar @ Room temp
  #param.qAs=0.00312  #saturation constant for O2  mol/g (at infinite pressure)
  #param.qBs=0.00312  #saturation constant for N2  mol/g (at infinite pressure)

  #For 5A:
  #param.bA=0.052    # 1/bar @ Room temp
  #param.bB=0.165    # 1/bar @ Room temp
  #param.qAs=0.0019  #saturation constant for O2  mol/g (at infinite pressure)
  #param.qBs=0.0025  #saturation constant for N2  mol/g (at infinite pressure)
  #param.kA=.15
  #param.kB=.05

  #From the new paper
  param.rho_s=1130       #adsorbent density kg/m3 (was rho_pellet)
  param.rho_w=7800       #wall density kg/m3
  param.Cpg=30.7     #specific heat capacity of gas phase [J/mol/K]
  param.Cpa=30.7     #specific heat capacity of adsorbed phase [J/mol/K]
  #param.Cps=1070     #specific heat capacity of adsorbent [J/kg/K]
  #from Jee paper
  param.Cps=1339     #specific heat capacity of adsorbent [J/kg/K]
  param.Cpw=502      #specific heat capacity of wall [J/kg/K]
  param.mu=1.72e-5   #fluid viscocity
  param.Dm=1.6e-5    #Molecular diffusivity [FOR CO2/N2 paper]
  param.gamma=1.4    #adiabatic constant
  param.Kz=0.09      #Effective gas thermal conductivity [J/m/K/s]
  param.Kw=16        #Effective thermal cond of wall
  param.hin=8.6      #inside heat transfer coeff
  param.hout=2.5     #outside heat transfer coeff
  param.R=8.314      #Gas constant  [m3Pa/mol/K]
  param.eta=.72      #compression/evacuation efficiency
  #not doing Temp and Tw simulation at the moment
  param.HA=0         #heat of adsorption of component A  [J/mol]
  param.HB=0         #heat of adsorption of component B  [J/mol]
  #####
  #Apply overrides, ignoring None values.  But see cycles/time logic below
  for k in param.keys():
    if k in mods and mods[k] is not None:
      param[k]=mods[k]
      print('params overriding {}:{}'.format(k, param[k]))
  #####
  #Things we have to calculate, need to do this after the overriding
  param.area=(param.D/2)**2*math.pi     #[m2]
  param.volume=param.area*param.L
  print('canister volume is {} L'.format(param.volume*1000))
  param.rin=param.D/2
  param.rout=param.rin + 0.25
  param.epst=param.epsilon/(1-param.epsilon)
  param.epsb=(1-param.epsilon)/param.epsilon
  param.cellsize=param.L/param.N
  param.deltaZ=1/param.N   # nondimensional deltaZ
  param.container_vol=param.area*param.L #volume of reactor (m3)
  #We compute the initial flow rate and use that for velocity normalization
  in_flow=orifice.orifice_flow2(param.feed_pressure*1e5/1000,100,param.input_orifice,T=param.Ta)/60
  in_vel=in_flow/param.area    # [m/s]
  param.norm_v0=in_vel  # [m/s]  feed velocity, which varies
  param.norm_t0=param.L/param.norm_v0   # so t=time/norm.t0
  param.feed_N2=1.0-param.feed_O2  # feed gas fraction N2
  #molecular weight of air is 29 g/mol or 29e-03 kg/mol
  param.rho_g=100000/param.R/param.Ta*29e-3   # Needs to be adjusted for pressure/Temp
  
  param.Dp=param.Dm/param.tauprime
  #NOTE: norm.v0 is computed after this, will be set to param.v0
  #Axial dispersion coefficient m2/sec
  #param.DL=0.7*param.Dm+0.5*param.norm_v0*param.rp*2.0
  param.DL=4.9e-4  # m2/sec from another paper
  print('param.DL={}'.format(param.DL))
  #compute param.end_time for the simulation
  #want exactly 1 to be not None and 1 None
  #assert (mod.cycles is None) + (mod.time is None) == 1
  #total time to simulate (dimensionless).  This is handled differently because
  #we allow 2 ways to override (cycles takes precedence over time)
  #First we check for real_cycle_time and real_vent_time
  rct=mods.get('real_cycle_time', None)
  rvt=mods.get('real_vent_time', None)
  #note: param.real_vent_time will only exist if this method of setting was used.
  #simulation should not care about real cycle time, this is just for reporting
  #in log files
  if rct:
    param.cycle_time=rct/param.norm_t0
    param.real_cycle_time=rct
    print('using a real cycle time of {:7.2f} -> {:7.2f}'.format(rct,param.cycle_time))
  if rvt:
    param.vent_time=rvt/param.norm_t0
    param.real_vent_time=rvt
    print('using a real vent time of {:7.2f} -> {:7.2f}'.format(rvt,param.vent_time))    
  param.end_time=2*param.cycle_time       #default will be 2 cycles
  if mods.get('cycles', None) is not None:
    param.end_time=param.cycle_time*mods['cycles']
  else:
    if mods.get('time', None) is not None:
      param.end_time=mods['time']
  print('running for end_time of {}'.format(param.end_time))
  #this might not be an integer
  param.cycles=param.end_time/param.cycle_time
  #set real times
  param.real_cycle_time=param.cycle_time*param.norm_t0
  param.real_vent_time=param.vent_time*param.norm_t0
  return param
