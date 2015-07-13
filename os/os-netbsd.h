#ifndef FIO_OS_NETBSD_H
#define FIO_OS_NETBSD_H

#define	FIO_OS	os_netbsd

#include <errno.h>
#include <lwp.h>
#include <sys/param.h>
/* XXX hack to avoid confilcts between rbtree.h and <sys/rb.h> */
#define	rb_node	_rb_node
#include <sys/sysctl.h>
#undef rb_node
#undef rb_left
#undef rb_right
#define _NetBSD_SOURCE
#include <sched.h>
#include <unistd.h>

#include "../file.h"

#define FIO_HAVE_ODIRECT
#define FIO_USE_GENERIC_BDEV_SIZE
#define FIO_USE_GENERIC_RAND
#define FIO_USE_GENERIC_INIT_RANDOM_STATE
#define FIO_HAVE_GETTID
#define	FIO_HAVE_CPU_AFFINITY
#define	FIO_MAX_CPUS 1024

typedef	cpuset_t os_cpu_mask_t;

static inline int fio_cpu_isset(os_cpu_mask_t *mask, const cpuid_t cpu)
{
	return cpuset_isset(cpu, mask);
}

static inline int fio_setaffinity(int tid, os_cpu_mask_t *cpumask)
{
	printf("TID: %d\n", tid);
	return _sched_setaffinity(getpid(), tid, cpuset_size(cpumask), cpumask);
}

static inline int fio_getaffinity(int tid, os_cpu_mask_t *cpumask)
{
	printf("TID: %d\n", tid);
	return _sched_getaffinity(getpid(), tid, cpuset_size(cpumask), cpumask);
}

static inline int fio_cpuset_init(os_cpu_mask_t *mask)
{
	cpuset_zero(mask);
	return 0;
}

static inline void fio_cpu_set(os_cpu_mask_t *mask, int cpu)
{
	cpuset_set(cpu, mask);
	return;
}

static inline int fio_cpuset_exit(os_cpu_mask_t *mask)
{
	return 0;
}

static inline int fio_cpu_count(os_cpu_mask_t *mask)
{
	int cpu = sysconf(_SC_NPROCESSORS_CONF);
	int count = 0;

	for (int i = 0; i < cpu; i++) {
		if(cpuset_isset(i, mask)) {
			count++;
		}
	}
	return count;
}

static inline void fio_cpu_clear(os_cpu_mask_t *mask, int cpu)
{
	cpuset_clr(cpu, mask);
	return;
}

#define OS_MAP_ANON		MAP_ANON

#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 4096
#endif

#define fio_swap16(x)	bswap16(x)
#define fio_swap32(x)	bswap32(x)
#define fio_swap64(x)	bswap64(x)

typedef off_t off64_t;

static inline int blockdev_invalidate_cache(struct fio_file *f)
{
	return EINVAL;
}

static inline unsigned long long os_phys_mem(void)
{
	int mib[2] = { CTL_HW, HW_PHYSMEM64 };
	uint64_t mem;
	size_t len = sizeof(mem);

	sysctl(mib, 2, &mem, &len, NULL, 0);
	return mem;
}

static inline int gettid(void)
{
	return (int) _lwp_self();
}

#ifdef MADV_FREE
#define FIO_MADV_FREE	MADV_FREE
#endif

/* XXX NetBSD doesn't have getopt_long_only */
#define	getopt_long_only	getopt_long

#endif
