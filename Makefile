CC = gcc
AS = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -Wall -Wextra
LDFLAGS = -m elf_i386 -T linker.ld

BUILD = build
KERNEL = kernel.bin

OBJS = \
$(BUILD)/entry.o \
$(BUILD)/kernel.o \
$(BUILD)/screen.o \
$(BUILD)/keyboard.o \
$(BUILD)/shell.o \
$(BUILD)/string.o \
$(BUILD)/fs.o \
$(BUILD)/heap.o \
$(BUILD)/idt.o \
$(BUILD)/pic.o \
$(BUILD)/irq.o \
$(BUILD)/pit.o \
$(BUILD)/irq_stub.o \
$(BUILD)/idt_load.o 
all: $(KERNEL)

# Create build directory
$(BUILD):
	mkdir -p $(BUILD)

# Assembly files
$(BUILD)/entry.o: boot/entry.asm | $(BUILD)
	$(AS) -f elf32 $< -o $@

$(BUILD)/irq_stub.o: kernel/interrupts/irq_stub.asm | $(BUILD)
	$(AS) -f elf32 $< -o $@

$(BUILD)/idt_load.o: kernel/interrupts/idt_load.asm | $(BUILD)
	$(AS) -f elf32 $< -o $@

# C source files
$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/screen.o: kernel/drivers/screen.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/keyboard.o: kernel/drivers/keyboard.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/shell.o: kernel/shell.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/fs.o: kernel/fs.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD)/idt.o: kernel/interrupts/idt.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/pic.o: kernel/interrupts/pic.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/irq.o: kernel/interrupts/irq.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/pit.o: kernel/timer/pit.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD)/string.o: kernel/lib/string.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
# Link everything
$(BUILD)/heap.o: kernel/memory/heap.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -rf $(BUILD) $(KERNEL)

run: all
	qemu-system-i386 -kernel $(KERNEL)
