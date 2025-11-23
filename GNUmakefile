.SUFFIXES:

ARCH := x86_64
IMAGE_NAME := osiris-$(ARCH)

HOST_CC := cc
HOST_CFLAGS := -g -O2 -pipe

.PHONY: all
all: $(IMAGE_NAME).iso

.PHONY: run
run: $(IMAGE_NAME).iso
	qemu-system-x86_64 -cdrom $(IMAGE_NAME).iso -m 2G

.PHONY: run-hdd
run-hdd: $(IMAGE_NAME).hdd
	qemu-system-x86_64 -hda $(IMAGE_NAME).hdd -m 2G

limine/limine:
	rm -rf limine
	git clone https://codeberg.org/Limine/Limine.git limine --branch=v10.x-binary --depth=1
	$(MAKE) -C limine \
		CC="$(HOST_CC)" \
		CFLAGS="$(HOST_CFLAGS)"

kernel/.deps-obtained:
	./kernel/get-deps

.PHONY: kernel
kernel: kernel/.deps-obtained
	$(MAKE) -C kernel

$(IMAGE_NAME).iso: limine/limine kernel
	rm -rf iso_root
	mkdir -p iso_root/boot/limine
	mkdir -p iso_root/EFI/BOOT

	cp -v kernel/bin-$(ARCH)/kernel iso_root/boot/

	cp -v limine.conf iso_root/boot/limine/
	cp -v limine/limine-bios.sys limine/limine-bios-cd.bin \
	      limine/limine-uefi-cd.bin iso_root/boot/limine/

	cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
	cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/

	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
	        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
	        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
	        -efi-boot-part --efi-boot-image --protective-msdos-label \
	        iso_root -o $(IMAGE_NAME).iso

	./limine/limine bios-install $(IMAGE_NAME).iso

	rm -rf iso_root

$(IMAGE_NAME).hdd: limine/limine kernel
	rm -f $(IMAGE_NAME).hdd

	# Create 64 MiB blank image
	dd if=/dev/zero bs=1M count=0 seek=64 of=$(IMAGE_NAME).hdd

	# Create GPT with 1 FAT partition (ESP)
	PATH=$$PATH:/usr/sbin:/sbin sgdisk $(IMAGE_NAME).hdd -n 1:2048 -t 1:ef00 -m 1

	# Install limine BIOS stages
	./limine/limine bios-install $(IMAGE_NAME).hdd

	# Format as FAT
	mformat -i $(IMAGE_NAME).hdd@@1M

	# Create directories
	mmd -i $(IMAGE_NAME).hdd@@1M ::/EFI ::/EFI/BOOT ::/boot ::/boot/limine

	# Copy files
	mcopy -i $(IMAGE_NAME).hdd@@1M kernel/bin-$(ARCH)/kernel ::/boot
	mcopy -i $(IMAGE_NAME).hdd@@1M limine.conf limine/limine-bios.sys ::/boot/limine
	mcopy -i $(IMAGE_NAME).hdd@@1M limine/BOOTX64.EFI ::/EFI/BOOT
	mcopy -i $(IMAGE_NAME).hdd@@1M limine/BOOTIA32.EFI ::/EFI/BOOT

.PHONY: clean
clean:
	$(MAKE) -C kernel clean
	rm -rf iso_root $(IMAGE_NAME).iso $(IMAGE_NAME).hdd

.PHONY: distclean
distclean:
	$(MAKE) -C kernel distclean
	rm -rf iso_root *.iso *.hdd kernel-deps limine

.PHONY: format
format:
	find . -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
