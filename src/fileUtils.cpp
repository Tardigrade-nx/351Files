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

// To lower case
static void AsciiToLower(std::string *s)
{
   for (char &c : *s)
      if (c >= 'A' && c <= 'Z')
         c -= ('Z' - 'z');
}

//------------------------------------------------------------------------------

// Lower case file extension
std::string FileUtils::getLowercaseFileExtension(const std::string &p_name) {
   const auto dot_pos = p_name.rfind('.');
   if (dot_pos == std::string::npos)
      return "";
   std::string ext = p_name.substr(dot_pos + 1);
   AsciiToLower(&ext);
   return ext;
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
   return p_path.substr(l_pos + 1);
}

//------------------------------------------------------------------------------

// Extract dir name from a path
std::string FileUtils::getDirName(const std::string &p_path)
{
   size_t l_pos = p_path.rfind('/');
   return p_path.substr(0, l_pos);
}

//------------------------------------------------------------------------------

// Copy files to dest dir
void FileUtils::copyFiles(const std::vector<std::string> &p_src, const std::string &p_dest)
{
   std::string l_destFile;
   std::string l_fileName;
   bool l_confirm(true);
   bool l_execute(true);
   for (auto l_it = p_src.begin(); l_it != p_src.end(); ++l_it)
   {
      l_execute = true;
      l_fileName = getFileName(*l_it);
      l_destFile = p_dest + (p_dest.at(p_dest.size() - 1) == '/' ? "" : "/") + l_fileName;

      // Check if destination files already exists
      if (l_confirm && fileExists(l_destFile))
      {
         INHIBIT(std::cout << "File " << l_destFile << " already exists => ask for confirmation" << std::endl;)
         Dialog l_dialog("Question:");
         l_dialog.addLabel("Overwrite " + l_fileName + "?");
         l_dialog.addOption("Yes", 0, g_iconSelect);
         l_dialog.addOption("Yes to all", 1, g_iconSelect);
         l_dialog.addOption("No", 2, g_iconNone);
         l_dialog.addOption("Cancel", 3, g_iconCancel);
         switch (l_dialog.execute())
         {
            // Yes
            case 0: break;
            // Yes to all
            case 1: l_confirm = false; break;
            // No
            case 2: l_execute = false; break;
            // Cancel
            default: return;
         }
      }
      if (l_execute)
      {
         INHIBIT(std::cout << "Copy " << *l_it << " to " << p_dest << "\n";)
         Run("cp", "-r", *l_it, p_dest);
         Run("sync", l_destFile);
      }
   }
}

//------------------------------------------------------------------------------

// Move files to dest dir
void FileUtils::moveFiles(const std::vector<std::string> &p_src, const std::string &p_dest)
{
   std::string l_destFile;
   std::string l_fileName;
   bool l_confirm(true);
   bool l_execute(true);
   for (auto l_it = p_src.begin(); l_it != p_src.end(); ++l_it)
   {
      l_execute = true;
      // Check if destination files already exists
      if (l_confirm)
      {
         l_fileName = getFileName(*l_it);
         l_destFile = p_dest + (p_dest.at(p_dest.size() - 1) == '/' ? "" : "/") + l_fileName;
         if (fileExists(l_destFile))
         {
            INHIBIT(std::cout << "File " << l_destFile << " already exists => ask for confirmation" << std::endl;)
            Dialog l_dialog("Question:");
            l_dialog.addLabel("Overwrite " + l_fileName + "?");
            l_dialog.addOption("Yes", 0, g_iconSelect);
            l_dialog.addOption("Yes to all", 1, g_iconSelect);
            l_dialog.addOption("No", 2, g_iconNone);
            l_dialog.addOption("Cancel", 3, g_iconCancel);
            switch (l_dialog.execute())
            {
               // Yes
               case 0: break;
               // Yes to all
               case 1: l_confirm = false; break;
               // No
               case 2: l_execute = false; break;
               // Cancel
               default: return;
            }
         }
      }
      if (l_execute)
      {
         INHIBIT(std::cout << "Move " << *l_it << " to " << p_dest << "\n";)
         Run("mv", *l_it, p_dest);
         Run("sync", p_dest);
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
   // Execute command 'du'
   std::string l_line = "du -s " + p_path;
   char l_buffer[256];
   FILE *l_pipe = popen(l_line.c_str(), "r");
   if (l_pipe == NULL)
   {
      std::cerr << "getDirSize: Error popen\n";
      return 0;
   }
   while (fgets(l_buffer, sizeof(l_buffer), l_pipe) != NULL);
   l_line = l_buffer;
   pclose(l_pipe);
   // Keep the number only
   size_t l_pos = l_line.find(" ");
   if (l_pos != std::string::npos)
      l_line = l_line.erase(l_pos);
   // Convert to int
   unsigned long long int l_result = 0;
   std::istringstream iss(l_line);
   iss >> l_result;
   // Convert to bytes
   if (std::getenv("POSIXLY_CORRECT") != NULL)
      l_result *= 512;
   else
      l_result *= 1024;
   return l_result;
}
