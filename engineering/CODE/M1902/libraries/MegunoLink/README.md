MLP
=== 

An [Arduino library](http://www.megunolink.com/documentation/arduino-libraries/) for sending MegunoLink Pro packets to the various visualisers.
Currently supports sending data to:

* [XY-Plots](http://www.megunolink.com/documentation/plotting/)
* [Time plots](http://www.megunolink.com/documentation/plotting/)
* [Tables](http://www.megunolink.com/documentation/table/)
* [Maps](http://www.megunolink.com/documentation/mapping/)
* [Interface panel](http://www.megunolink.com/documentation/interface-panel/)
* [Message stream](http://www.megunolink.com/documentation/monitoring-data/)

The library also supports:

* A [command handler for serial commands](http://www.megunolink.com/documentation/arduino-libraries/serial-command-handler/)
* A class to make it easier to write [timer driven code](http://www.megunolink.com/documentation/arduino-libraries/arduino-timer/) with Arduino millis() timer
* A template for [storing data in the eeprom](http://www.megunolink.com/documentation/arduino-libraries/eepromstore/)
* An [exponential filter](http://www.megunolink.com/documentation/arduino-libraries/exponential-filter/)
* A [circular buffer template](http://www.megunolink.com/documentation/arduino-libraries/circular-buffer/)
* A command processor for dispatching commands

Visit www.MegunoLink.com to download [MegunoLink Pro](http://www.MegunoLink.com). 

Arduino Installation
--------------------
Install the library into the `libraries` folder of your Arduino installation. 

Visual Studio Installation
--------------------------
The library can also be used with our [Arduino Build Tool for Microsoft Visual Studio](http://www.megunolink.com/building-arduino-programs-with-microsoft-visual-studio/). To
use the library in your Arduino program, open the Property Manager (View&rarr;Other Windows&rarr;Property Manager) then:

1. Select the Arduino program's project in the Property Manager
1. Click the "Add existing property sheet" on the Property Manager toolbar. 
1. Browse to the library project and select the file named MLPLib.props

Now building the Arduino Program will also build the referenced libraries. The libraries will pick up the hardware settings (CPU, clock frequency etc) from the Arduino Program. This means if you have one 
program targeting a Mega and another targeting the Uno, both can use the same Arduino Library project
for common code.

License Information
--------------------------
The hardware is released under the GNU Lesser General Public License (LGPL)

Distributed as-is; no warranty is given.
