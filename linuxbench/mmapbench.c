// #include <litc.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/mman.h>

static long
nowms(void)
{
  struct timeval tv;
  if (gettimeofday(&tv, NULL)) {
    printf ("nowms");
    exit(1);
  }
  return tv.tv_sec*1000 + tv.tv_usec/1000;
}

#define SZ  (1 << 22)
#define N  1024

int main(int argc, char **argv)
{

  printf("mmapbench\n");

  const size_t sz = SZ;
  long tot = 0;
  for (int i = 0 ; i < N; i++) {
    char *p = mmap(NULL, sz, PROT_READ | PROT_WRITE,
		   MAP_ANON | MAP_PRIVATE, -1, 0);
    if (p == MAP_FAILED) {
      printf("mmap failed");
    exit(1);
    }

    long st = nowms();
    for (int j = 0; j < sz; j += 4096) {
      p[j] = 0xcc;
    }

    tot += nowms() - st;

    if (munmap(p, sz) != 0)
    {
      printf("mmap failed");
      exit(1);
    }
   }
  printf("mmapbench %ld pgfaults in %ld ms\n", (long) (SZ/4096) * N, tot);

  return 0;
}
