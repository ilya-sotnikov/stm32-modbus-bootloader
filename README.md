# stm32-modbus-bootloader

Simple Modbus RTU bootloader for STM32F103 (blue pill).

The code is somewhat messy but good enough for POC.

Inspired by this series of articles by SLR:

- [Bootloader Basics – Part 1](https://embetronicx.com/tutorials/microcontrollers/stm32/bootloader/bootloader-basics/)
- [Simple STM32F103 Bootloader Implementation – Bootloader Development Tutorial Part 2](https://embetronicx.com/tutorials/microcontrollers/stm32/stm32f1-bootloader/simple-stm32f103-bootloader-tutorial/)
- [STM32F1 Firmware Update using Custom Bootloader – Bootloader Development Tutorial Part 3](https://embetronicx.com/tutorials/microcontrollers/stm32/stm32f1-bootloader/stm32f1-firmware-update-using-custom-bootloader/)

## Structure

- stm32f103-bootloader -- bootloader
- stm32f103-app -- example application (blink LED)

## Protocol

You can use it with [mbloader](https://github.com/ilya-sotnikov/mbloader). There's an explanation of the protocol.

## License

- FreeMODBUS -- BSD-3-Clause
- STM32 LL -- BSD-3-Clause
- CMSIS -- Apache-2.0

## Import

STM32CubeIDE -> File -> Import -> Existing Projects into Workspace
-> Select root directory

## How it works

1. After reset, the MCU waits 5 seconds for the command to start programming.
This is indicated by a rapidly blinking LED.
2. If the MCU received the command, programming sequence will be executed.
3. If the MCU didn't receive the command, it will jump to the app.

## Usage

You can use it with the [mbloader](https://github.com/ilya-sotnikov/mbloader).

There's an example app. Check the linker file and the system_stm32f1xx.c 
file (USER_VECT_TAB_ADDRESS and VECT_TAB_OFFSET).

Bootloader occupies the first 16 KB of flash.
