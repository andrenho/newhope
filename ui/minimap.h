#ifndef UI_MINIMAP_H_
#define UI_MINIMAP_H_

#include <pthread.h>

#include <cstdint>
#include <cstdio>
#include <vector>
using namespace std;

#include "engine/point.h"

class Minimap {
public:
    virtual ~Minimap() = default;

    Minimap(const Minimap&) = delete;
    Minimap& operator=(const Minimap&) = delete;

    void Initialize();
    void Finalize();
    void Draw(int x, int y) const;
    virtual void DestroyImage() = 0;

    const int W, H;

protected:
    Minimap(int w, int h);

    virtual void CreateImage(int w, int h) = 0;
    virtual void DrawPoint(int x, int y, uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void DrawPoints(vector<Point> const& pts, 
            uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void DrawRectangle_(int x1, int y1, int x2, int y2, 
            bool fill, uint8_t r, uint8_t g, uint8_t b) const = 0;
    virtual void DrawRectangle_Screen(int x1, int y1, int x2, int y2, 
            bool fill, uint8_t r, uint8_t g, uint8_t b) const = 0;
    virtual void DrawWaitingScreen() const = 0;
    virtual void FinishImage() = 0;
    virtual void UpdateScreen() const = 0;
    
    virtual void PresentImage(int x, int y) const = 0;

private:
    static void* ThreadInvoker(void* arg);
    void InitializationThread();
    
    bool minimap_built = false, thread_killed = false;
    pthread_t thread;
};

#endif  // UI_MINIMAP_H_


// vim: ts=4:sw=4:sts=4:expandtab
