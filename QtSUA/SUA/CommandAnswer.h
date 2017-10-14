#ifndef COMMAND_ANSWER
#define COMMAND_ANSWER

#include <QString>

class CommandAnswer
{
public:
  CommandAnswer(QString header, int executedCommand);
  virtual ~CommandAnswer();

  QString GetHeader() { return header; }
  void SetHeader(QString value) { header = value; }

  int GetExecutedCommand() { return executedCommand; }
  void SetExecutedCommand(int value) { executedCommand = value; }

private:
  QString header;
  int executedCommand;
};

#endif
