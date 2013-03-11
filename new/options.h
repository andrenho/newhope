#ifndef OPTIONS_H
#define OPTIONS_H

typedef enum GameType { OPT_GAME, OPT_EDITOR } GameType;

class Options {
public:
	Options(int argc, char** argv);

	GameType Type;
};

#endif
