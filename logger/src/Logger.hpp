// The Heap project
// Logger
// Free to use and distribute
/// Main Header

#pragma once

#include <chrono>
#include <thread>
#include <cstdio>
#include <vector>

#ifndef MAX_MESSAGES
#define MAX_MESSAGES (256)
#endif

class Logger;

typedef enum eLogLevel LogLevel;
typedef struct sMessage Message;
typedef struct sLoggerFile LoggerFile;
typedef const char* cstr_t;
typedef std::chrono::system_clock defaultclock_t;
typedef std::chrono::time_point<defaultclock_t> timepoint_t;
typedef std::vector<LoggerFile> filesVec_t;
typedef void (*fn_logOutputer_t)(Logger*);

enum eLogLevel
{
   Debug = 0,
   Info = 10,
   Warning = 20,
   Error = 35,
   Fatal = 50
};
struct sMessage
{
   int level;
   cstr_t text;
   timepoint_t time;
};
struct sLoggerFile
{
   bool close;
   FILE* fd;
};

class Logger
{
public:
   static Logger* Get();
private:
   Logger(fn_logOutputer_t outputer);

private:
   int m_logLevel;
   bool m_locked;
   bool m_destroying;

   size_t m_messagesCount;
   Message m_messages[MAX_MESSAGES];

   filesVec_t m_outFiles;

   std::thread m_outputerTh;

public:
   void Log(int logLevel, cstr_t fmt, ...);

   template<typename... _Args> void Debug(cstr_t fmt, _Args... args)    { Log(LogLevel::Debug, fmt, args...); }
   template<typename... _Args> void Info(cstr_t fmt, _Args... args)     { Log(LogLevel::Info, fmt, args...); }
   template<typename... _Args> void Warning(cstr_t fmt, _Args... args)  { Log(LogLevel::Warning, fmt, args...); }
   template<typename... _Args> void Error(cstr_t fmt, _Args... args)    { Log(LogLevel::Error, fmt, args...); }
   template<typename... _Args> void Fatal(cstr_t fmt, _Args... args)    { Log(LogLevel::Fatal, fmt, args...); }
   
   int            GetLogLevel()        { return m_logLevel; }
   void           SetLogLevel(int v)   { m_logLevel = v; }
   size_t         GetMessagesCount()   { return m_messagesCount; }
   bool           IsDestroying()       { return m_destroying; }
   filesVec_t     GetOutFiles()        { return m_outFiles; }
   Message*       GetMessages();

   void           AddOutFileH(FILE* file, bool close = false);
   void           AddOutFile(cstr_t path, bool close = true);

   /* Make sure all messages are printed */
   void           Destroy();

private:
   void           _insert(Message& msg);

   void           _lock();
   void           _unlock();
};