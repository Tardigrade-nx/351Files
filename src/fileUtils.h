#ifndef _FILEUTILS_H_
#define _FILEUTILS_H_

#include <string>

namespace FileUtils
{

   // Format file size to human readable (K, M, G)
   std::string formatSize(const unsigned long int &p_size);

}

#endif
