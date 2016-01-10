// TODO: Handle error if thread isn't created properly
// TODO: Sleeping for a whole second when inactive sounds like overkill
// TODO: See how full the logger queue ever gets--that'd be an interesting stat!

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "logger.h"

void LoggerInactiveCallback()
{
  const int Millisecond = 1000;
  int logger_lines_lost = GetLoggerLinesLost();
  if(logger_lines_lost > 0)
  {
    printf("LOG STATS: %d log lines lost; %d inactive cycles\n", GetLoggerLinesLost(), GetLoggerInactiveCycles());
  }

  usleep(50 * Millisecond);
}

void RunLogger()
{
  LoggerProcedure();
  pthread_exit(NULL);
}

void BuildAndStartLogger()
{
  pthread_t log_thread;
  const pthread_attr_t* LogThreadAttributes = NULL;
  void* log_routine_pointer = RunLogger;
  int threadStatus = pthread_create(&log_thread, LogThreadAttributes, log_routine_pointer, NULL);
}
