# M19 Oxygen Concentrator Testbench
LabVIEW testbench to tune process parameters for the M-19 Oxygen Concentrator

# Author
Vijay Raghav Varada

@vjvarada

vjvarada@fracktal.in

2021

# Usage

### Arduino Firmware
Add Arduino libraries into your Arduino libraries directory and upload the firmware.ino code in the firmware folder

### Settings Tab
![alt text](https://github.com/FracktalWorks/M19-oxygen-concentrator-testbench/raw/main/Images/Settings%20Tab.png "Settings Tab")

Before starting, navigate to the settings tab, select the Arduino's Com Port. Select the Pin definition used and if your electronics is an Active HIGH ot Active LOW system

### Instrument Panel
![alt text](https://github.com/FracktalWorks/M19-oxygen-concentrator-testbench/raw/main/Images/Instrument%20Panel.png "Instrument Panel")

The instrument panel is where all the magic happens. Displays all the timing graphs and displays oxygen percentage. 

Switch the ON/OFF Switch to ON position and clock the run arrow on the top left to start the code

The bottom right has the manual/auto mode, where you can switch between the automatic operation ot manually operate the valves



# Dependencies
NI-Visa Drivers : https://www.ni.com/en-in/support/downloads/drivers/download.ni-visa.html

LabVIEW Runtime Engine: https://www.ni.com/en-in/support/downloads/software-products/download.labview-runtime.html



# Licence
M19 Oxygen Concentrator Testbench is licensed under the GNU Affero General Public License, version 3.