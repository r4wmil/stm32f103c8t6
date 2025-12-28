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
	nob_temp_reset();
	cmd_append(&cmd,
			"platform/startup_stm32f103xb.s", "platform/system_stm32f1xx.c",
			"-T", "platform/STM32F102X6_FLASH.ld",
			"-o", "out/binary.elf",
			"-I/usr/arm-none-eabi/include", "-ICMSIS/Core/Include", "-ICMSIS/STM32F1/Include",
			"-mcpu=cortex-m3", "-mthumb", "-nostdlib", "-DSTM32F103x6");

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
