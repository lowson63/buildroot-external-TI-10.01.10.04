
################################### Important ##################################
# This script assuems libnl was configured and built. Please provide the paths
# to it's includes and compiled libraries via the following paths:
NL_HEADERS=" "                              # ..../include/libnl3
NL_LIB=" "                                  # Where libnl-genl-3.a can be found

# Specify where the toolchain used for cross-compiling is located:
export TOOLCHAIN_PATH=" "         # (e.g, ../arm/bin)
export CROSS_COMPILE=" "          # compiler prefix (e.g, arm-linux-gnueabihf- )
export ARCH=" "                   # CPU arch (e.g, arm)
################################################################################

#### Internal variables ####
export PATH=$TOOLCHAIN_PATH:$PATH

NLVER=3 \
    CFLAGS+="-I$NL_HEADERS" \
    LIBS+="-L$NL_LIB -lnl-genl-3 -lnl-3 -lpthread" \
    make
