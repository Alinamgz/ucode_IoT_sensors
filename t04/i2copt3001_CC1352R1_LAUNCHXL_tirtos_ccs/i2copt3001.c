/*
 * Copyright (c) 2016-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *    ======== i2copt3001.c ========
 */
#include <stddef.h>
#include <stdint.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/display/Display.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

/* Module Header */
#include <ti/sail/opt3001/opt3001.h>

/* Driver configuration */
#include "ti_drivers_config.h"

static Display_Handle display;

#define SAMPLE_TIME 10 /*In seconds*/
#define HIGH_LIMIT 25000.0F
#define LOW_LIMIT 100.0F

#define OPT_TASK_STACK_SIZE 768

#define CONFIG_OPT3001_LIGHT 0
#define CONFIG_OPT3001_COUNT 1

OPT3001_Handle opt3001Handle = NULL;
OPT3001_Params opt3001Params;

/* Please check <ti/sail/opt3001/opt3001.h> file to know more about
 * OPT3001_HWAttrs and OPT3001_Config structures */
OPT3001_Object OPT3001_object[CONFIG_OPT3001_COUNT];

const OPT3001_HWAttrs OPT3001_hwAttrs[CONFIG_OPT3001_COUNT] = {
    {
//#ifdef CONFIG_I2C_OPT_BOOSTXL_SENSORS_OPT3001_ADDR  // BOOSTXL-SENSORS
//        .slaveAddress = OPT3001_SA4,
//#else  // BOOSTXL-BASSENSORS
//        .slaveAddress = OPT3001_SA1,
//#endif
        .slaveAddress = OPT3001_SA4,
        .gpioIndex = CONFIG_GPIO_OPT3001_INT,
    },
};

const OPT3001_Config OPT3001_config[] = {
    {
        .hwAttrs = &OPT3001_hwAttrs[0],
        .object = &OPT3001_object[0],
    },
    {NULL, NULL},
};

/* Global lux values which may be accessed from GUI Composer App */
float lux;

/* Global sample rate which may be accessed and set from GUI Composer App */
volatile uint16_t sampleTime;

sem_t opt3001Sem;

/*
 *  ======== opt3001Callback ========
 *  When a fault condition is met on the opt3001 hardware, the INT pin is
 *  asserted generating an interrupt. This callback function serves as an ISR
 *  for a single opt3001 sensor.
 */
void opt3001Callback(uint_least8_t index) {
    sem_post(&opt3001Sem);
    }

/*
 *  ======== opt3001InterruptTask ========
 *  This task is unblocked when the INT pin is asserted and generates an
 *  interrupt. When the OPT3001 is in LATCH mode, the configuration register
 *  must be read to the clear the INT pin.
 */
void *opt3001InterruptTask(void *arg0) {
    uint16_t data;

    while (1) {
        /* Pend on the semaphore, opt3001Sem */
        if (0 == sem_wait(&opt3001Sem)) {
            /* Read config register, resetting the INT pin */
            OPT3001_readRegister(opt3001Handle, &data, OPT3001_CONFIG);

            if (data & OPT3001_FL) {
                Display_print0(display, 0, 0, "ALERT INT: Lux Low\n");
            }

            if (data & OPT3001_FH) {
                Display_print0(display, 0, 0, "ALERT INT: Lux High\n");
            }
        }


    }
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0) {
    I2C_Handle i2cHandle;
    I2C_Params i2cParams;
    pthread_t alertTask;
    pthread_attr_t pAttrs;
    int retc;
    float hiLim = HIGH_LIMIT;
    float loLim = LOW_LIMIT;
    sampleTime = SAMPLE_TIME;

    /* Call driver init functions */
    GPIO_init();
    I2C_init();
    OPT3001_init();

    /* Open the HOST display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1);
    }

    /* Turn on user LED */
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);

    Display_print0(display, 0, 0, "Starting the i2copt3001 example\n");

    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2cHandle = I2C_open(CONFIG_I2C_OPT, &i2cParams);
    if (i2cHandle == NULL) {
        Display_print0(display, 0, 0, "Error Initializing I2C\n");
        while (1);
    }
    else {
        Display_print0(display, 0, 0, "I2C Initialized!\n");
    }

    if (0 != sem_init(&opt3001Sem, 0, 0)) {
        /* sem_init() failed */
        Display_print0(display, 0, 0, "opt3001Sem Semaphore creation failed");
        while (1);
    }

    pthread_attr_init(&pAttrs);
    pthread_attr_setstacksize(&pAttrs, OPT_TASK_STACK_SIZE);
    retc = pthread_create(&alertTask, &pAttrs, opt3001InterruptTask, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        Display_print0(display, 0, 0, "Alert Task creation failed");
        while (1);
    }

    /* Initialize opt3001Params structure to defaults */
    OPT3001_Params_init(&opt3001Params);

    /* Callback for INT pin event */
    opt3001Params.callback = opt3001Callback;

    /* Open OPT3001 sensor with custom parameters */
    opt3001Handle =
        OPT3001_open(CONFIG_OPT3001_LIGHT, i2cHandle, &opt3001Params);

    /* Check if the open is successful */
    if (opt3001Handle == NULL) {
        Display_print0(display, 0, 0, "OPT3001 Open Failed!");
        while (1);
    }

    /* Allow the sensor hardware to complete its first conversion */
    sleep(1);

    /* Set Lux High and Low Limit Registers */
    if (!OPT3001_setLuxLimits(opt3001Handle, hiLim, loLim)) {
        Display_print0(display, 0, 0, "Setting Lux Limits Failed!");
    }

    /* Read Lux Limits written to OPT3001 limit registers */
    if (!OPT3001_getLuxLimits(opt3001Handle, &hiLim, &loLim)) {
        Display_print0(display, 0, 0, "Getting Lux Limits Failed!");
    }

    /* Print set Lux Limits */
    Display_print1(display, 0, 0, "High Lux Limit Set:\t %f\n", hiLim);
    Display_print1(display, 0, 0, "Low Lux Limit Set:\t %f\n", loLim);

    Display_print0(display, 0, 0, "Taking Preliminary Reading...\n\n");

    /* Get current Lux */
    if (!OPT3001_getLux(opt3001Handle, &lux)) {
        Display_print0(display, 0, 0, "OPT3001 sensor read failed");
    }

    Display_print1(display, 0, 0, "Lux: %f", lux);

    /* Enable interrupts from OPT3001 */
    OPT3001_enableInterrupt(opt3001Handle);

    /* Begin infinite task loop */
    while (1) {
        /* Get Lux */
        if (!OPT3001_getLux(opt3001Handle, &lux)) {
            Display_print0(display, 0, 0, "OPT3001 sensor read failed");
        }

        Display_print1(display, 0, 0, "Lux: %f", lux);

        sleep(sampleTime);
    }
}
