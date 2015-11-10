LINUX_VERSION_EXTENSION = "-galaxy100"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
COMPATIBLE_MACHINE = "galaxy100"

KERNEL_CONFIG_COMMAND = "oe_runmake galaxy100_defconfig && oe_runmake oldconfig"
SRC_URI += "file://patch-linux-aspeed-2.6.28.9/0001-add-linux-galaxy100-config.patch"
