#ifndef COMMAND_H
#define COMMAND_H

class Command {
public:
	Command() { }
	virtual ~Command() { }

	virtual void Execute() = 0;
private:
	Command(const Command&);
	Command& operator=(const Command&);
};


class QuitCommand : public Command {
public:
	void Execute();
};

#endif
