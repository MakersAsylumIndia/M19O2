
Note: this repository contains the simulation code for Project Apollo

Please refer to the original repository for more details on Apollo: https://github.com/oxycon/ProjectApollo 

These files are for a simulation of pressure swing adsorption oxygen concentrator

# References

For numerical methods:
Haghpanah, et al "Multiobjective Optimization of a Four-Step Adsorption Process
for Postcombustion CO2 Capture Via Finite Volume Simulation"  Industrial & Engineering
Chemistry Research, 2013

For PSA operation:
Farooq, "Numerical Simulation of a Pressure Swing Adsorption Oxygen Unit"
Chemical Engineering Science, Vol 44, No 12, pp 2809-2816, 1989.

For constants for sieve material 5A and 13X
Mofarahi, "Comparision of Two Pressure Swing Adsorption Processes for Air Separation
Using Zeolite 5A and Zeolite 13X", Petroleum & Coal,  ISSN 1337-7027, 55 (3) 216-225, 2013.

# Software organization
```
psa.py         simulation module - provides a function simulate() and simulate_and_plot()
               which can be called to do the acutal simulation
params.py       sets the parameters (other files are available: params-apollo, etc.)
mylog.py       log file utilities
difference.py  finite volume and finite difference functions
simple_sim     early simulation that does not do any adsorption (ignore)
```
# Top level files
```
(these call simulate_and_plot() ) and run from the command line.
Use standard python command line options, use --h for help
single_sim     does a single simulation, demostrates usage of simulate_and_plot
optimize       Uses scipy optimization routines to optimize variable through search
annealing      Uses simulated annealing technique for same
monte_sim      Uses Monte Carlo technique for same
multiple_sim   Does a range of simulations
search_sim     Similar, tries to capture best result
jee_sim        Simulation of Jee paper parameters
```

# State Vars
The state variables in the system are:
For each container (1 and 2), each state variable in an (N,) array, where
N (=param.N) is the number of cells in the discretization of the PDE.  10 cells
works well, although higher accuracy is obtained with a much larger number.  All
the variables are in dimensionless values, that are normalized to generally be from
0-1, although they could exceed that.  For example, time is normalized so that the
initial velocity air flow takes 1 unit of time to travel across the container
bed, but the simulation might run for 100's of units of time.
```
   P:  Pressure of gas in bed
   T:  Temperature in bed (this version uses a contant T)
   Tw: Temperature of wall of bed (this version uses a constant Tw)
   xA: Adsorbed amount (mass/volume) of component A (Oxygen)
   xB: Adsorbed amount (mass/volume) of component B (Nitrogen)
   yA: partial concentration of component A (Oxygen) in bed (fraction 0-1)
   yB: partial concentration of component B (Nitrogen) in bed (fraction 0-1)
```
The product tank has 2 state variables:
```
  Pprod:  Pressure in tank
  yprod:  fraction of Oxygen in tank
```
# Modes
We have 6 modes to operate under:
```
                            COL1                    COL2
MODE     Switch 5-way valve                            
half==0 and 1.   Pressurize 1           pressurizing              Vent
half==0 and 2.   Produce 1              pressurize & output       Vent
Open cross valve
half==0 and 3.   CrossVent              Pressurize, out to COL2   Vent
Close Cross valve and Switch 5-way valve
half==1 and 1.   Pressure 2
half==1 and 2.   Produce 2
Open cross valve
half==1 and 3.   CrossVent 2
and repeat at top
```

# Observations

1. The cycle time must be short enough that the product (O2) tank can supply the
   desired flow rate without dropping the pressure too much.  At 5 LPM, a 2L tank
   at 3 bar abs can hold about 2*2/5*60 sec of product (48 sec).
2. The absorption rate of the seive material affects the rate of pressure growth
   in the tanks.  Thus the cycle time is dependent on the absorption rate of the
   material
3. A shorter cycle time could work, but the oxygen level grows slower (in cycles),
   as the pressure does not reach feed pressure by the end of the cycle (unless
   a larger orifice is used).  This might be OK, it just takes more than 7 cycles
   to reach steady state.
   See sims for 10 sec (real) cycle


# NOTES
You can use Inkscape to view and convert svg files. inkview comes with inkscape.

inkview <svg-files>
inkscape -z -w <width-in-pixels> -h <height-in-pixels> <svg-file> -e <png-file-name>

Inviracare XL cylinders are D=.083m, L=.355m
2 Liter bottle: D=0.10, L=.33
