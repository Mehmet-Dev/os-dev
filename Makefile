# ===== Tools =====
ASM = nasm
CC = gcc
LD = ld

# ===== Flags =====
ASMFLAGS_BIN = -f bin
ASMFLAGS_ELF = -f elf32
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -c -I$(KERNEL_DIR)/headers
LDFLAGS = -m elf_i386 -T linker.ld --oformat binary

# ===== Directories =====
BOOT_DIR = boot
KERNEL_DIR = kernel
BUILD_DIR = build

# ===== Files =====
BOOT_SRC = $(BOOT_DIR)/boot.asm
KERNEL_ASM_SRC = $(KERNEL_DIR)/kernel.asm
KERNEL_C_SRCS = $(wildcard $(KERNEL_DIR)/*.c)

BOOT_BIN = $(BUILD_DIR)/boot.bin
KERNEL_ASM_OBJ = $(BUILD_DIR)/kernel_asm.o
KERNEL_C_OBJS = $(patsubst $(KERNEL_DIR)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_C_SRCS))
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

# ===== Kernel C Files (Generic Rule) =====
# This rule tells Make how to compile ANY .c file it finds into a .o file
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

# ===== Link Kernel (ASM + C -> Bin) =====
$(KERNEL_BIN): $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJS)
	$(LD) $(LDFLAGS) $(KERNEL_ASM_OBJ) $(KERNEL_C_OBJS) -o $@

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
