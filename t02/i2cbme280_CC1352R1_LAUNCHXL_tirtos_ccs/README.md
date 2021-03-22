## Example Summary

Sample application to read pressure, temperature and humidity from the BME280 sensor.

## Peripherals & Pin Assignments

SysConfig generates the driver configurations into the __ti_drivers_config.c__
and __ti_drivers_config.h__ files. Information on pins and resources used
is present in both generated files. The SysConfig user interface can also be
utilized to determine pins and resources used.

Pin assignments for BOOSTXL-BASSENSORS BoosterPack
* `CONFIG_I2C_BME` - I2C peripheral instance used to communicate with BME280 Sensor

## BoosterPacks, Board Resources & Jumper Settings

This example requires a [__BOOSTXL-SENSORS BoosterPack__](http://www.ti.com/tool/BOOSTXL-SENSORS).
> If you're using an IDE (such as CCS or IAR), please refer to Board.html in your project
directory for resources used and board-specific jumper settings. Otherwise, you can find
Board.html in the directory &lt;PLUGIN_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

* Connect the BOOSTXL-SENSORS BoosterPack before powering the hardware.

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's Homepage"),teraterm, CCS terminal, etc.) to the appropriate COM port.
    * The COM port can be determined via Device Manager in Windows or via `ls /dev/tty*` in Linux.

The connection will have the following settings:
```
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```
* Run the example.

* The example collects the samples for humidity, temperature and pressure from BME280 driver and displays it on serial console as shown below.
```

    Starting the BME280 sensor example...

    I2C Initialized!

    70 KPa(Pressure), 22 DegC(Temp), 59 %RH(Humidity)

    90 KPa(Pressure), 26 DegC(Temp), 58 %RH(Humidity)

    90 KPa(Pressure), 26 DegC(Temp), 58 %RH(Humidity)

```

## Application Design Details

This application uses 1 thread:

`mainThread` - performs the following actions:

1. Opens and initializes an I2C Driver.

2. Initialize the BME280 sensor and set readout template.

3. Periodically read the temperature, humidity and pressure reading from the BME280 sensor
