#ifndef UI_H
#define UI_H

#include <cstdint>
#include <vector>
using namespace std;

class Command;

class UI {
public:
	UI() { }
	virtual ~UI() { }

	virtual void Initialize() = 0;
	virtual bool Active() = 0;
	virtual void Quit() = 0;

	virtual uint32_t Now() = 0;
	virtual void Wait(uint32_t tm) = 0;

	virtual void GetEvents(vector<Command*>& commands) = 0;

	virtual void RedrawScene() = 0;

private:
	UI(const UI&);
	UI& operator=(const UI&);
};

#endif
