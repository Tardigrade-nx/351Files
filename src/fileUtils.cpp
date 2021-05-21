#include <sstream>
#include <iomanip>
#include <cmath>
#include "fileUtils.h"

// Units used for human readable file size
namespace FileUtils
{
   #define NB_FILE_SIZE_UNITS 4
   const std::string fileSizeUnits[] = { "", " K", " M", " G" };
}

// Format file size to human readable (K, M, G)
std::string FileUtils::formatSize(const unsigned long int &p_size)
{
   // Convert size
   double l_size = p_size;
   int l_i = 0;
   for (; l_size > 1024.0 && l_i < NB_FILE_SIZE_UNITS; ++l_i)
      l_size /= 1024.0;
   // Format size to string
   std::ostringstream oss;
   if (std::floor(l_size) == l_size)
      oss << static_cast<unsigned long int>(l_size) << fileSizeUnits[l_i];
   else
      oss << std::fixed << std::setprecision(1) << l_size << fileSizeUnits[l_i];
   return oss.str();
}
