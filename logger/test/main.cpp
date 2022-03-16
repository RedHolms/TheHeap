#include <cstdio>
#include <iostream>

#include "Logger.hpp"

using namespace std;

int main()
{
   Logger* log = Logger::Get();
   log->AddOutFileH(stdout, false);
   log->SetLogLevel(1 << 31);

   log->Debug("Debug message");
   log->Info("Info message");
   log->Warning("Warning message");
   log->Error("Error message");
   log->Fatal("Fatal message");

   log->Log(-1, "Message with custom log level");

   log->Destroy();
   return 0;
}