
KERNEL_DIR := /lib/modules/$(shell uname -r)/build

EXTRA_CFLAGS += -I$(obj)/../../common/Headers  -I$(obj)/../../common/atomic -D_LOOSE_KERNEL_NAMES -DMODVERSIONS -D__Pentium__ -DCONFIG_KERNELD -DMODULE -D__KERNEL__ -DLINUX

obj-m := midishare.o
midishare-objs := msLoader.o MidiShareLinux.o MidiShare.o msConf.o \
      msAlarms.o msAppls.o msConnx.o \
      msFilter.o msMail.o msTasks.o msXmtRcv.o \
      msHandler.o msInit.o msSmpte.o msTime.o \
      msEvents.o msFields.o msMemory.o msSeq.o \
      msSorter.o msDriver.o lffifoIntel.o lflifoIntel.o

ifndef KERNELRELEASE

A = atomic
C = Clients
K = Kernel
M = Memory
S = Sorter
D = Drivers

COMMON := $(A)/lffifoIntel.c $(A)/lflifoIntel.c \
$(C)/msAlarms.c $(C)/msAppls.c $(C)/msConnx.c $(C)/msFilter.c \
$(C)/msMail.c $(C)/msTasks.c $(C)/msXmtRcv.c \
$(K)/msHandler.c $(K)/msInit.c $(K)/msSmpte.c $(K)/msTime.c \
$(M)/msEvents.c $(M)/msFields.c $(M)/msMemory.c $(M)/msSeq.c \
$(S)/msSorter.c $(D)/msDriver.c

all:
	for x in $(COMMON); do ln -sf ../../common/$$x `basename $$x`; done
	$(MAKE) modules -C $(KERNEL_DIR) SUBDIRS=$(shell pwd)

clean:
	-rm -f *.ko *.o
	-rm -f *.mod.* .*.cmd
	-for x in $(COMMON); do rm -f `basename $$x`; done

endif
# DO NOT DELETE

MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/config/modversions.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kernel.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/linkage.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/config.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/autoconf.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rhconfig.h
MidiShare.o: /boot/kernel.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/kgdb.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/linkage.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/stddef.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/types.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/posix_types.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/posix_types.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/types.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler-gcc3.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler-gcc.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/byteorder.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/little_endian.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/swab.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/generic.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/bug.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/uaccess.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/errno.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/errno.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/errno.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/errno-base.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/thread_info.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/bitops.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/bitops.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/thread_info.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/page.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/processor.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/vm86.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/math_emu.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/sigcontext.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/segment.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/cpufeature.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/msr.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/system.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/cache.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/cache.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/threads.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/prefetch.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/string.h
MidiShare.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/string.h
MidiShare.o: ../../common/Headers/msKernel.h ../../common/Headers/msDefs.h
MidiShare.o: ../../common/Headers/msTypes.h ../../common/Headers/msAppls.h
MidiShare.o: ../../common/Headers/msMemory.h ../../common/Headers/msDriver.h
MidiShare.o: ../../common/Headers/msSorter.h ../../common/Headers/msInit.h
MidiShare.o: msLoader.h msIOCtl.h ../../common/Headers/msAppFun.h
MidiShare.o: ../../common/Headers/msDrvFun.h ../../common/Headers/msTasks.h
MidiShare.o: ../../common/Headers/msXmtRcv.h ../../common/Headers/msEvents.h
MidiShare.o: ../../common/Headers/msConnx.h ../../common/Headers/msFilter.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/config/modversions.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/version.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rhconfig.h
MidiShareLinux.o: /boot/kernel.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/slab.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/config.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/autoconf.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/kgdb.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/gfp.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/mmzone.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/spinlock.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/preempt.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/linkage.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/linkage.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler-gcc3.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler-gcc.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/thread_info.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/bitops.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/types.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/bitops.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/thread_info.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/page.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/processor.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/vm86.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/math_emu.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/sigcontext.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/segment.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/cpufeature.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/msr.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/system.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kernel.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/stddef.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/types.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/posix_types.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/posix_types.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/byteorder.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/little_endian.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/swab.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/generic.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/bug.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/cache.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/cache.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/threads.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/stringify.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/spinlock.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/atomic.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/rwlock.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/list.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/prefetch.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/wait.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/numa.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/topology.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/cpumask.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/bitmap.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/string.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/string.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/smp.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/smp.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/fixmap.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/acpi.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/apicdef.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/kmap_types.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/mpspec.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/mpspec_def.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/io_apic.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/apic.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/pm.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/topology.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/topology.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kmalloc_sizes.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/poll.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/poll.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/mm.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sched.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/param.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/capability.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/timex.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/timex.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/time.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/seqlock.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/div64.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/jiffies.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rbtree.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/semaphore.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rwsem.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/rwsem.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/ptrace.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/mmu.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sem.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/ipc.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/ipcbuf.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/sembuf.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/signal.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/signal.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/siginfo.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/siginfo.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/securebits.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/fs_struct.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/completion.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/pid.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/percpu.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/percpu.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/percpu.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/param.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/resource.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/resource.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/timer.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/aio.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/workqueue.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/aio_abi.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/current.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/errno.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/errno.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/errno.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/errno-base.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/prio_tree.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/fs.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/limits.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kdev_t.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/ioctl.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/ioctl.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dcache.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rcupdate.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/stat.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/stat.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kobject.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sysfs.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/radix-tree.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/audit.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/init.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/quota.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dqblk_xfs.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dqblk_v1.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dqblk_v2.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/nfs_fs_i.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/nfs.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sunrpc/msg_prot.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/fcntl.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/fcntl.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/err.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/pgtable.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/pgtable-2level-defs.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/pgtable-2level.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/pgtable.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/page-flags.h
MidiShareLinux.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/uaccess.h
MidiShareLinux.o: msLoader.h ../../common/Headers/msKernel.h
MidiShareLinux.o: ../../common/Headers/msDefs.h
MidiShareLinux.o: ../../common/Headers/msTypes.h
MidiShareLinux.o: ../../common/Headers/msAppls.h
MidiShareLinux.o: ../../common/Headers/msMemory.h
MidiShareLinux.o: ../../common/Headers/msDriver.h
MidiShareLinux.o: ../../common/Headers/msSorter.h msIOCtl.h
MidiShareLinux.o: ../../common/Headers/msExtern.h
MidiShareLinux.o: ../../common/Headers/msTasks.h
MidiShareLinux.o: ../../common/Headers/msSync.h
MidiShareLinux.o: ../../common/Headers/msEvents.h
MidiShareLinux.o: ../../common/Headers/msAppFun.h
MidiShareLinux.o: ../../common/Headers/msMem.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/config/modversions.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/fs.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/config.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/autoconf.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rhconfig.h
msConf.o: /boot/kernel.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/kgdb.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/linkage.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/linkage.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/limits.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/wait.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/list.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/stddef.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/prefetch.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/types.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/posix_types.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/posix_types.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/types.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/processor.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/vm86.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/math_emu.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/sigcontext.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler-gcc3.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler-gcc.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/segment.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/page.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/cpufeature.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/bitops.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/bitops.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/msr.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/system.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kernel.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/byteorder.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/little_endian.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/swab.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/generic.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/bug.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/cache.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/cache.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/threads.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/spinlock.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/preempt.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/thread_info.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/thread_info.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/stringify.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/spinlock.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/atomic.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/rwlock.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kdev_t.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/ioctl.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/ioctl.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dcache.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rcupdate.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/percpu.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/slab.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/gfp.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/mmzone.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/numa.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/topology.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/cpumask.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/bitmap.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/string.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/string.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/smp.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/smp.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/fixmap.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/acpi.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/apicdef.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/kmap_types.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/mpspec.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/mpspec_def.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/io_apic.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/apic.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/pm.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/topology.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/topology.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kmalloc_sizes.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/percpu.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/percpu.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/seqlock.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/stat.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/stat.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/time.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/param.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/timex.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/timex.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/div64.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/prio_tree.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kobject.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sysfs.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rwsem.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/rwsem.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/radix-tree.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/audit.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/init.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/semaphore.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/quota.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/errno.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/errno.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/errno.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/errno-base.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dqblk_xfs.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dqblk_v1.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dqblk_v2.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/nfs_fs_i.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/nfs.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sunrpc/msg_prot.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/fcntl.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/fcntl.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/err.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/current.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sched.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/capability.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/jiffies.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rbtree.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/ptrace.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/mmu.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sem.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/ipc.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/ipcbuf.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/sembuf.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/signal.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/signal.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/siginfo.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/siginfo.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/securebits.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/fs_struct.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/completion.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/pid.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/param.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/resource.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/resource.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/timer.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/aio.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/workqueue.h
msConf.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/aio_abi.h
msConf.o: msConf.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/config/modversions.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/module.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/config.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/autoconf.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rhconfig.h
msLoader.o: /boot/kernel.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/kgdb.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sched.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/param.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/capability.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/types.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/posix_types.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/stddef.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/posix_types.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/types.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler-gcc3.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/compiler-gcc.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/spinlock.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/preempt.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/linkage.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/linkage.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/thread_info.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/bitops.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/bitops.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/thread_info.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/page.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/processor.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/vm86.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/math_emu.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/sigcontext.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/segment.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/cpufeature.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/msr.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/system.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kernel.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/byteorder.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/little_endian.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/swab.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/byteorder/generic.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/bug.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/cache.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/cache.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/threads.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/stringify.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/spinlock.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/atomic.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/rwlock.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/timex.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/timex.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/time.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/seqlock.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/div64.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/jiffies.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rbtree.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/cpumask.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/bitmap.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/string.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/string.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/semaphore.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/wait.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/list.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/prefetch.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rwsem.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/rwsem.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/ptrace.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/mmu.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/smp.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/smp.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/fixmap.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/acpi.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/apicdef.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/kmap_types.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/mpspec.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/mpspec_def.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/io_apic.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/apic.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/pm.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sem.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/ipc.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/ipcbuf.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/sembuf.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/signal.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/signal.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/siginfo.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/siginfo.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/securebits.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/fs_struct.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/completion.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/pid.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/percpu.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/slab.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/gfp.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/mmzone.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/numa.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/topology.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/topology.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/topology.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kmalloc_sizes.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/percpu.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/percpu.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/param.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/resource.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/resource.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/timer.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/aio.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/workqueue.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/aio_abi.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/current.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/stat.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/stat.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kmod.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/errno.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/errno.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/errno.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm-generic/errno-base.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/elf.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/elf.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/user.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/utsname.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kobject.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sysfs.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/moduleparam.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/init.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/local.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/module.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/fs.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/limits.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/kdev_t.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/ioctl.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/ioctl.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dcache.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/rcupdate.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/prio_tree.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/radix-tree.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/audit.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/quota.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dqblk_xfs.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dqblk_v1.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/dqblk_v2.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/nfs_fs_i.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/nfs.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/sunrpc/msg_prot.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/fcntl.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/asm/fcntl.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/err.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/device.h
msLoader.o: /lib/modules/2.6.8.1-12mdksmp/build/include/linux/ioport.h
msLoader.o: ../../common/Headers/msAppFun.h ../../common/Headers/msKernel.h
msLoader.o: ../../common/Headers/msDefs.h ../../common/Headers/msTypes.h
msLoader.o: ../../common/Headers/msAppls.h ../../common/Headers/msMemory.h
msLoader.o: ../../common/Headers/msDriver.h ../../common/Headers/msSorter.h
msLoader.o: msLoader.h msIOCtl.h ../../common/Headers/msInit.h msConf.h
msLoader.o: msExports.h ../../common/Headers/msConnx.h
msLoader.o: ../../common/Headers/msDrvFun.h ../../common/Headers/msEvents.h
msLoader.o: ../../common/Headers/msFields.h ../../common/Headers/msFilter.h
msLoader.o: ../../common/Headers/msMail.h ../../common/Headers/msSeq.h
msLoader.o: ../../common/Headers/msSmpte.h ../../common/Headers/msTasks.h
msLoader.o: ../../common/Headers/msTime.h ../../common/Headers/msXmtRcv.h
