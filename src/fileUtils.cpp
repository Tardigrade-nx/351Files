#include <sstream>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#ifdef _POSIX_SPAWN
#include <spawn.h>
#else
#include <signal.h>
#endif
#include "fileUtils.h"
#include "def.h"
#include "dialog.h"

namespace FileUtils
{

   // Units used for human readable file size
   #define NB_FILE_SIZE_UNITS 4
   const std::string fileSizeUnits[] = { "", " K", " M", " G" };

   // Wait PID
   int WaitPid(pid_t id)
   {
      int status;
      while (waitpid(id, &status, WNOHANG) == 0)
         usleep(50 * 1000);
      if (1 != WIFEXITED(status) || 0 != WEXITSTATUS(status))
      {
         perror("Child process error");
         return -1;
      }
      return 0;
   }

   // If child_pid is NULL, waits for the child process to finish.
   // Otherwise, doesn't wait and stores child process pid into child_pid.
   int SpawnAndWait(const char *argv[])
   {
      pid_t child_pid;
      #ifdef _POSIX_SPAWN
      // This const cast is OK, see https://stackoverflow.com/a/190208.
      if (::posix_spawnp(&child_pid, argv[0], nullptr, nullptr, (char **)argv, nullptr) != 0)
      {
         perror(argv[0]);
         return -1;
      }
      return WaitPid(child_pid);
      #else
      struct sigaction sa, save_quit, save_int;
      sigset_t save_mask;
      int wait_val;
      memset(&sa, 0, sizeof(sa));
      sa.sa_handler = SIG_IGN;
      /* __sigemptyset(&sa.sa_mask); - done by memset() */
      /* sa.sa_flags = 0; - done by memset() */

      sigaction(SIGQUIT, &sa, &save_quit);
      sigaction(SIGINT, &sa, &save_int);
      sigaddset(&sa.sa_mask, SIGCHLD);
      sigprocmask(SIG_BLOCK, &sa.sa_mask, &save_mask);
      if ((child_pid = vfork()) < 0)
      {
         wait_val = -1;
         goto out;
      }
      if (child_pid == 0)
      {
         sigaction(SIGQUIT, &save_quit, NULL);
         sigaction(SIGINT, &save_int, NULL);
         sigprocmask(SIG_SETMASK, &save_mask, NULL);

         // This const cast is OK, see https://stackoverflow.com/a/190208.
         if (execvp(argv[0], (char **)argv) == -1)
            perror(argv[0]);
         exit(127);
      }
      wait_val = WaitPid(child_pid);

      out:
      sigaction(SIGQUIT, &save_quit, NULL);
      sigaction(SIGINT, &save_int, NULL);
      sigprocmask(SIG_SETMASK, &save_mask, NULL);
      return wait_val;
      #endif
   }

   const char *AsConstCStr(const char *s) { return s; }
   const char *AsConstCStr(const std::string &s) { return s.c_str(); }

   template <typename... Args>
   bool Run(Args... args)
   {
      const char *execve_args[] = {AsConstCStr(args)..., nullptr};
      return SpawnAndWait(execve_args) == 0;
   }

}

//------------------------------------------------------------------------------

// Format file size to human readable (K, M, G)
std::string FileUtils::formatSize(const unsigned long long int &p_size)
{
   // Convert size
   double l_size = p_size;
   int l_i = 0;
   for (; l_size > 1024.0 && l_i < NB_FILE_SIZE_UNITS; ++l_i)
      l_size /= 1024.0;
   // Format size to string
   std::ostringstream oss;
   if (std::floor(l_size) == l_size)
      oss << static_cast<unsigned int>(l_size) << fileSizeUnits[l_i];
   else
      oss << std::fixed << std::setprecision(1) << l_size << fileSizeUnits[l_i];
   return oss.str();
}

//------------------------------------------------------------------------------

// String to lower case
void FileUtils::toLower(std::string &s)
{
   for (char &c : s)
      if (c >= 'A' && c <= 'Z')
         c -= ('Z' - 'z');
}

//------------------------------------------------------------------------------

// File extension
std::string FileUtils::getFileExtension(const std::string &p_name) {
   const auto l_pos = p_name.rfind('.');
   if (l_pos == std::string::npos)
      return "";
   return p_name.substr(l_pos + 1);
}

//------------------------------------------------------------------------------

// File exists?
bool FileUtils::fileExists(const std::string &p_path)
{
   struct stat l_stat;
   return stat(p_path.c_str(), &l_stat) == 0;
}

//------------------------------------------------------------------------------

// Extract file name from a path
std::string FileUtils::getFileName(const std::string &p_path)
{
   size_t l_pos = p_path.rfind('/');
   if (l_pos == std::string::npos)
      return p_path;
   return p_path.substr(l_pos + 1);
}

//------------------------------------------------------------------------------

// Extract dir name from a path
std::string FileUtils::getDirName(const std::string &p_path)
{
   size_t l_pos = p_path.rfind('/');
   if (l_pos == std::string::npos)
      return p_path;
   return p_path.substr(0, l_pos);
}

//------------------------------------------------------------------------------

// Extract file name without extension
std::string FileUtils::getRootName(const std::string &p_path)
{
   const auto l_pos = p_path.rfind('.');
   if (l_pos == std::string::npos)
      return p_path;
   return p_path.substr(0, l_pos);
}

//------------------------------------------------------------------------------

// Copy files to dest dir
void FileUtils::copyOrMoveFiles(const char p_action, const std::vector<std::string> &p_src, const std::string &p_dest)
{
   std::string l_destFile;
   std::string l_fileName;
   bool l_yesToAll(false);
   bool l_noToAll(false);
   bool l_execute(true);
   for (auto l_it = p_src.begin(); l_it != p_src.end(); ++l_it)
   {
      l_execute = true;
      l_fileName = getFileName(*l_it);
      l_destFile = p_dest + (p_dest.at(p_dest.size() - 1) == '/' ? "" : "/") + l_fileName;

      // Case: source file and destination file are the same
      if (*l_it == l_destFile)
      {
         // Copy => duplicate 'file.ext' as 'file_copy.ext'
         if (p_action == 'c')
         {
            std::string ext = getFileExtension(l_destFile);
            l_destFile = getRootName(l_destFile) + "_copy" + (ext.empty() ? "" : ".")  + ext;
         }
         // Move => do nothing
         else if (p_action == 'm')
         {
            continue;
         }
      }

      // Check if destination file already exists
      if (fileExists(l_destFile))
      {
         INHIBIT(std::cout << "File " << l_destFile << " already exists\n";)
         // If 'no to all', go to next file
         if (l_noToAll)
            continue;
         if (!l_yesToAll)
         {
            // Ask for confirmation
            Dialog l_dialog("Question:");
            l_dialog.addLabel("Overwrite " + getFileName(l_destFile) + "?");
            l_dialog.addOption("Yes", 0, g_iconSelect);
            l_dialog.addOption("Yes to all", 1, g_iconSelect);
            l_dialog.addOption("No", 2, g_iconNone);
            l_dialog.addOption("No to all", 3, g_iconNone);
            l_dialog.addOption("Cancel", 4, g_iconCancel);
            switch (l_dialog.execute())
            {
               // Yes
               case 0: break;
               // Yes to all
               case 1: l_yesToAll = true; break;
               // No
               case 2: l_execute = false; break;
               // No to all
               case 3: l_execute = false; l_noToAll = true; break;
               // Cancel
               default: return;
            }
         }
      }

      // Execute command
      if (l_execute)
      {
         switch(p_action)
         {
            case 'c':
               INHIBIT(std::cout << "Copy " << *l_it << " to " << l_destFile << "\n";)
               Run("cp", "-r", *l_it, l_destFile);
               break;
            case 'm':
               INHIBIT(std::cout << "Move " << *l_it << " to " << l_destFile << "\n";)
               Run("mv", *l_it, l_destFile);
               break;
            default:
               return;
         }
         Run("sync", l_destFile);
      }
   }
}

//------------------------------------------------------------------------------

// Remove files
void FileUtils::removeFiles(const std::vector<std::string> &p_files)
{
   // Confirmation
   std::ostringstream oss;
   oss << "Delete " << p_files.size() << " file(s)?";
   Dialog l_dialog("Question:");
   l_dialog.addLabel(oss.str());
   l_dialog.addOption("Yes", 0, g_iconSelect);
   l_dialog.addOption("No", 1, g_iconCancel);
   if (l_dialog.execute() != 0)
      return;
   // Delete files
   for (auto l_it = p_files.begin(); l_it != p_files.end(); ++l_it)
   {
      INHIBIT(std::cout << "Remove " << *l_it << "\n";)
      Run("rm", "-rf", *l_it);
   }
}

//------------------------------------------------------------------------------

// Create new directory
void FileUtils::makeDirectory(const std::string &p_file)
{
   INHIBIT(std::cout << "Create directory " << p_file << "\n";)
   Run("mkdir", "-p", p_file);
   Run("sync", p_file);
}

//------------------------------------------------------------------------------

// Rename a file
void FileUtils::renameFile(const std::string &p_file1, const std::string &p_file2)
{
   // Check if destination file already exists
   if (fileExists(p_file2))
   {
      INHIBIT(std::cout << "File " << p_file2 << " already exists => ask for confirmation\n";)
      Dialog l_dialog("Question:");
      l_dialog.addLabel("Overwrite " + getFileName(p_file2) + "?");
      l_dialog.addOption("Yes", 0, g_iconSelect);
      l_dialog.addOption("No", 1, g_iconCancel);
      if (l_dialog.execute() != 0)
         return;
   }
   // Rename file
   INHIBIT(std::cout << "Rename file " << p_file1 << " to " << p_file2 << '\n';)
   Run("mv", p_file1, p_file2);
   Run("sync", p_file2);
}

//------------------------------------------------------------------------------

// Dir size
unsigned long long int FileUtils::getDirSize(const std::string &p_path)
{
   // Execute command 'du -bs'
   std::string line = "du -bs \"" + p_path + "\" 2>/dev/null";
   bool sizeInK = false;
   char buffer[256];
   FILE *pipe = popen(line.c_str(), "r");
   if (pipe == NULL)
   {
      std::cerr << "getDirSize: Error popen\n";
      return 0;
   }
   while (fgets(buffer, sizeof(buffer), pipe) != NULL);
   if (WEXITSTATUS(pclose(pipe)) != 0)
   {
      // 'du -bs' failed, try again with 'du -ks'
      line = "du -ks \"" + p_path  + "\" 2>/dev/null";
      pipe = popen(line.c_str(), "r");
      if (pipe == NULL)
      {
         std::cerr << "getDirSize: Error popen\n";
         return 0;
      }
      while (fgets(buffer, sizeof(buffer), pipe) != NULL);
      if (WEXITSTATUS(pclose(pipe)) != 0)
         return 0;
      sizeInK = true;
   }
   // Keep the number only
   line = buffer;
   size_t pos = line.find(" ");
   if (pos != std::string::npos)
      line = line.erase(pos);
   // Convert to int
   unsigned long long int result = 0;
   std::istringstream iss(line);
   iss >> result;
   if (sizeInK)
      result *= 1024;
   return result;
}

//------------------------------------------------------------------------------

// File is text
bool FileUtils::fileIsText(const std::string &p_path)
{
   std::string line = "file -bi \"" + p_path + "\"";
   char buffer[256];
   FILE *pipe = popen(line.c_str(), "r");
   if (pipe == NULL)
   {
      std::cerr << "getDirSize: Error popen\n";
      return 0;
   }
   while (fgets(buffer, sizeof(buffer), pipe) != NULL);
   pclose(pipe);
   line = buffer;
   return (line.substr(0, 4) == "text" && line.find("charset=us-ascii") != std::string::npos);
}
