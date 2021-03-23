# &ensp;Sensors
This challenge is made to get aqcuanted with Sensors BoosterPack, learn how to work with sensors. Also to complete this challenge various types of communication were researched (paralel vs serial, syncrhonous vs asyncronous).

### &ensp; <b> t00 </b>
&ensp;&ensp;&ensp;&ensp;  What is UART and how to use it.<br/>
&ensp;&ensp;&ensp;&ensp; An app that turns on red LED, runs UART and echoes back to the console.
#### &ensp;&ensp;&ensp;&ensp; <b> Devices</b>
 - [Board LAUNCHXL-CC1352R1](https://www.ti.com/tool/LAUNCHXL-CC1352R1#description)
---

### &ensp; t01
 &ensp;&ensp;&ensp;&ensp; Temperature, pressure and humidity sensor. <br/>
 &ensp;&ensp;&ensp;&ensp; App that gets temperature, pressure and humidity from a BME280 sensor and writes them in a console every 10 seconds.
 #### &ensp;&ensp;&ensp;&ensp; <b> Devices</b>
 - [Board LAUNCHXL-CC1352R1](https://www.ti.com/tool/LAUNCHXL-CC1352R1#description)
 - [Board BOOSTXL-SENSORS](https://www.ti.com/tool/BOOSTXL-SENSORS)
---

### &ensp; t02
 &ensp;&ensp;&ensp;&ensp; What is a logic analyzer abd how to use it. <br/>
 &ensp;&ensp;&ensp;&ensp; Same app as in t01, but here logic analyzer is connected on the SCL and SDA lines.
  #### &ensp;&ensp;&ensp;&ensp; <b> Devices</b>
 - [Board LAUNCHXL-CC1352R1](https://www.ti.com/tool/LAUNCHXL-CC1352R1#description)
 - [Board BOOSTXL-SENSORS](https://www.ti.com/tool/BOOSTXL-SENSORS)
 - [Logic analyzer Saleae](https://arduino.ua/prod651-logicheskii-analizator-saleae-8-kanalov)
---

### &ensp; t03
 &ensp;&ensp;&ensp;&ensp; Accelerometer, magnetometer and gyroscope. <br/>
 &ensp;&ensp;&ensp;&ensp; App that reads data from BMM150 and BMI160 sensors and returns a complete picture of the orientation of the device in space.
 #### &ensp;&ensp;&ensp;&ensp; <b> Devices</b>
 - [Board LAUNCHXL-CC1352R1](https://www.ti.com/tool/LAUNCHXL-CC1352R1#description)
 - [Board BOOSTXL-SENSORS](https://www.ti.com/tool/BOOSTXL-SENSORS)
---

### &ensp; t04
 &ensp;&ensp;&ensp;&ensp; Photodetector. <br/>
 &ensp;&ensp;&ensp;&ensp; App that reads data from OPT3001 Ambient Light Sensor and writes the luminous flux per unit area (every 10 seconds).
 #### &ensp;&ensp;&ensp;&ensp; <b> Devices</b>
 - [Board LAUNCHXL-CC1352R1](https://www.ti.com/tool/LAUNCHXL-CC1352R1#description)
 - [Board BOOSTXL-SENSORS](https://www.ti.com/tool/BOOSTXL-SENSORS)
---
---

 ## &ensp; &ensp;  <b> How to run this project? </b>
 - git clone
 - open project with CCS
 - select `empty_CC1352R1_LAUNCHXL_tirtos_ccs` in your project explorer
 - press `hammer` icon to build this project
 - there are various ways to flash the board with built project:
    - select `empty_CC1352R1_LAUNCHXL_tirtos_ccs` in your project explorer and ...
   - ... press `curly brackets in a folder` icon
   </br>OR
   - ... press Run -> Load -> [project_name]
![screenshot](https://user-images.githubusercontent.com/54025456/109845882-43b68980-7c56-11eb-97dd-72f7ce694c9f.png)

   #### OR
   - Open <b>UniFlash</b> and follow [this video guide](https://www.youtube.com/watch?v=V3-xcRmu5S0&t=51s) for device autodetect, and [this guide](http://software-dl.ti.com/ccs/esd/uniflash/docs/v5_0/quick_start_guide/uniflash_quick_start_guide.html) to flash the device.

## &ensp;  What to `.gitignore`?
Which CCS project files should be checkd in, and which should be `.gitignore`d? [Read here](https://software-dl.ti.com/ccs/esd/documents/sdto_ccs_source-control.html)

