// Copyright 2014 <Imperial Software>

#ifndef ENGINE_COMMAND_H_
#define ENGINE_COMMAND_H_

class Command {
public:
	Command() { }
	virtual ~Command() { }

	virtual void Execute() const = 0;
private:
	Command(const Command&);
	Command& operator=(const Command&);
};


class QuitCommand : public Command {
public:
	void Execute() const;
};

#endif  // ENGINE_COMMAND_H_
