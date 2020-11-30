# nRF Connect

(previosly named nRF Master Control Panel)

## Application

The nRF Connect is an application designed for Bluetooth Low Energy developers. It allows for scanning for BLE devices and communicating with them.

The nRF Connect for Android applicaiton requires Android version 4.3 or later and is available on Google Play:

https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp

or [here](https://github.com/NordicSemiconductor/Android-nRF-Connect/releases), in case Google Play is not available. We recommend to install the application using Google Play as it will get automatic updates.

**Note: The source code of nRF Connect is not available.**

## Features

1. Scanning for Bluetooth LE devices in range
1. Displaying the RSSI graph with an option to export to CSV or EXCEL
1. Displaying detailed packet history with RSSI, packet changes and advertising intervals
1. Displaying advertisements of mutliple devices on a timeline
1. Connecting to multiple devices at the same time
1. Showing device services and characteristics
1. Reading and writing characteristics
1. Configuring GATT Server
1. Peripheral mode (BLE advertising) (newest Android 5+ devices only)
1. Logging events and packets to nRF Logger app (available on Google Play)
1. Device Firmware Update (DFU) (read below)
1. Automated tests (read below)
1. Recording and playing macros to automate operations
1. Parsing most of Bluetooth SIG adopted characteristics + some proprietary ones
1. Support for Eddystone (parsing, decrypting EID, Eddystone Config Service)

## DFU - Device Firmware Update

The Device Firmware Update BLE protocol may be used to update the Soft Device, Bootloader or application Over-the-Air on nRF5 device. The full DFU documentation may be found on http://developer.nordicsemi.com/.

Since SDK 6.1 the DFU does not require physical intereaction with the device (e.g. pressing a button). A 'buttonless' DFU is available. The application, that supports the buttonless DFU should have the DFU Service present in its services list. See the SDK for samples.

The DFU Bootloader from SDK 7.0+ requires the extended init packet. Please, check the [Creating a zip with image and init packet](https://www.nordicsemi.com/en/DocLib/Content/SDK_Doc/nRF5_SDK/v15-2-0/lib_bootloader_dfu_validation) or [How to create the init packet](init%20packet%20handling/README.md). The Distribution packet (ZIP) is supported in nRF Connect since version 3.0 (named nRF Master Control Panel at that time). For older versions, the old ZIP file with the fixed naming convention or the separate HEX/BIN and DAT files should ne used. 

## Automated testing

The nRF Connect allows for executing XML scripts with test suites. Two sample XMLs for testing: the HRM-DFU sample and HRM-Scanning sample are copied to /Nordic Semiconductor folder on the phone's/tablet's local storage when the application is first launched or updated (make sure the Storage permission is granted for Android 6+). They contains the full documentation of the currently available features. Copy those files and the *test.bat* script from your phone (or from here) to your PC.

To start the test run the *test.bat* script:

    test.bat [-d device_id] [-E KEY EXTRA]* test.xml

You may specify key-value pairs in execution or inside the test using 

    <set name="KEY" value="VALUE" />
    
and then use them as `${KEY}` (see the sample_test_hrm.xml).

Read mode about the test suites in the [documentation](documentation/Automated%20tests/README.md).

## Macros

Starting from version 4.4.0 it is now possible to automate operations using macros. Macros can be recorded, or imported from a XML file.

Read more about macros in the [macros documentation](documentation/Macros/README.md).
