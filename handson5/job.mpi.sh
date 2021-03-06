#!/bin/bash
#BSUB -J "SA-MIRI"
#BSUB -o %J.out
#BSUB -e %J.err
#BSUB -W 00:55
#BSUB -x
#BSUB -R"span[ptile=16]"

SIZE=1024
echo $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time
mpirun mvmmpi $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time

SIZE=2048
echo $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time
mpirun mvmmpi $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time

SIZE=4096
echo $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time
mpirun mvmmpi $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time

SIZE=8192
echo $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time
mpirun mvmmpi $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time

SIZE=16384
echo $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time
mpirun mvmmpi $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time

SIZE=32768
echo $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time
mpirun mvmmpi $SIZE >>$LSB_MAX_NUM_PROCESSORS.proc.mpi.time
