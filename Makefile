UPCC = /gpfs/gpfs1/common/upc/12.1.0.3/upc/12.1/bin/xlupc
#UPCC= /gpfs/usr/u/upc/upc/12.1/bin
#       This is the path to the 12.1.0.1 (PTF 1)  version.
#       #       #UPCC = /gpfs/usr/u/annyly/compiler/130502a/upc/12.1/bin/xlupc -L/gpfs/usr/u/irinar/patch -R/gpfs/usr/u/irinar/patch
#       This is an older version.
#       #       #UPCC = /gpfs/usr/u/annyly/compiler/130502a/upc/12.1/bin/xlupc #-L/gpfs/usr/u/irinar/patch -R/gpfs/usr/u/irinar/patch
#UPCC = /gpfs/usr/u/annyly/compiler/121204/upc/12.1/bin/xlupc -L. -R.

# ----------------------------------------------------------------------
#  #
all: upc_qlearn.upc upc_qlearn  Makefile
#upc_qlearn: upc_qlearn.upc pong.upc linalg.upc cblas_LINUX.a 
upc_qlearn: upc_qlearn.upc pong.upc linalg.upc
	${UPCC} $^ -o $@
#       ${UPCC} $^ -qupc=THREADS=4 -qupc=DNODES=4 -qcheck=allocmem -qcheck=collective -qcheck=stack -qcheck=sync -qcheck=uninit -o $@
#       #       ${UPCC} $^ -qupc=threads=4 -qupc=dnodes=4 -o $@
#           if you wanted to specify running with a specific # of threads or nodes

run: upc_qlearn run.upc_qlearn.sh
	./run.upc_qlearn.sh

