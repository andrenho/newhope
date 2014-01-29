// Copyright 2014 <Imperial Software>

#ifndef UI_UI_H_
#define UI_UI_H_

#include <cstdint>
#include <vector>

class Command;

class UI {
public:
	virtual ~UI() { }

	virtual void Initialize() = 0;
	virtual bool Active() const = 0;
	virtual void Quit() = 0;

	virtual uint32_t Now() const = 0;
	virtual void Wait(uint32_t tm) const = 0;

	virtual void GetEvents(std::vector<Command*>& commands) const = 0;

	virtual void RedrawScene() const = 0;

protected:
	UI() { }

private:
	UI(const UI&);
	UI& operator=(const UI&);
};

#endif  // UI_UI_H_
