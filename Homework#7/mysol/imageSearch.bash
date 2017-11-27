#!/bin/bash

# Lines beginning with # are comments Only lines beginning #PBS are
# processed by qsub

#PBS -N report_8_Threads_version
#PBS -l walltime=32:25:00
#PBS -l mem=2GB
#PBS -S /bin/bash
#PBS -j oe

cd $PBS_O_WORKDIR

# Convenience function to run multiple times with a given number
# of threads.
#   $1: Parameter for number of threads
function run5Times() {
	# Set number of OpenMP threads based on number of cores specified as
	# parameter to this function.
	export OMP_NUM_THREADS=$1
	echo "-----------[ Statistics for $OMP_NUM_THREADS threads Mamogram]------------"
	# Run the program 5 times to get measure consistent timings
	/usr/bin/time -v ./Homework7 Mammogram.png Cancer_mask.png Mammo_result.png true 75 32
	/usr/bin/time -v ./Homework7 Mammogram.png Cancer_mask.png Mammo_result.png true 75 32
	/usr/bin/time -v ./Homework7 Mammogram.png Cancer_mask.png Mammo_result.png true 75 32
	/usr/bin/time -v ./Homework7 Mammogram.png Cancer_mask.png Mammo_result.png true 75 32
	/usr/bin/time -v ./Homework7 Mammogram.png Cancer_mask.png Mammo_result.png true 75 32
	echo "-----------[ End of Test for Mammogram.png]------------"
	echo "-----------[ Statistics for Test Image with and_mask.png]------------"
	/usr/bin/time -v ./Homework7 TestImage.png and_mask.png testimage_and_mask_result.png true 75 16
	/usr/bin/time -v ./Homework7 TestImage.png and_mask.png testimage_and_mask_result.png true 75 16
	/usr/bin/time -v ./Homework7 TestImage.png and_mask.png testimage_and_mask_result.png true 75 16
	/usr/bin/time -v ./Homework7 TestImage.png and_mask.png testimage_and_mask_result.png true 75 16
	/usr/bin/time -v ./Homework7 TestImage.png and_mask.png testimage_and_mask_result.png true 75 16	
	echo "-----------[ End of Test image with and_mask.png ]------------"
	echo "-----------[ Statistics for MiamiMarcumCenter]------------"
	/usr/bin/time -v ./Homework7 MiamiMarcumCenter.png WindowPane_mask.png marcum_result.png true 50 64
	/usr/bin/time -v ./Homework7 MiamiMarcumCenter.png WindowPane_mask.png marcum_result.png true 50 64
	/usr/bin/time -v ./Homework7 MiamiMarcumCenter.png WindowPane_mask.png marcum_result.png true 50 64
	/usr/bin/time -v ./Homework7 MiamiMarcumCenter.png WindowPane_mask.png marcum_result.png true 50 64
	/usr/bin/time -v ./Homework7 MiamiMarcumCenter.png WindowPane_mask.png marcum_result.png true 50 64
	echo "-----------[ End of Marcum Center]"

#	echo "-----------[ Statistics for TestImage.png i_mask.png]------------"
#	/usr/bin/time -v ./Homework7 TestImage.png i_mask.png testImage_i_mask_result.png true
#	echo "-----------[ End of Marcum Center]"
}

# The main part of the script that runs the program 5 times
#run5Times 1
#run5Times 4
run5Times 8
#end of script
