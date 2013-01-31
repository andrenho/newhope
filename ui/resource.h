#ifndef UI_RESOURCE_H
#define UI_RESOURCE_H

#include <map>
#include <stdexcept>
#include <string>

#include "libs/image.h"
#include "util/logger.h"
#include "util/defines.h"

class GraphicLibrary;
class Rect;

//typedef std::map<const std::string, const Resource*> mapR;

class Resources {
public:
	explicit Resources(const GraphicLibrary& video);
	~Resources();

	const inline Image* operator[](const std::string& s) const {
		try {
			return dynamic_cast<const Image*>(res.at(s));
		} catch(std::out_of_range& e) {
			logger.Error(1, "Image resource %s does not exists.", 
					s.c_str());
			abort();
		}
	}

	const inline Point<int> Desloc(const Image* name) const {
		try {
			return desloc.at(name);
		} catch (std::out_of_range& e) {
			return Point<int>(0, 0);
		}
	}

private:
	const std::string FindFile(const std::string& filename);
	void LoadFile(const std::string& name, const std::string& path, 
			const Rect& r, const std::string& suffix);
	void LoadFile(const std::string& name, const std::string& path);

	const GraphicLibrary& video;
	std::map<const std::string, const Resource*> res;
	std::map<const Image*, Point<int>> desloc;

	DISALLOW_COPY_AND_ASSIGN(Resources);
};

#endif
