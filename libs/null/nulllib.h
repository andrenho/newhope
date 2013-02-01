#ifndef LIBS_NULL_NULLIB_H
#define LIBS_NULL_NULLLIB_H
 
#include "libs/graphiclibrary.h"
#include "libs/image.h"

class NullTimer : public Timer {
public:
	NullTimer() : Timer(0) { }

	inline bool ReachedCountDown() const {
		return false;
	}

	inline void WaitCountDown() {
	}

private:
	DISALLOW_COPY_AND_ASSIGN(NullTimer);
};


class NullImage : public Image {
public:
	NullImage() : Image(0, 0, false) { }

	inline void SetPixel(int x, int y, Color c) { }
	inline void Blit(const Rect& rs, const Image& image,
			const Rect& rd) const { }
	inline void Update() { }
	inline void FillBox(Color c) { }
	inline void FillBox(Rect r, Color c) { }
	inline void DrawLine(Point<int> p1, Point<int> p2, Color c, int w=1) {}
	inline void RemoveAlphaChannel() {}

private:
	DISALLOW_COPY_AND_ASSIGN(NullImage);
};


class NullFont : public Font {
public:
	NullFont() : Font() { }
private:
	DISALLOW_COPY_AND_ASSIGN(NullFont);
};


class NullLibrary : public GraphicLibrary {
public:
	NullLibrary() : GraphicLibrary() { Window = new NullImage(); }

	~NullLibrary() { delete Window; }

	inline Timer* CreateTimer(int wait_ms) const {
		return new NullTimer();
	}

	inline Image* LoadImage(const std::string& filename, const Rect& r) const {
		return new NullImage();
	}

	inline Image* CreateImage(int w, int h, int has_alpha) const {
		return new NullImage();
	}

	inline Font* LoadFont(const std::string& filename, int size) const {
		return new NullFont();
	}

	inline const Event* GetEvent() const {
		return new Event(Event::TypeEvent::NO_EVENT);
	}

	inline void GetKeyState(KeyState& state) const { }

private:
	DISALLOW_COPY_AND_ASSIGN(NullLibrary);
};

#endif
