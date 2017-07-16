#include "glog/logging.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define LENGTH (256UL * 1024 * 1024)
#define PROTECTION (PROT_READ | PROT_WRITE | PROT_EXEC)

#ifndef MAP_HUGETLB
#define MAP_HUGETLB 0x40000 /* arch specific */
#endif

#define ADDR (void *)(0x0UL)
#define FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB)

namespace {

void check_bytes(char *addr) {
  LOG(INFO) << "First hex is " << *((unsigned int *)addr);
}

void write_bytes(char *addr) {
  for (unsigned long i = 0; i < LENGTH; i++)
    *(addr + i) = (char)i;
}

int read_bytes(char *addr) {
  check_bytes(addr);
  for (unsigned long i = 0; i < LENGTH; i++) {
    if (*(addr + i) != (char)i) {
      LOG(ERROR) << "Mismatch at " << i;
      return 1;
    }
  }
  return 0;
}
}  // namespace

void __attribute__((noinline)) moved_func() {
  LOG(INFO) << "This function was moved somewhere else";
}

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  LOG(INFO) << "Starting application";

  void *addr = mmap(ADDR, LENGTH, PROTECTION, FLAGS, -1, 0);
  if (addr == MAP_FAILED) {
    LOG(FATAL) << "mmap failed";
  }

  LOG(INFO) << "Returned address is " << addr;
  check_bytes((char *)addr);
  write_bytes((char *)addr);
  auto ret = read_bytes((char *)addr);

  /* munmap() length of MAP_HUGETLB memory must be hugepage aligned */
  if (munmap(addr, LENGTH)) {
    LOG(FATAL) << "munmap failed";
  }

  moved_func();

  LOG(INFO) << "Exiting application";
  return ret;
}
