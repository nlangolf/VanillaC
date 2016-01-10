#include <stdarg.h>
#include <stdio.h>
#include "logger.h"

const int LogLineMaxLength = 50;
const int MaxPendingLogLines = 100;

int logger_lines_lost = 0;
int logger_inactive_cycles = 0;

struct LogLine
{
  char string[LogLineMaxLength];
};

struct LogLine logLines[MaxPendingLogLines];
int currentWriteLogLineIndex = 0;
int currentReadLogLineIndex = 0;

char* GetLine(int index)
{
  return logLines[index].string;
}

int ReadLinesExist()
{
  if (currentReadLogLineIndex == currentWriteLogLineIndex)
  {
    return 0;
  }

  return 1;
}

char* GetNextWriteLine()
{
  int nextWriteLogLineIndex = currentWriteLogLineIndex + 1;
  if(nextWriteLogLineIndex >= MaxPendingLogLines)
  {
    nextWriteLogLineIndex = 0;
  }

  if(nextWriteLogLineIndex == currentReadLogLineIndex)
  {
    logger_lines_lost++;
  }

  char* writeLine = GetLine(currentWriteLogLineIndex);
  currentWriteLogLineIndex = nextWriteLogLineIndex;
  return writeLine;
}

char* GetNextReadLine()
{
  char* readLine = GetLine(currentReadLogLineIndex);
  currentReadLogLineIndex++;
  if(currentReadLogLineIndex >= MaxPendingLogLines)
  {
    currentReadLogLineIndex = 0;
  }

  return readLine;
}

void LogFormat(const char* format, ...)
{
  va_list args;
  char* logLine = GetNextWriteLine();
  va_start(args, format);
  vsnprintf(logLine, LogLineMaxLength, format, args);
  va_end(args);
}

void LoggerProcedure()
{
  while(1)
  {
    if(ReadLinesExist() == 0)
    {
      logger_inactive_cycles++;
      LoggerInactiveCallback();
    }

    char* logLine = GetNextReadLine();
    printf("%s\n", logLine);
  }
}

int GetLoggerInactiveCycles()
{
  return logger_inactive_cycles;
}

int GetLoggerLinesLost()
{
  return logger_lines_lost;
}
