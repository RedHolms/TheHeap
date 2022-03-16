// The Heap project
// Logger
// Free to use and distribute
/// Main Source

#include <iostream>
#include <cstdio>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdarg>

#include "Logger.hpp"

using namespace std;
using namespace std::chrono;

static Logger* _logger_instance = nullptr;
static bool _outputer_destroy = false;

constexpr const char*   _time_format   = "%m.%d.%Y %T";
constexpr const char*   _log_format    = "[%s.%06lli] (%s)\t%s\n";
constexpr milliseconds  _msgs_wait     = 10ms;
#define _curent_ms                     ((duration_cast<microseconds>(defaultclock_t::now().time_since_epoch()) % 1000000).count())

static void _outputer(Logger* _this);
static char* _format_log(Message& msg);
static void _log_write(const char* buff, filesVec_t& filesToWrite);

Logger::Logger(fn_logOutputer_t outputer)
{
   m_logLevel = LogLevel::Info;
   m_locked = false;
   m_messagesCount = 0;
   m_destroying = false;

   m_outputerTh = thread(outputer, this);
}

Logger* Logger::Get()
{  
   if (!_logger_instance)
      _logger_instance = new Logger(_outputer);
   return _logger_instance;
}

void Logger::Log(int logLevel, cstr_t fmt, ...)
{
   if (logLevel < m_logLevel || m_destroying) return;

   va_list args;
   va_start(args, fmt);

   va_list args2;
   va_copy(args2, args);
   size_t size = vsnprintf(nullptr, 0, fmt, args) + 1;
   va_end(args2);

   char* buff = new char[size];
   vsnprintf(buff, size, fmt, args);
   va_end(args);

   Message msg;
   msg.level = logLevel;
   msg.text = buff;
   msg.time = defaultclock_t::now();
   _insert(msg);
}

Message* Logger::GetMessages()
{
   _lock();
      Message* messages = new Message[m_messagesCount];
      memcpy(messages, m_messages, m_messagesCount * sizeof(Message));
      m_messagesCount = 0;
   _unlock();
   return messages;
}

void Logger::AddOutFileH(FILE* file, bool close)
{
   m_outFiles.push_back({close, file});
}
void Logger::AddOutFile(cstr_t path, bool close)
{
   fclose(fopen(path, "w")); /* First clear file content */
   AddOutFileH(fopen(path, "ab"));
}

void Logger::Destroy()
{
   m_destroying = true;
   for (filesVec_t::iterator file = m_outFiles.begin(); file != m_outFiles.end(); file++) {
      if ((*file).close) fclose((*file).fd);
   }
   m_outputerTh.join();
}

void Logger::_insert(Message& msg)
{
   while (m_messagesCount >= MAX_MESSAGES); /* If messages queue overflowed, wait for outputing */
   _lock();
      m_messages[m_messagesCount++] = msg;
   _unlock();
}

void Logger::_lock()
{
   while (m_locked); /* If locked, wait for unlock */
   m_locked = true;
}
void Logger::_unlock()
{
   if (m_locked)
      m_locked = false;
}

static void _outputer(Logger* _this)
{
   if (!_this) return;

   while (true)
   {
      while (_this->GetMessagesCount() == 0) 
      {
         if (_this->IsDestroying()) return;
         std::this_thread::sleep_for(_msgs_wait); /* Wait for messages */
      }

      size_t messagesCount = _this->GetMessagesCount();
      Message* messages = _this->GetMessages();

      for (size_t i = 0; i < messagesCount; i++)
      {
         Message& msg = messages[i];
         char* buff = _format_log(msg);
         _log_write(buff, _this->GetOutFiles());
      }

      delete[] messages;
   }
}
static char* _format_log(Message& msg)
{
   size_t tempSize;
   char* timeBuff;
   const char* logLevelBuff;

   time_t curent = defaultclock_t::to_time_t(msg.time);
   struct tm* time = localtime(&curent);
   timeBuff = new char[256];
   strftime(timeBuff, 256, _time_format, time);

   switch (msg.level)
   {
   case LogLevel::Debug: logLevelBuff = "DEBUG"; break;
   case LogLevel::Info: logLevelBuff = "INFO"; break;
   case LogLevel::Warning: logLevelBuff = "WARNING"; break;
   case LogLevel::Error: logLevelBuff = "ERROR"; break;
   case LogLevel::Fatal: logLevelBuff = "FATAL"; break;
   default:
      tempSize = snprintf(nullptr, 0, "%i", msg.level) + 1;
      char* _buff = new char[tempSize];
      snprintf(_buff, tempSize, "%i", msg.level);
      logLevelBuff = _buff;
      break;
   }
   tempSize = snprintf(nullptr, 0, _log_format, timeBuff, _curent_ms, logLevelBuff, msg.text) + 1;
   char* outBuff = new char[tempSize];
   snprintf(outBuff, tempSize, _log_format, timeBuff, _curent_ms, logLevelBuff, msg.text);
   return outBuff;
}
static void _log_write(const char* buff, filesVec_t& filesToWrite)
{
   for (filesVec_t::iterator file = filesToWrite.begin(); file != filesToWrite.end(); file++)
   {
      fwrite(buff, sizeof(char), strlen(buff), (*file).fd);
      fflush((*file).fd);
   }
}