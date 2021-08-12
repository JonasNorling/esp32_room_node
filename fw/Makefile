export ZEPHYR_BASE ?= $(HOME)/src/zephyrproject/zephyr
export ZEPHYR_TOOLCHAIN_VARIANT=espressif
export ESPRESSIF_TOOLCHAIN_PATH=${HOME}/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf

BUILD=build
TARGET_BOARD ?= esp32

.PHONY: all clean FORCE flash

all: $(BUILD)/zephyr/zephyr.elf

$(BUILD)/zephyr/zephyr.elf: FORCE
	PATH=${PATH}:${ESPRESSIF_TOOLCHAIN_PATH}/bin \
	west build -d $(BUILD) -b $(TARGET_BOARD)

flash: $(BUILD)/zephyr/zephyr.elf
	west flash -d $(BUILD)

clean:
	rm -rf $(BUILD)
