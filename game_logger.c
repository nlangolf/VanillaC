#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "logger.h"

void LoggerInactiveCallback()
{
  printf("LOG STATS: %d log lines lost; %d inactive cycles\n", GetLoggerLinesLost(), GetLoggerInactiveCycles());
  sleep(1);
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
