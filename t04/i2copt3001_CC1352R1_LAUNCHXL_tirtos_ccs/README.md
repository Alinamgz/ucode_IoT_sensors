## Example Summary

Sample application to read lux value from an OPT3001 optical sensor.

## Peripherals & Pin Assignments

SysConfig generates the driver configurations into the __ti_drivers_config.c__
and __ti_drivers_config.h__ files. Information on pins and resources used
is present in both generated files. The SysConfig user interface can also be
utilized to determine pins and resources used.

Pin assignments for BOOSTXL-BASSENSORS BoosterPack
* `CONFIG_LED_0_GPIO` - Indicator LED
* `CONFIG_I2C_OPT` - I2C peripheral instance used to communicate with the OPT3001 sensor.
* `CONFIG_GPIO_OPT3001_INT` - OPT3001 interrupt pin
* `CONFIG_GPIO_OPT3001_PWR` - OPT3001 power pin

Pin assignments for BOOSTXL-SENSORS BoosterPack
* `CONFIG_LED_0_GPIO` - Indicator LED
* `CONFIG_I2C_OPT` - I2C peripheral instance used to communicate with the OPT3001 sensor.
* `CONFIG_GPIO_OPT3001_INT` - OPT3001 interrupt pin

## BoosterPacks, Board Resources & Jumper Settings

This example requires a [__BOOSTXL-BASSENSORS BoosterPack__](http://www.ti.com/tool/BOOSTXL-BASSENSORS) or [__BOOSTXL-SENSORS BoosterPack__](http://www.ti.com/tool/BOOSTXL-SENSORS).

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in your project
directory for resources used and board-specific jumper settings. Otherwise, you can find
Board.html in the directory &lt;PLUGIN_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

> The OPT3001 sensor has the feature of alerting (raising an interrupt) when the measured LUX value goes below configured MIN value.
Please note when using CC3220S and SF LaunchPads, there is hardware conflict when using OPT3001 Interrupt pin on the BOOSTXL-SENSORS board and an installed pulldown resistor. The resistor on the LaunchPad should be removed if the example is expected to run correctly. 
For Revision B marked boards, R128 (connected to SW2) should be removed. For Revision A marked boards, R142 (connected to SW3) should be removed. 

> In case of the BOOSTXL-BASSSENSORS also the opt3001 interrupt does not work out of the box for the CC3220S and CC3220SF
LaunchPads, we need to populate a 0-ohm resistor R157 to get the interrupt working(please check the schematics for CC3220S and CC3220SF LaunchPads for more information).

## Example Usage

* Connect the BOOSTXL-BASSENSORS BoosterPack before powering the hardware.

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's Homepage"),teraterm, CCS terminal, etc.) to the appropriate COM port.
    * The COM port can be determined via Device Manager in Windows or via `ls /dev/tty*` in Linux.

The connection will have the following settings:
```
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```

* Run the example. `CONFIG_LED_0_GPIO` turns ON to indicate the driver
initialization is complete.

* The example will request lux samples from the OPT3001 and print them on
the console.
```
        Starting the i2copt3001 example
        I2C Initialized!
        High Lux Limit Set:      24995.8398
        Low Lux Limit Set:       100.0000
        Taking Preliminary Reading...
        Lux: 186.0000
        lux: 186.0000
        lux: 186.0000
```

By default, this example works with BOOSTXL-BASSSENSORS BoosterPack. If you want to use this example for BOOSTXL-SENSORS BoosterPack, follow the below steps:-
* Open the i2copt3001.syscfg
* Select the Hardware tab
* Select the component "BAS Sensors BoosterPack I2C" and click the "REMOVE SOFTWARE" button
* Select the subcomponent "OPT3001 Interrupt" from the component "OPT3001 Sensor" and click the "REMOVE SOFTWARE" button
* Select the subcomponent "OPT3001 Power" from the component "OPT3001 Sensor" and click the "REMOVE SOFTWARE" button
* Select the component "Sensors I2C", click the "USE SOFTWARE" button and change the "Name" of this component as "CONFIG_I2C_OPT"
* Select the subcomponent "Optical Interrupt" from the component "Optical Sensor", click the "USE SOFTWARE" button, change the "Name" of this subcomponent as "CONFIG_GPIO_OPT3001_INT", "Pull" as "Pull Up", and "Interrupt Trigger" as "Falling Edge"

## Application Design Details

This application uses two threads:

`mainThread` - performs the following actions:

1. Opens and initializes an OPT3001 driver object.

2. Uses OPT3001 driver to get data from the OPT3001 sensor.

3. Extracts the light intensity (in lux) and prints the value onto the console.

4. The thread sleeps for 1 second.

`opt3001InterruptTask` - performs the following actions:

1. The opt3001InterruptTask alert is always blocked on a semaphore opt3001Sem.

2. Whenever the lux goes below 100  OPT3001 interrupt pin is triggered and unblocks this task.

3. This task prints lux Alert message when unblocked.