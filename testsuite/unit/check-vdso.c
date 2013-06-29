/*
 * VDSO feature set testcase
 * by Wolfgang Mauerer <wolfgang.mauerer@siemens.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <asm/xenomai/syscall.h>
#include <cobalt/kernel/vdso.h>

extern unsigned long cobalt_sem_heap[2];

extern struct xnvdso *vdso;

int main(int argc, char **argv)
{
	unsigned long long test_features;

	if (argc != 2) {
		printf("No specific feature(s) given, using XNVDSO_FEATURES\n");
		test_features = XNVDSO_FEATURES;
	} else
		test_features = strtoull(argv[1], NULL, 0);

	if (cobalt_sem_heap[1] == 0) {
		fprintf(stderr, "Could not determine position of the "
			"global semaphore heap\n");
		return 1;
	}

	printf("Contents of the features flag: %llu\n", vdso->features);

	if (vdso->features == test_features)
		return 0;

	fprintf(stderr, "error: vdso->features != 0x%llx\n", test_features);

	return 1;
}