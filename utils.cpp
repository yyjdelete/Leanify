#include "utils.h"

#ifdef _WIN32
#include <Windows.h>  // WideCharToMultiByte
#else
#include <iconv.h>  // convert UTF16 to UTF8 on non Windows
#include <cstdio>
#endif

using std::cout;
using std::endl;
using std::string;

// convert UNICODE string aka UTF16 string
// to multi byte string aka UTF8 string
void UTF16toMBS(const wchar_t* u, size_t srclen, char* mbs, size_t dstlen) {
#ifdef _WIN32
  WideCharToMultiByte(CP_ACP, 0, u, srclen / 2, mbs, dstlen, nullptr, nullptr);
#else
  iconv_t conv = iconv_open("UTF-8", "UTF-16");
  if (iconv(conv, (char**)&u, &srclen, &mbs, &dstlen) == (size_t)-1)
    perror("iconv");
  iconv_close(conv);
#endif  // _WIN32
}

void PrintFileName(const string& name) {
  for (int i = 1; i < depth; i++)
    cout << "-> ";
  cout << name << endl;
}
