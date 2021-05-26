#ifndef _FILEUTILS_H_
#define _FILEUTILS_H_

#include <string>

namespace FileUtils
{

   // File utilities

   // Format file size to human readable (K, M, G)
   std::string formatSize(const unsigned long int &p_size);

   // Lower case file extension
   std::string getLowercaseFileExtension(const std::string &p_name);

}

#endif
