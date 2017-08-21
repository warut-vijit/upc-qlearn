#!/bin/bash

export MP_DEVTYPE=hfi
export MP_ADAPTER_USE=shared
export MP_INSTANCES=1
export MP_ENDPOINTS=1
export MP_EUIDEVICE=sn_single
export MP_EUILIB=us

export MP_MSG_API=pgas
export MP_SHARED_MEMORY=yes
export MP_EUIDEVELOP=min
export SCI_DEBUG_FANOUT=16

# Uncommenting these will make the output more clear and organized, but slower -- organizing 
# the output takes some more time.
#export MP_EUIDEVELOP=min
export MP_INFOLEVEL=0
export MP_LABELIO=yes
export MP_STDOUTMODE=ordered


#====================================
# SLURM scheduling using the SLURM resource manager
# export MP_COLLECTIVE_GROUPS=4
# export MP_DEBUG_JOBOPT_VALIDATE=no
# export MP_RESD=yes
# export MP_RMPOOL=slurm
# srun -N2 -n4 --extra-node-info=4:1:1  ./upc_qlearn

#====================================
# POE scheduling using POE manual job execution
# NOTES:  MP_ENDPOINTS must be equal to or greater than the number of threads per process - i.e., UPC_NTHREADS/MP_PROCS
unset MP_RMPOOL
export MP_RESD=poe
export MP_HOSTFILE=host.list
export UPC_NTHREADS=20
export MP_PROCS=$UPC_NTHREADS
export MP_ENDPOINTS=1
poe ./upc_qlearn 


# If you want to record and separate your stdout and stderr, do this:
# NOWTIME=`date +%Y%m%d@%H:%M`
# echo "nowtime is ${NOWTIME}"
# poe ./upc_qlearn > upc_qlearn.${NOWTIME}.out 2> upc_qlearn.${NOWTIME}.err

#
# OTHER ENVIRONMENT VARIABLES FOR REFERENCE
# 
#export XLPGAS_PAMI_SETUP_LOG=1
#export MP_BULK_MIN_MSG_SIZE=64k
#export MP_CPU_USE=multiple
#export MP_CSS_INTERRUPT=no
#export MP_EAGER_LIMIT=64k
#export MP_ENDPOINTS=1
#export MP_USE_BULK_XFER=yes
#export MP_DEBUG_CHECK_PARAM=no
#export MP_DEBUG_THREAD_SAFE=no
#export MP_RDMA_ROUTE_MODE=hw_direct
#export MP_IMM_SEND_BUFFERS=32
#export MP_INSTANCES=max
#export MP_MPILIB=pempi
#export MP_RETRY=10
#export MP_RFIFO_SIZE=16M
#export MP_SINGLE_THREAD=yes
#export MP_TASK_AFFINITY=core
#export MP_WAIT_MODE=poll
#export HPCT_NL=yes
#export SHMEM_DEBUG_HFI20_WORKAROUND=no
#export MP_CHECKPOINT=no
#export MP_CLOCK_SOURCE=switch
#export MP_S_BSR_BARRIER=no
#export LD_LIBRARY_PATH=/opt/ibmhpc/pe12014/pempi/gnu/lib/libmpi:/opt/ibmphc/pe12014/base/gnu/lib:/usr/lib
#export MP_EUILIBPATH=/opt/ibmhpc/pe12014/pempi/gnu/lib/libmpi:/opt/ibmhpc/pe12014/base/gnu/lib:/usr/lib
#export MP_DEBUG_ACKFLUSH_THRESHOLD=10

