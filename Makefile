# Copyright (C) 2021 Toitware ApS.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; version
# 2.1 only.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# The license can be found in the file `LICENSE` in the top level
# directory of this repository.

.ONESHELL: # Run all lines of targets in one shell
.SHELLFLAGS += -e

BUILD_DATE = $(shell date -u +"%Y-%m-%dT%H:%M:%SZ")

# Use 'make ESP32_ENTRY=examples/mandelbrot.toit' to compile a different
# example for the ESP32 firmware.
ESP32_ENTRY=examples/hello.toit
ESP32_WIFI_SSID=
ESP32_WIFI_PASSWORD=
ESP32_PORT=
ESP32_CHIP=esp32
export IDF_TARGET=$(ESP32_CHIP)

# Use Toitware ESP-IDF fork by default.
export IDF_PATH ?= $(CURDIR)/third_party/esp-idf

ifeq ($(OS),Windows_NT)
	EXE_SUFFIX=".exe"
else
	EXE_SUFFIX=
endif

BIN_DIR = build/host/sdk/bin
TOITPKG_BIN = $(BIN_DIR)/toitpkg$(EXE_SUFFIX)
TOITLSP_BIN = $(BIN_DIR)/toitlsp$(EXE_SUFFIX)
TOITVM_BIN = $(BIN_DIR)/toitvm$(EXE_SUFFIX)
TOITC_BIN = $(BIN_DIR)/toitc$(EXE_SUFFIX)

# Note that the boot snapshot lives in the bin dir.
TOIT_BOOT_SNAPSHOT = $(BIN_DIR)/toitvm_boot.snapshot

SNAPSHOT_DIR = build/host/sdk/snapshots

prefix ?= /opt/toit-sdk

TOOLS = $(TOITPKG_BIN) $(TOITLSP_BIN) $(TOITVM_BIN) $(TOITC_BIN)
SNAPSHOTS = $(SNAPSHOT_DIR)/system_message.snapshot $(SNAPSHOT_DIR)/snapshot_to_image.snapshot $(SNAPSHOT_DIR)/inject_config.snapshot

.PHONY: all
all: tools

.PHONY: tools
tools: check-env $(TOOLS) $(SNAPSHOTS)

.PHONY: tools-riscv64
tools-riscv64: check-env toitpkg toitlsp build/riscv64/sdk/bin/toitvm build/riscv64/sdk/bin/toitc

.PHONY: build/riscv64/sdk/bin/toitvm build/riscv64/sdk/bin/toitc
build/riscv64/sdk/bin/toitvm build/riscv64/sdk/bin/toitc: build/riscv64/CMakeCache.txt
	(cd build/riscv64 && ninja build_toitvm)

build/riscv64/CMakeCache.txt: build/riscv64/
	(cd build/riscv64 && cmake ../../ -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../../toolchains/riscv64.cmake)

.PHONY: tools-arm64
tools-arm64: check-env toitpkg toitlsp build/arm64/sdk/bin/toitvm build/arm64/sdk/bin/toitc

.PHONY: build/arm64/sdk/bin/toitvm build/arm64/sdk/bin/toitc
build/arm64/sdk/bin/toitvm build/arm64/sdk/bin/toitc: build/arm64/CMakeCache.txt
	(cd build/arm64 && ninja build_toitvm)

.PHONY: build/win64/sdk/bin/toitvm build/win64/sdk/bin/toitc
build/win64/sdk/bin/toitvm build/win64/sdk/bin/toitc: build/win64/CMakeCache.txt
	(cd build/win64 && ninja build_toitvm)

.PHONY: build/win32/sdk/bin/toitvm build/win32/sdk/bin/toitc
build/win32/sdk/bin/toitvm build/win32/sdk/bin/toitc: build/win32/CMakeCache.txt
	(cd build/win32 && ninja build_toitvm)

build/arm64/CMakeCache.txt: build/arm64/
	(cd build/arm64 && cmake ../../ -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../../toolchains/arm64.cmake)

build/win64/CMakeCache.txt: build/win64/
	(cd build/win64 && cmake ../../ -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../../toolchains/win64.cmake)

build/win32/CMakeCache.txt: build/win32/
	(cd build/win32 && cmake ../../ -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../../toolchains/win32.cmake)

.PHONY: tools-arm32
tools-arm32: check-env toitpkg toitlsp build/arm32/sdk/bin/toitvm build/arm32/sdk/bin/toitc

.PHONY: build/arm32/sdk/bin/toitvm build/arm32/sdk/bin/toitc
build/arm32/sdk/bin/toitvm build/arm32/sdk/bin/toitc: build/arm32/CMakeCache.txt
	(cd build/arm32 && ninja build_toitvm)

build/arm32/CMakeCache.txt: build/arm32/
	(cd build/arm32 && cmake ../../ -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../../toolchains/arm32.cmake)

.PHONY: esp32
esp32: check-env build/$(ESP32_CHIP)/toit.bin

.PHONY: check-flash-env
check-flash-env:
ifndef ESP32_PORT
	$(error ESP32_PORT is not set)
endif

.PHONY: flash
flash: check-flash-env esp32
	python $(IDF_PATH)/components/esptool_py/esptool/esptool.py --chip $(ESP32_CHIP) --port $(ESP32_PORT) --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0xd000 build/$(ESP32_CHIP)/ota_data_initial.bin 0x1000 build/$(ESP32_CHIP)/bootloader/bootloader.bin 0x10000 build/$(ESP32_CHIP)/toit.bin 0x8000 build/$(ESP32_CHIP)/partitions.bin

build/$(ESP32_CHIP)/toit.bin build/$(ESP32_CHIP)/toit.elf: build/$(ESP32_CHIP)/lib/libtoit_image.a build/config.json
	make -C toolchains/$(ESP32_CHIP)/
	$(TOITVM_BIN) tools/inject_config.toit build/config.json build/$(ESP32_CHIP)/toit.bin

build/$(ESP32_CHIP)/lib/libtoit_image.a: build/$(ESP32_CHIP)/$(ESP32_CHIP).image.s build/$(ESP32_CHIP)/CMakeCache.txt
	(cd build/$(ESP32_CHIP) && ninja toit_image)

# We don't track dependencies in the Makefile, so we always have to call out to ninja.
.PHONY: $(TOITVM_BIN) $(TOITC_BIN) $(TOIT_BOOT_SNAPSHOT)
$(TOITVM_BIN) $(TOITC_BIN) $(TOIT_BOOT_SNAPSHOT): build/host/CMakeCache.txt
	(cd build/host && ninja build_toitvm)

build/host/CMakeCache.txt: build/host/
	(cd build/host && cmake ../.. -G Ninja -DCMAKE_BUILD_TYPE=Release)

build/$(ESP32_CHIP)/CMakeCache.txt: build/$(ESP32_CHIP)/
	(cd build/$(ESP32_CHIP) && IMAGE=build/$(ESP32_CHIP)/$(ESP32_CHIP).image.s cmake ../../ -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../../toolchains/$(ESP32_CHIP)/$(ESP32_CHIP).cmake --no-warn-unused-cli)

build/$(ESP32_CHIP)/$(ESP32_CHIP).image.s: build/$(ESP32_CHIP)/ build/snapshot $(TOITVM_BIN) $(SNAPSHOT_DIR)/snapshot_to_image.snapshot
	$(TOITVM_BIN) $(SNAPSHOT_DIR)/snapshot_to_image.snapshot build/snapshot $@

$(SNAPSHOT_DIR):
	mkdir -p $@

$(SNAPSHOT_DIR)/snapshot_to_image.snapshot: tools/snapshot_to_image.toit $(TOITC_BIN) $(SNAPSHOT_DIR)
	$(TOITC_BIN) -w $@ $<

$(SNAPSHOT_DIR)/system_message.snapshot: tools/system_message.toit $(TOITC_BIN) $(SNAPSHOT_DIR)
	$(TOITC_BIN) -w $@ $<

$(SNAPSHOT_DIR)/inject_config.snapshot: tools/inject_config.toit $(TOITC_BIN) $(SNAPSHOT_DIR)
	$(TOITC_BIN) -w $@ $<

build/snapshot: $(TOITC_BIN) $(ESP32_ENTRY)
	$(TOITC_BIN) -w $@ $(ESP32_ENTRY)

.PHONY: build/config.json
build/config.json:
	echo '{"wifi": {"ssid": "$(ESP32_WIFI_SSID)", "password": "$(ESP32_WIFI_PASSWORD)"}}' > $@

GO_BUILD_FLAGS ?=
ifeq ("$(GO_BUILD_FLAGS)", "")
$(eval GO_BUILD_FLAGS=CGO_ENABLED=1 GODEBUG=netdns=go)
else
$(eval GO_BUILD_FLAGS=$(GO_BUILD_FLAGS) CGO_ENABLED=1 GODEBUG=netdns=go)
endif

GO_LINK_FLAGS ?=
GO_LINK_FLAGS +=-X main.date=$(BUILD_DATE)

TOITLSP_SOURCE := $(shell find ./tools/toitlsp/ -name '*.go')
$(TOITLSP_BIN): $(TOITLSP_SOURCE)
	cd tools/toitlsp; $(GO_BUILD_FLAGS) go build  -ldflags "$(GO_LINK_FLAGS)" -tags 'netgo osusergo' -o "$(CURDIR)"/$@ .

.PHONY: toitlsp
toitlsp: $(TOITLSP_BIN)

.PHONY: toitpkg
toitpkg: $(TOITPKG_BIN)

TOITPKG_VERSION := v0.0.0-20211126161923-c00da039da00
$(TOITPKG_BIN):
	GOBIN="$(CURDIR)"/$(dir $@) go install github.com/toitlang/tpkg/cmd/toitpkg@$(TOITPKG_VERSION)

build/host/:
	mkdir -p $@

build/$(ESP32_CHIP)/: check-env
	mkdir -p $@
	make -C toolchains/$(ESP32_CHIP) -s "$(CURDIR)"/build/$(ESP32_CHIP)/include/sdkconfig.h

build/riscv64/ build/arm64/ build/arm32/ build/win64/ build/win32/:
	mkdir -p $@

.PHONY:	clean check-env
clean:
	rm -rf build/

check-env:
ifeq ("$(wildcard $(IDF_PATH)/components/mbedtls/mbedtls/LICENSE)","")
ifeq ("$(IDF_PATH)", "$(CURDIR)/third_party/esp-idf")
	$(error mbedtls sources are missing. Did you initialize the submodules?)
else
	$(error Invalid IDF_PATH. Missing mbedtls sources.)
endif
endif
ifneq ("$(IDF_PATH)", "$(CURDIR)/third_party/esp-idf")
	$(info -- Not using Toitware ESP-IDF fork.)
endif

.PHONY: install-sdk install

install-sdk: $(TOOLS) $(SNAPSHOTS)
	install -D --target-directory="$(DESTDIR)$(prefix)"/bin $(TOOLS)
	install -m 644 -D --target-directory="$(DESTDIR)$(prefix)"/bin $(TOIT_BOOT_SNAPSHOT)
	cp -R "$(CURDIR)"/lib "$(DESTDIR)$(prefix)"/lib
	find "$(DESTDIR)$(prefix)"/lib -type f -exec chmod 644 {} \;
	install -m 644 -D --target-directory="$(DESTDIR)$(prefix)"/snapshots $(SNAPSHOTS)

install: install-sdk


.PHONY: test
test:
	(cd build/host && ninja check_slow)
