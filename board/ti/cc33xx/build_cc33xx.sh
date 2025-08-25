#! /bin/bash

set -e # exit on errors


function print_usage ()
{
    echo ""
    echo "This script build all/one of the relevant cc33xx software for Buildroot."
    echo ""
    echo "Usage : "
    echo ""
    echo "Building full package : "
    echo " ./build_cc33xx.sh patch		[ Add cc33xx driver and configuration to PSDK's kernel ]"
    echo " ./build_cc33xx.sh build		[ Build calibrator and cc33xx-conf ]"
    echo " ./build_cc33xx.sh install	[ copy to rootfs ]"

    exit 1
}


function main()
{
	case "$1" in
		'patch')
		# TODO: not test yet
		cd patches
		patch -p1 --directory="/media/buildroot-2024.05.3/output/build/linux-10.01.10/" < ./cc33xx_kernel.patch
		patch -p1 --directory="/media/buildroot-2024.05.3/output/build/linux-10.01.10/" < ./cc33xx_am625-sk_dts.patch
		patch -p1 --directory="/media/buildroot-2024.05.3/output/build/linux-10.01.10/" < ./cc33xx_ti_arm64_config.patch
		;;
			  
		'build')
		cd calibrator_src
		./build_calibrator.sh
		cd ../cc33conf_src
		gcc main.c crc32.c -o cc33xxconf.a64
		./cc33xxconf.a64 -S conf.h -G struct.bin
		./cc33xxconf.a64 -D
		cd ..
		;;

		'install')
		echo "install to rootfs-overlay"
		cd calibrator_src
		cp calibrator /media/buildroot-external-TI-10.01.10.04/board/ti/am62x-sk/rootfs-overlay/usr/bin
		cd ../usr
		cp sbin/cc33conf /media/buildroot-external-TI-10.01.10.04/board/ti/am62x-sk/rootfs-overlay/usr -r
		cd ../cc33conf_src
		cp cc33xx-conf-default.bin /media/buildroot-external-TI-10.01.10.04/board/ti/am62x-sk/rootfs-overlay/lib/firmware/ti-connectivity/cc33xx-conf.bin
		cd ../usr
		cp share/cc33xx /media/buildroot-external-TI-10.01.10.04/board/ti/am62x-sk/rootfs-overlay/usr -r
		;;

		'-h')
		print_usage
		;;

		'--help')
		print_usage
		;;

		*)
		echo "**** Unknown action '$1' - please see usage below **** "
		print_usage
		;;

	esac

	return 0
}


main $@