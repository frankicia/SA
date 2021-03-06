#!/bin/bash
#BSUB -J "SA-MIRI"
#BSUB -o %J.out
#BSUB -e %J.err
#BSUB -W 01:00
#BSUB -x
#BSUB -R"span[ptile=16]"

SIZE=1024
echo $SIZE >>seq.time
mvmseq $SIZE >>seq.time

SIZE=2048
echo $SIZE >>seq.time
mvmseq $SIZE >>seq.time

SIZE=4096
echo $SIZE >>seq.time
mvmseq $SIZE >>seq.time

SIZE=8192
echo $SIZE >>seq.time
mvmseq $SIZE >>seq.time

SIZE=16384
echo $SIZE >>seq.time
mvmseq $SIZE >>seq.time

SIZE=32768
echo $SIZE >>seq.time
mvmseq $SIZE >>seq.time
