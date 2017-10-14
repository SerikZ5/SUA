#include "CommandAnswer.h"

CommandAnswer::CommandAnswer(QString header, int executedCommand)
{
  this->header = header;
  this->executedCommand = executedCommand;
}

CommandAnswer::~CommandAnswer()
{
}