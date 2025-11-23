.SUFFIXES:

OUTPUT := kernel

ARCH := x86_64

PREFIX := /usr/local

TOOLCHAIN :=
TOOLCHAIN_PREFIX :=
ifneq ($(TOOLCHAIN),)
    ifeq ($(TOOLCHAIN_PREFIX),)
        TOOLCHAIN_PREFIX := $(TOOLCHAIN)-
    endif
endif

CC  := $(TOOLCHAIN_PREFIX)gcc
CXX := $(TOOLCHAIN_PREFIX)c++
LD  := $(TOOLCHAIN_PREFIX)ld

ifeq ($(TOOLCHAIN),llvm)
    CC := clang
    CXX := clang++
    LD := ld.lld
endif

CFLAGS   := -g -O2 -pipe
CXXFLAGS := $(CFLAGS)
CPPFLAGS :=

CC_IS_CLANG  := $(shell ! $(CC) --version 2>/dev/null | grep -q '^Target: '; echo $$?)
CXX_IS_CLANG := $(shell ! $(CXX) --version 2>/dev/null | grep -q '^Target: '; echo $$?)

SHARED_FLAGS := \
    -Wall -Wextra -nostdinc -ffreestanding \
    -fno-stack-protector -fno-stack-check -fno-lto \
    -fno-PIC -ffunction-sections -fdata-sections

CPPFLAGS_C := \
    -I maat \
    -I limine-protocol/include \
    -isystem freestnd-c-hdrs/$(ARCH)/include \
    $(CPPFLAGS) \
    -MMD -MP

CPPFLAGS_CXX := \
    $(CPPFLAGS_C) \
    -isystem freestnd-cxx-hdrs/$(ARCH)/include

ifeq ($(CC_IS_CLANG),1)
    CC += -target x86_64-unknown-none-elf
endif
ifeq ($(CXX_IS_CLANG),1)
    CXX += -target x86_64-unknown-none-elf
endif

SHARED_FLAGS += \
    -m64 -march=x86-64 -mabi=sysv \
    -mno-80387 -mno-mmx -mno-sse -mno-sse2 \
    -mno-red-zone -mcmodel=kernel

LDFLAGS += \
    -m elf_x86_64 \
    -nostdlib -static \
    -z max-page-size=0x1000 \
    --gc-sections \
    -T linker-scripts/x86_64.lds

NASMFLAGS := -g -F dwarf -Wall -f elf64

CFLAGS   += -std=gnu11 $(SHARED_FLAGS)
CXXFLAGS += -std=gnu++20 -fno-rtti -fno-exceptions $(SHARED_FLAGS)

SRCFILES := $(shell find -L maat cc-runtime/src -type f 2>/dev/null | LC_ALL=C sort)
SRCFILES += $(shell find -L maat/arch/x86_64 -type f 2>/dev/null | LC_ALL=C sort)

CFILES    := $(filter %.c,$(SRCFILES))
CXXFILES  := $(filter %.cpp,$(SRCFILES))
ASFILES   := $(filter %.S,$(SRCFILES))
NASMFILES := $(filter %.asm,$(SRCFILES))

OBJ := $(addprefix obj-$(ARCH)/, \
        $(CFILES:.c=.c.o) \
        $(CXXFILES:.cpp=.cpp.o) \
        $(ASFILES:.S=.S.o) \
        $(NASMFILES:.asm=.asm.o))

HEADER_DEPS := $(addprefix obj-$(ARCH)/, \
        $(CFILES:.c=.c.d) \
        $(CXXFILES:.cpp=.cpp.d) \
        $(ASFILES:.S=.S.d))

all: bin-$(ARCH)/$(OUTPUT)

-include $(HEADER_DEPS)

bin-$(ARCH)/$(OUTPUT): $(OBJ)
	mkdir -p "$(dir $@)"
	$(LD) $(LDFLAGS) $(OBJ) -o $@

obj-$(ARCH)/%.c.o: %.c
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS_C) -c $< -o $@

obj-$(ARCH)/%.cpp.o: %.cpp
	mkdir -p "$(dir $@)"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS_CXX) -c $< -o $@

obj-$(ARCH)/%.S.o: %.S
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS_C) -c $< -o $@

obj-$(ARCH)/%.asm.o: %.asm
	mkdir -p "$(dir $@)"
	nasm $(NASMFLAGS) $< -o $@

clean:
	rm -rf bin-$(ARCH) obj-$(ARCH)

distclean:
	rm -rf bin-* obj-* .deps-obtained freestnd-c-hdrs freestnd-cxx-hdrs cc-runtime limine-protocol

