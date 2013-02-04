#ifndef UI_MINIMAP_H
#define UI_MINIMAP_H

#include <map>
#include <vector>
#ifdef USE_CPP_THREADS
#  include <thread>
#else
#  include <pthread.h>
#endif

#include "libs/colors.h"
#include "util/point.h"
#include "util/defines.h"
#include "world/world.h"

class GraphicLibrary;
class Image;
class Resources;

class Minimap {
public:
	Minimap(const GraphicLibrary& video, const World& world, 
			const Resources& res);
	~Minimap();

	void Reset();
	void Display();

private:
	void Create();
	void SetupColors();
	void KillThread();
	void DrawPaper();
	void DrawMap();
	void DrawRivers();
	void DrawCities();
	void HandleEvents();
	void DrawPath(const std::vector<Tile>& points, Color c);

	const GraphicLibrary& video;
	const World& world;
	const Resources& res;

#ifdef USE_CPP_THREADS
	static int CreationThread(void* self);
	std::thread* thr;
#else
	static void* CreationThread(void* self);
	pthread_t* thr;
#endif
	Image* image;
	int sz;
	bool thread_killed;
	std::map<TerrainType, Color> colors;

	DISALLOW_COPY_AND_ASSIGN(Minimap);
};

#endif
