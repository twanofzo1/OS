#!/bin/bash
set -euo pipefail

IMAGE=image.hdd
IMAGE_SIZE=64M
PART_START=2048          # 2048 * 512 = 1MiB (alignment)


echo "Building kernel..."
if ! make clean; then
    echo "Error: make clean failed!" >&2
    exit 1
fi
if ! make; then
    echo "Error: kernel build failed!" >&2
    exit 1
fi
if [ ! -f bin/myos ]; then
    echo "Error: Kernel binary not found!" >&2
    exit 1
fi
echo "Kernel built successfully!"


echo "Creating fresh disk image (${IMAGE_SIZE})..."
if ! rm -f "$IMAGE"; then
    echo "Error: failed to remove old disk image!" >&2
    exit 1
fi
if ! truncate -s "$IMAGE_SIZE" "$IMAGE"; then
    echo "Error: failed to create disk image!" >&2
    exit 1
fi

# Create a fresh GPT and add a single EFI System Partition spanning the rest of the disk.
# NOTE: sgdisk -n <num>:<start>:<end> requires both colons. Leaving <end> blank uses last usable sector.

if ! PATH=$PATH:/usr/sbin:/sbin sgdisk -Z "$IMAGE" >/dev/null 2>&1; then
    echo "Warning: failed to zap old partition tables (may be fine)." >&2
fi
if ! PATH=$PATH:/usr/sbin:/sbin sgdisk -og "$IMAGE"; then
    echo "Error: failed to create new GPT!" >&2
    exit 1
fi
if ! PATH=$PATH:/usr/sbin:/sbin sgdisk -n 1:${PART_START}: -t 1:ef00 -m 1 "$IMAGE"; then
    echo "Error: failed to create EFI partition!" >&2
    exit 1
fi

# Fetch Limine (binary branch) only if not already present; otherwise update.

if [ -d limine/.git ]; then
    echo "Updating existing Limine repo..."
    if ! git -C limine fetch --depth=1 origin v9.x-binary; then
        echo "Error: failed to fetch Limine repo!" >&2
        exit 1
    fi
    if ! git -C limine reset --hard FETCH_HEAD; then
        echo "Error: failed to reset Limine repo!" >&2
        exit 1
    fi
else
    echo "Cloning Limine..."
    if ! git clone https://codeberg.org/Limine/Limine.git limine --branch=v9.x-binary --depth=1; then
        echo "Error: failed to clone Limine repo!" >&2
        exit 1
    fi
fi


echo "Building Limine tools..."
if ! make -C limine; then
    echo "Error: failed to build Limine tools!" >&2
    exit 1
fi


echo "Installing Limine BIOS stages..."
if ! ./limine/limine bios-install "$IMAGE"; then
    echo "Error: failed to install Limine BIOS stages!" >&2
    exit 1
fi

# The first partition starts at 2048 * 512 = 1MiB. mtools syntax image@@offset.
PART_OFFSET=$((PART_START * 512))


echo "Formatting EFI System Partition as FAT32..."
if ! mformat -i ${IMAGE}@@${PART_OFFSET}; then
    echo "Error: failed to format EFI System Partition!" >&2
    exit 1
fi


echo "Creating directory structure inside image..."
if ! mmd -i ${IMAGE}@@${PART_OFFSET} ::/EFI ::/EFI/BOOT ::/boot ::/boot/limine; then
    echo "Warning: failed to create some directories (may already exist)." >&2
fi


echo "Copying kernel and Limine files..."
if ! mcopy -i ${IMAGE}@@${PART_OFFSET} bin/myos ::/boot; then
    echo "Error: failed to copy kernel to image!" >&2
    exit 1
fi
if ! mcopy -i ${IMAGE}@@${PART_OFFSET} limine.conf limine/limine-bios.sys ::/boot/limine; then
    echo "Error: failed to copy Limine BIOS files!" >&2
    exit 1
fi
if ! mcopy -i ${IMAGE}@@${PART_OFFSET} limine/BOOTX64.EFI ::/EFI/BOOT; then
    echo "Error: failed to copy BOOTX64.EFI!" >&2
    exit 1
fi
if [ -f limine/BOOTIA32.EFI ]; then
    if ! mcopy -i ${IMAGE}@@${PART_OFFSET} limine/BOOTIA32.EFI ::/EFI/BOOT; then
        echo "Warning: failed to copy BOOTIA32.EFI!" >&2
    fi
fi


if ! qemu-system-x86_64 -hda image.hdd -m 512M; then
    echo "Error: QEMU failed to start or exited with error!" >&2
    exit 1
fi