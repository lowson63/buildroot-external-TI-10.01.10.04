CROSS_COMPILE = /media/buildroot-2024.05.3/output/host/bin/aarch64-buildroot-linux-gnu-
CC = $(CROSS_COMPILE)gcc
CFLAGS += -O2 -Wall -DCONFIG_LIBNL32
CFLAGS += -I/media/buildroot-2024.05.3/output/target/usr/include -I/media/buildroot-2024.05.3/output/target/include
# Add the path to the libnl headers
CFLAGS += -I/media/buildroot-2024.05.3/output/build/libnl-3.9.0/include
# Add library paths and link necessary libraries
LDFLAGS += -L/media/buildroot-2024.05.3/output/target/usr/lib -L/media/buildroot-2024.05.3/output/target/lib
LIBS += -lnl-3 -lnl-route-3 -lpthread -lc
# Add libnl-genl-3 to the linked libraries
LIBS += -lnl-genl-3

OBJS = calibrator.o cc33xx_plt.o  cc33xx_debug.o

%.o: %.c calibrator.h nl80211.h plt.h nvs_dual_band.h cc33xx_plt.h
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS) 
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o calibrator

uim:
	$(CC) $(CFLAGS) $(LDFLAGS) uim_rfkill/$@.c -o $@

static: $(OBJS) 
	$(CC) $(LDFLAGS) --static $(OBJS) $(LIBS) -o calibrator

install:
	@echo Copy files to $(NFSROOT)/usr/bin
	@cp -f ./calibrator $(NFSROOT)/usr/bin
	@chmod 755 $(NFSROOT)/usr/bin/calibrator

clean:
	@rm -f *.o calibrator uim
