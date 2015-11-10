
SRC_URI = "file://linux-aspeed-2.6.28.9 \
           file://patch-2.6.28.9/0001-MTD-fix-m25p80-64-bit-divisions.patch \
           file://patch-2.6.28.9/0005-mtd-Bug-in-m25p80.c-during-whole-chip-erase.patch \
           file://patch-2.6.28.9/0006-mtd-fix-timeout-in-M25P80-driver.patch \
           file://patch-2.6.28.9/0009-mtd-m25p80-timeout-too-short-for-worst-case-m25p16-d.patch \
           file://patch-2.6.28.9/0010-mtd-m25p80-fix-null-pointer-dereference-bug.patch \
           file://patch-2.6.28.9/0012-mtd-m25p80-add-support-for-AAI-programming-with-SST-.patch \
           file://patch-2.6.28.9/0020-mtd-m25p80-make-command-buffer-DMA-safe.patch \
           file://patch-2.6.28.9/0021-mtd-m25p80-Add-support-for-CAT25xxx-serial-EEPROMs.patch \
           file://patch-2.6.28.9/0022-mtd-m25p80-Add-support-for-Macronix-MX25L25635E.patch \
           file://patch-2.6.28.9/0023-mtd-m25p80-Add-support-for-Macronix-MX25L25655E.patch \
           file://patch-2.6.28.9/0024-mtd-m25p80-add-support-for-Micron-N25Q256A.patch \
           file://patch-2.6.28.9/0025-mtd-m25p80-add-support-for-Micron-N25Q128.patch \
           file://patch-2.6.28.9/0026-mtd-m25p80-modify-info-for-Micron-N25Q128.patch \
           file://patch-2.6.28.9/0027-mtd-m25p80-n25q064-is-Micron-not-Intel-Numonyx.patch \
           file://patch-2.6.28.9/0028-ipv6-Plug-sk_buff-leak-in-ipv6_rcv-net-ipv6-ip6_inpu.patch \
           file://patch-2.6.28.9/0029-mtd-m25p80-Add-support-for-the-Winbond-W25Q64.patch \
           file://patch-2.6.28.9/0001-bzip2-lzma-library-support-for-gzip-bzip2-and-lzma-d.patch \
           file://patch-2.6.28.9/0002-bzip2-lzma-config-and-initramfs-support-for-bzip2-lz.patch \
          "

LINUX_VERSION ?= "2.6.28.9"
LINUX_VERSION_EXTENSION ?= "-aspeed"

PR = "r1"
PV = "${LINUX_VERSION}"

include linux-aspeed.inc

# Install bounds.h for external module install
# The default install script handles this. However, it looks for bounds.h from
# 'include/generated', which doesnot match 2.6.28, where the file is in
# 'include/linux'.
do_install[postfuncs] += "install_bounds_h"
install_bounds_h() {
	kerneldir=${D}${KERNEL_SRC_PATH}
	if [ -f include/linux/bounds.h ]; then
		cp -l include/linux/bounds.h $kerneldir/include/linux/bounds.h
	fi
}
