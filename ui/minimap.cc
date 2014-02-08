#include "ui/minimap.h"

#include <iostream>

#include "./globals.h"
#include "engine/hero.h"
#include "engine/world.h"
#include "ui/ui.h"

Minimap::Minimap(int w, int h)
    : W(w), H(h), init_thread(nullptr), minimap_built(false), thread_killed(false)
{
}


Minimap::~Minimap()
{
    if(init_thread) {
        delete init_thread;
    }
}


void
Minimap::Initialize()
{
    init_thread = new std::thread(&Minimap::InitializationThread, this);
}


void
Minimap::StopThreadExecution()
{
    thread_killed = true;
    if(!minimap_built) {
        init_thread->join();
    }
}


void
Minimap::InitializationThread()
{
    int limit_x1, limit_y1, limit_x2, limit_y2;
    world->Limits(limit_x1, limit_y1, limit_x2, limit_y2);

    int prop_w = limit_x1 / (limit_x2 - limit_x1),
        prop_h = limit_y1 / (limit_y2 - limit_y1);

    CreateImage(W, H);

    // draw tiles
    for(int x=0; x<W; x++) for(int y=0; y<H; y++) {
        if(thread_killed) { return; }
        Block const* b[10];
        world->Tiles(b, (-limit_x1+limit_x2) / W * (x-W/2),
                (-limit_y1+limit_y2) / H * (y-H/2));
        DrawPoint(x, y, b[0]->R, b[0]->G, b[0]->B);
    }

    // draw rivers
    for(auto const& river : world->Rivers()) {
        if(thread_killed) { return; }
        std::vector<Point> pts;
        for(auto const& p : river) {
            pts.push_back(Point(
                (p.X() / (limit_x2 - limit_x1) - prop_w) * W + W/2,
                (p.Y() / (limit_y2 - limit_y1) - prop_h) * H + H/2));
        }
        DrawPoints(pts, Block::WATER->R, Block::WATER->G, Block::WATER->B);
    }
    
    // draw cities
    for(auto const& city : world->Cities()) {
        if(thread_killed) { return; }
        int px = static_cast<int>((static_cast<double>(city->X) / (limit_x2 - limit_x1) - prop_w) * W + W/2);
        int py = static_cast<int>((static_cast<double>(city->Y) / (limit_y2 - limit_y1) - prop_h) * H + H/2);
        DrawRectangle(px-2, py-2, 5, 5, true, 148, 69, 53);
        DrawRectangle(px-4, py-4, 9, 9, false, 148, 69, 53);
    }

    FinishImage();
    minimap_built = true;
}


void 
Minimap::Draw(int x, int y) const
{
    if(!minimap_built) {
        DrawWaitingScreen();
        init_thread->join();
    }

    // present image
    PresentImage(x, y);

    int limit_x1, limit_y1, limit_x2, limit_y2;
    world->Limits(limit_x1, limit_y1, limit_x2, limit_y2);

    int prop_w = limit_x1 / (limit_x2 - limit_x1),
        prop_h = limit_y1 / (limit_y2 - limit_y1);

    // draw player
    Point const& pos = world->Hero().Position();
    int px = static_cast<int>((pos.X() / (limit_x2 - limit_x1) - prop_w) * W + x + W/2);
    int py = static_cast<int>((pos.Y() / (limit_y2 - limit_y1) - prop_h) * H + y + H/2);
    DrawRectangleScreen(px-2, py-2, 5, 5, true, 255, 255, 0);
    DrawRectangleScreen(px-3, py-3, 7, 7, false, 0, 0, 0);

    UpdateScreen();
}


// vim: ts=4:sw=4:sts=4:expandtab
