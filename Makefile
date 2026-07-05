# ===== Tools =====
ASM = nasm
CC = gcc
LD = ld

# ===== Flags =====
ASMFLAGS_BIN = -f bin
ASMFLAGS_ELF = -f elf32
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -c
LDFLAGS = -m elf_i386 -T linker.ld --oformat binary

# ===== Directories =====
BOOT_DIR = boot
KERNEL_DIR = kernel
BUILD_DIR = build

# ===== Files =====
BOOT_SRC = $(BOOT_DIR)/boot.asm
KERNEL_ASM_SRC = $(KERNEL_DIR)/kernel.asm
KERNEL_C_SRC = $(KERNEL_DIR)/kernel.c

BOOT_BIN = $(BUILD_DIR)/boot.bin
KERNEL_ASM_OBJ = $(BUILD_DIR)/kernel_asm.o
KERNEL_C_OBJ = $(BUILD_DIR)/kernel_c.o
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OS_IMAGE = $(BUILD_DIR)/os-image.img

# ===== Default target =====
all: $(OS_IMAGE)

# ===== Bootloader =====
$(BOOT_BIN): $(BOOT_SRC)
	mkdir -p $(BUILD_DIR)
	$(ASM) $(ASMFLAGS_BIN) $< -o $@

# ===== Kernel Entry (ASM) =====
$(KERNEL_ASM_OBJ): $(KERNEL_ASM_SRC)
	mkdir -p $(BUILD_DIR)
	$(ASM) $(ASMFLAGS_ELF) $< -o $@

# ===== Kernel Main (C) =====
$(KERNEL_C_OBJ): $(KERNEL_C_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

# ===== Link Kernel (ASM + C -> Bin) =====
$(KERNEL_BIN): $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ)
	$(LD) $(LDFLAGS) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJ) -o $@

# ===== OS Image =====
$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)

# ===== Run in QEMU =====
run: $(OS_IMAGE)
	qemu-system-x86_64 -drive format=raw,file=$(OS_IMAGE) -d cpu_reset -no-reboot

# ===== Clean =====
clean:
	rm -rf $(BUILD_DIR)

# ===== Rebuild =====
re: clean all
