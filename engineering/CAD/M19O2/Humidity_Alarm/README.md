# M19O2 Humidity Alarm

  Reads a DHT-22 sensor attached in-line between the output of Dessicant Sieve and input of high pressure regulator of the M19O2 Oxygen Concentrator.
  This tells us how much moisture is entering the Zeolite Dessicant Sieves.
  
  * HIGH = >50%rH = Red Blinking LED + Buzzer
  * MID = <50%rH, >30%rH = Red Blinking LED
  * LOW = <30%rH, >15%rH = Blinking Green LED
  * AWESOME = <15%rH = Steady Green LED

  The DHT22 can only measure under ambient pressure (1 ATM), so we tap off the high pressure line and feed air through restrictors (orifices) in to an air-sealed chamber containing the DHT-22.
  The output from the air sealed chamber feeds back in to the main line, again through a similar restrictor orifice.

  In a later iteration, a flushing solenoid valve can be connected which will purge incomming air back to atmosphere until humidity level falls below a set percentage, thus preventing humid air from reaching the sieves.

  This code is in the public domain.

  Hardware and code by Anool J M (@anool) and Narender Sharma of Maker's Asylum  

  https://github.com/MakersAsylumIndia/M19_OxiKit/tree/main/engineering/M19Oxykit_humidity_alarm

=======

![Humidity Alarm photo 1](https://github.com/MakersAsylumIndia/M19_OxiKit/blob/main/engineering/Humidity_Alarm/Photos/rH_01.jpg)

![Humidity Alarm photo 2](https://github.com/MakersAsylumIndia/M19_OxiKit/blob/main/engineering/Humidity_Alarm/Photos/rH_02.jpg)

License
-------
[CERN Open Hardware Licence v1.2 ]

[CERN Open Hardware Licence v1.2 ]:http://www.ohwr.org/attachments/2388/cern_ohl_v_1_2.txt
