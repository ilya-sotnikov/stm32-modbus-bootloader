# stm32-modbus-bootloader

Simple Modbus RTU bootloader for STM32F103 (blue pill).

## Structure

- stm32f103-bootloader -- bootloader
- stm32f103-app -- example application (blink LED)

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

You can use it with the mbloader.

There's an example app. Check the linker file and the system_stm32f1xx.c 
file (USER_VECT_TAB_ADDRESS and VECT_TAB_OFFSET).

Bootloader occupies the first 16 KB of flash.
