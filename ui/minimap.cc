#include "ui/minimap.h"

#include <glog/logging.h>
#include <iostream>
using namespace std;

#include "./main.h"
#include "engine/hero.h"
#include "engine/world.h"
#include "ui/ui.h"

Minimap::Minimap(int w, int h)
    : W(w), H(h)
{
}


void
Minimap::Initialize()
{
    //InitializationThread();
    pthread_create(&thread, 0, Minimap::ThreadInvoker, this);
}


void
Minimap::Finalize()
{
    thread_killed = true;
    pthread_join(thread, nullptr);
    DestroyImage();
}


void* 
Minimap::ThreadInvoker(void* self)
{
    static_cast<Minimap*>(self)->InitializationThread();
    return 0;
}


void
Minimap::InitializationThread()
{
    LOG(INFO) << "Minimap creation thread initialized.\n";

    int limit_x1, limit_y1, limit_x2, limit_y2;
    world->Limits(limit_x1, limit_y1, limit_x2, limit_y2);

    int prop_w = limit_x1 / (limit_x2 - limit_x1),
        prop_h = limit_y1 / (limit_y2 - limit_y1);

    CreateImage(W, H);

    // draw tiles
    for(int x=0; x<W; x++) for(int y=0; y<H; y++) {
        if(thread_killed) { return; }
        Block b[10];
        world->Tiles(b, (-limit_x1+limit_x2) / W * (x-W/2),
                (-limit_y1+limit_y2) / H * (y-H/2));
        BlockType const& bp = world->Blocks.Examine(b[0]);
        DrawPoint(x, y, bp.R, bp.G, bp.B);
    }

    // draw rivers
    for(auto const& river : world->Rivers()) {
        if(thread_killed) { return; }
        vector<Point> pts;
        for(auto const& p : river) {
            pts.push_back(Point(
                (p.X() / (limit_x2 - limit_x1) - prop_w) * W + W/2,
                (p.Y() / (limit_y2 - limit_y1) - prop_h) * H + H/2));
        }
        BlockType const& bp = world->Blocks.Examine(Block::WATER);
        DrawPoints(pts, bp.R, bp.G, bp.B);
    }
    
    // draw roads
    for(auto const& road : world->Roads()) {
        if(thread_killed) { return; }
        vector<Point> pts;
        for(auto const& p : road) {
            pts.push_back(Point(
                (p.X() / (limit_x2 - limit_x1) - prop_w) * W + W/2,
                (p.Y() / (limit_y2 - limit_y1) - prop_h) * H + H/2));
        }
        BlockType const& bp = world->Blocks.Examine(Block::ASPHALT);
        DrawPoints(pts, bp.R, bp.G, bp.B);
    }

    // draw cities
    for(auto const& city : world->Cities()) {
        if(thread_killed) { return; }
        int px = static_cast<int>((static_cast<double>(city.X) / (limit_x2 - limit_x1) - prop_w) * W + W/2);
        int py = static_cast<int>((static_cast<double>(city.Y) / (limit_y2 - limit_y1) - prop_h) * H + H/2);
        DrawRectangle(px-2, py-2, 5, 5, true, 148, 69, 53);
        DrawRectangle(px-4, py-4, 9, 9, false, 148, 69, 53);
    }

    FinishImage();
    minimap_built = true;

    LOG(INFO) << "Minimap creation thread finished.\n";
}


void 
Minimap::Draw(int x, int y) const
{
    if(!minimap_built) {
        DrawWaitingScreen();
    }
    pthread_join(thread, nullptr);

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
