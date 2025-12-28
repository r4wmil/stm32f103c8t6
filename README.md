# STM32F1 flushing test

## Building using NoBuild
```sh
gcc build.c -o build
./nob
```

## Requirments
```txt
arm-none-eabi-gcc
arm-none-eabi-gdb
arm-none-eabi-newlib
openocd
```

## Specs
F - general purpose
1 - first gen
0 - access line
3 - mainstream performance line
C - 48 pins
8 - 64 KB Flash
T - industrial temp range (-40C, +85C)
6 - LQFP circuit package

## Where got files

`./STM32F102X6_FLASH.ld` - STM32CubeF1 -> ./Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/linker
`./startup_stm32f103xb.s` - STM32CubeF1 -> ./Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc
`./system_stm32f1xx.c` - STM32CubeF1 -> ./Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/
`./CMSIS/Core/Include` - STM32CubeF1 -> ./Drivers/CMSIS/Core/Include
`./CMSIS/STM32F1/Include` - STM32CubeF1 -> ./Drivers/CMSIS/Core/Include

## Links
STM32 naming conventions:
- https://www.compilenrun.com/docs/iot/stm32/stm32-fundamentals/stm32-naming-convention/
