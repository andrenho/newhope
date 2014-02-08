#ifndef UI_MINIMAP_H_
#define UI_MINIMAP_H_

#include <cstdint>
#include <cstdio>
#include <vector>
#include <thread>

#include "engine/point.h"

class Minimap {
public:
    Minimap(int w, int h);
    virtual ~Minimap();

    void Initialize();
    void StopThreadExecution();
    void Draw(int x, int y) const;
    virtual void DestroyImage() = 0;

    const int W, H;

protected:
    virtual void CreateImage(int w, int h) = 0;
    virtual void DrawPoint(int x, int y, uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void DrawPoints(std::vector<Point> const& pts, 
            uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void DrawRectangle(int x1, int y1, int x2, int y2, 
            bool fill, uint8_t r, uint8_t g, uint8_t b) const = 0;
    virtual void DrawRectangleScreen(int x1, int y1, int x2, int y2, 
            bool fill, uint8_t r, uint8_t g, uint8_t b) const = 0;
    virtual void DrawWaitingScreen() const = 0;
    virtual void FinishImage() = 0;
    virtual void UpdateScreen() const = 0;
    
    virtual void PresentImage(int x, int y) const = 0;

private:
    Minimap(const Minimap&);
    Minimap& operator=(const Minimap&);

    std::thread* init_thread;
    bool minimap_built, thread_killed;

    void InitializationThread();
};

#endif  // UI_MINIMAP_H_


// vim: ts=4:sw=4:sts=4:expandtab
