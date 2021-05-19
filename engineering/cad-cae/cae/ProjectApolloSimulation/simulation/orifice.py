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

#Use python 3
import math

#reference www.tlv.com/global/US/calculator/air-flow-rate-through-orifice
#this is SI units
#Output in Nm^3/min
T_room=20+273.15   # Room temp in K
R=8.314     #J/mol-K

#This is helper - uses kPa pressure and SNm3 flow rate
def orifice_flow2(p1, p2, d, C=0.70, T=T_room):
  #C=0.7   # discharge coefficient
  #T temp in K
  #d: diameter in mm
  #p1,p2 kPa abs
  #returns flow in Nm^3/min
  #assert p1>=0
  #assert p2>=0
  #ODE solver requests states with negative values, so we should just report 0 flow
  if p1<0 or p2<0:
    return 0
  neg=1
  if not p1>=p2:
    p1,p2=p2,p1
    neg=-1
  Fy=1.0    #specific heat ratio factor, 1.0 for air
  xT=0.72   # pressure differential ratio
  pr=(p1-p2)/p1
  z=pr/(3*Fy*xT)
  thresh=Fy*xT
  #print('pr {} vs {}'.format(pr, thresh))
  if pr < thresh:
    Q=1/60*4.17*C*(d/4.654)**2*p1*(1-z)*math.sqrt(pr/(T))
    #print('<')
  else:        
    Q=1/60*0.667*4.17*C*(d/4.654)**2*p1*math.sqrt(Fy*xT/(T))
    #print('>')
  #print(' <   {}'.format(Q1))
  #print(' >=  {}'.format(Q2))
  #print('p1={} p2={} d={} Q={}'.format(p1,p2,d,Q))
  return Q*neg

#This is one used in simulation
def orifice_flow(c1, c2, d, C=0.70, T=T_room):
  #c1,c2 are concentrations in mol/cm3
  #d is diameter in mm
  #returns flow in cm3/sec at the output concentration c2, flow in STP cm3/sec 
  #First convert to units for helper function
  #P=n/V*RT
  p1=c1*1e6*R*T    # in Pa
  p2=c2*1e6*R*T    # in Pa
  x=orifice_flow2(p1/1000,p2/1000,d,C,T)/60  # SNm^3/s
  #V=n/P*RT
  f=x*(100000/p2)*1e6    # cm3/sec
  return f, x*1e6        #at output pressure, normal cm3/sec
  
  

if __name__=='__main__':
  #check that this matches tlv.vom
  #answers should be .0143, .0222, .0297
  for p in ( (200,100,1), (300,100,1), (400,100,1)):
    f=orifice_flow2(p[0],p[1],p[2])
    print('flow {} {}'.format(p,f))
