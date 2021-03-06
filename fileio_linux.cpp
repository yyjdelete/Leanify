#include "fileio.h"

#include <cstdio>
#include <iostream>

#include <sys/mman.h>

#include <fcntl.h>
#include <ftw.h>
#include <unistd.h>

using std::cerr;
using std::endl;

// traverse directory and call Callback() for each file
void TraverseDirectory(const char Dir[], int Callback(const char file_path[], const struct stat* sb, int typeflag)) {
  if (ftw(Dir, Callback, 16))
    perror("ftw");
}

bool IsDirectory(const char path[]) {
  struct stat sb;
  if (!stat(path, &sb))
    return (sb.st_mode & S_IFDIR) != 0;
  return false;
}

File::File(const char* filepath) {
  fp_ = nullptr;
  fd_ = open(filepath, O_RDWR);

  if (fd_ == -1) {
    perror("Open file error");
    return;
  }

  struct stat sb;
  if (fstat(fd_, &sb) == -1) {
    perror("fstat");
    return;
  }
  size_ = sb.st_size;

  // map the file into memory
  fp_ = mmap(nullptr, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
  if (fp_ == MAP_FAILED) {
    perror("Map file error");
    fp_ = nullptr;
  }
}

void File::UnMapFile(size_t new_size) {
  if (munmap(fp_, size_) == -1)
    perror("munmap");
  if (new_size)
    if (ftruncate(fd_, new_size) == -1)
      perror("ftruncate");

  close(fd_);
  fp_ = nullptr;
}
