#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define CC "arm-none-eabi-gcc"

int main(int argc, char** argv) {

	NOB_GO_REBUILD_URSELF(argc, argv);

	if (argc != 2) {
		nob_log(NOB_ERROR, "Must have 1 argument: ");
		nob_log(NOB_ERROR, "E. g.: './build 1_blink'");
		return 1;
	}
	
	Cmd cmd = {0};

	if (!nob_mkdir_if_not_exists("out")) { return 1; }

	cmd_append(&cmd, CC, nob_temp_sprintf("%s/main.c", argv[1]));
	//cmd_append(&cmd, CC, nob_temp_sprintf("-I%s/", argv[1]));
	nob_temp_reset();
	cmd_append(&cmd,
			"STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103x6.s",
			"STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c",
			"-T", "./STM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/linker/STM32F102X6_FLASH.ld",
			"-o", "out/binary.elf",
			"-I/usr/arm-none-eabi/include",
			"-ISTM32CubeF1/Drivers/CMSIS/Include",
			"-ISTM32CubeF1/Drivers/CMSIS/Core/Include",
			"-ISTM32CubeF1/Drivers/CMSIS/Device/ST/STM32F1xx/Include",
			"-mcpu=cortex-m3", "-mthumb", "-specs=nosys.specs", "-DSTM32F103x6");

	if (!strcmp(argv[1], "4_hal_blink")) {
		cmd_append(&cmd, "-I./STM32CubeF1/Projects/STM3210C_EVAL/Examples/GPIO/GPIO_IOToggle/Inc"); // for 'stm32f1xx_hal_conf.h'
		cmd_append(&cmd, "-ISTM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Inc");
		cmd_append(&cmd, "-ISTM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy");
		cmd_append(&cmd, "STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c");
		cmd_append(&cmd, "STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c");
		cmd_append(&cmd, "STM32CubeF1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c");
	}

	if (!cmd_run(&cmd)) { return 1; }

	cmd_append(&cmd,
			"arm-none-eabi-objcopy",
			"-O", "binary",
			"out/binary.elf", "out/binary.bin");

	if (!cmd_run(&cmd)) { return 1; }

	//cmd_append(&cmd, "st-flash", "write", "out/binary.bin", "0x08000000");
	cmd_append(&cmd, "st-flash", "--connect-under-reset", "write", "out/binary.bin", "0x08000000");

	if (!cmd_run(&cmd)) { return 1; }

	return 0;
}
