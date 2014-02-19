#include "ui/w/wireframeui.h"

#include <chipmunk.h>
#include <SDL2/SDL.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
using namespace std;

#include "./main.h"
#include "engine/command.h"
#include "engine/hero.h"
#include "engine/point.h"
#include "engine/rectangle.h"
#include "engine/vehicle.h"
#include "engine/world.h"
#include "ui/w/wminimap.h"
#include "ui/w/wdialogmanager.h"

WireframeUI::WireframeUI()
    : active(true), win(nullptr), ren(nullptr), rx(0), ry(0)
{
    // initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "\nUnable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // initialize window
    if(!(win = SDL_CreateWindow("NewHope", SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE))) {
        fprintf(stderr, "\nUnable to intialize window: %s\n", SDL_GetError());
        exit(1);
    }

    // initialize renderer
    if(!(ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE))) {
        fprintf(stderr, "\nUnable to intialize renderer: %s\n", SDL_GetError());
        exit(1);
    }

    // create minimap
    minimap = unique_ptr<Minimap>(new WMinimap(600, 600, *ren));

    // create dialog
    dialog = unique_ptr<DialogManager>(new WDialogManager(win, ren));
}


WireframeUI::~WireframeUI()
{
    minimap->Finalize();

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}


void 
WireframeUI::Initialize()
{
    minimap->Initialize();
}


void
WireframeUI::Quit()
{
    active = false;
}


uint32_t 
WireframeUI::Now() const
{
    return SDL_GetTicks();
}


void 
WireframeUI::Wait(uint32_t tm) const
{
    SDL_Delay(tm);
}


vector<unique_ptr<Command>>
WireframeUI::GetEvents() const
{
    vector<unique_ptr<Command>> cmds;
    
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
        case SDL_QUIT:
            cmds.push_back(unique_ptr<Command>(new QuitCommand()));
            break;
        case SDL_KEYDOWN:
            switch(e.key.keysym.sym) {
            case SDLK_m:
                cmds.push_back(unique_ptr<Command>(new ShowMinimapCommand()));
                break;
            case SDLK_e:
                cmds.push_back(unique_ptr<Command>(new ExitVehicleCommand()));
                break;
            default:
                break;
            }
        default:
            break;
        }
    }

    const Uint8* k = SDL_GetKeyboardState(NULL);
    cmds.push_back(unique_ptr<Command>(new MoveCommand(
            k[SDL_SCANCODE_UP], k[SDL_SCANCODE_DOWN],
            k[SDL_SCANCODE_LEFT], k[SDL_SCANCODE_RIGHT])));

    return cmds;
}


Rectangle& 
WireframeUI::GetVisibleArea(Rectangle& r) const
{
    int win_w, win_h;
    SDL_GetWindowSize(win, &win_w, &win_h);

    Point p1((-rx) / Z - 5, (-ry) / Z - 5);
    Point p2((-rx + (win_w)) / Z + 5, (-ry + (win_h)) / Z + 5);
    r.setP1(p1);
    r.setP2(p2);
    return r;
}


void
WireframeUI::RedrawScene() const
{
    Point hero_pos = world->Hero().Position();
    CenterScreen(hero_pos);

    Rectangle visible_area;
    GetVisibleArea(visible_area);
    RenderScene(visible_area);
}


void
WireframeUI::PresentScene() const
{
    SDL_RenderPresent(ren);
}


void 
WireframeUI::ShowMinimap() const
{
    int win_w, win_h;
    SDL_GetWindowSize(win, &win_w, &win_h);

    // border
    SDL_SetRenderDrawColor(ren, 0x96, 0x4b, 0x00, SDL_ALPHA_OPAQUE);
    SDL_Rect r = { win_w/2 - minimap->W/2 - 5, win_h/2 - minimap->H/2 - 5, 
        minimap->W + 10, minimap->H + 10 };
    SDL_RenderFillRect(ren, &r);

    // minimap
    minimap->Draw(win_w/2 - minimap->W/2, win_h/2 - minimap->H/2);

    // wait for keypress
    SDL_Event e;
    for(;;) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_KEYDOWN) {
                return;
            }
        }
        SDL_Delay(100);
    }
}


/***************************************************************************/


void 
WireframeUI::CenterScreen(Point const& p) const
{
    int win_w, win_h;
    SDL_GetWindowSize(win, &win_w, &win_h);

    rx = -p.X() * Z + (win_w/2);
    ry = -p.Y() * Z + (win_h/2);
}


void
WireframeUI::RenderScene(Rectangle const& rect) const
{
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);

    // draw tiles
    for(int x=static_cast<int>(rect.P1().X()); x<static_cast<int>(rect.P2().X()); x++) {
        for(int y=static_cast<int>(rect.P1().Y()); y<static_cast<int>(rect.P2().Y()); y++) {
            DrawTile(x, y);
        }
    }

    // draw static objects
    cpBodyEachShape(space->staticBody, DrawStaticShape, const_cast<WireframeUI*>(this));

    // draw objects
    for(auto const& object: world->Objects()) {
        Point pos = object->Position();
        if(rect.ContainsPoint(pos)) {
            DrawObject(*object);
        }
    }
}


void 
WireframeUI::DrawTile(int x, int y) const
{
    static Block b[10];
    int n = world->Tiles(b, x, y);
    if(n > 0) {
        BlockType const& bp = world->Blocks.Examine(b[0]);
        SDL_SetRenderDrawColor(ren,
            static_cast<Uint8>(min(bp.R + 50, 255)),
            static_cast<Uint8>(min(bp.G + 50, 255)),
            static_cast<Uint8>(min(bp.B + 50, 255)),
            SDL_ALPHA_OPAQUE);

        SDL_Rect rect = { 
            static_cast<int>(x*Z+rx), 
            static_cast<int>(y*Z+ry), 
            static_cast<int>(Z), 
            static_cast<int>(Z) };
        SDL_RenderFillRect(ren, &rect);
    }
}


void 
WireframeUI::DrawObject(Object const& object) const
{
    const Person* person = nullptr;
    const Vehicle* vehicle = nullptr;

    if((person = dynamic_cast<Person const*>(&object)) != nullptr) {
        Point pos = object.Position();
        SDL_SetRenderDrawColor(ren, 0, 128, 0, 255);
        RenderCircle(pos.X()*Z+rx, pos.Y()*Z+ry, person->Radius()*Z);
    } else if((vehicle = dynamic_cast<Vehicle const*>(&object)) != nullptr) {
        cpBody *vb, *rwb, *fwb;
        cpShape *vs, *rws, *fws;
        vehicle->PhysicsBodies(vb, rwb, fwb);
        vehicle->PhysicsShapes(vs, rws, fws);
        DrawShape(vb, vs);
        DrawShape(rwb, rws);
        DrawShape(fwb, fws);
    }
}


void 
WireframeUI::DrawShape(cpBody* body, cpShape* shape) const
{
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

    // get body info
    cpVect v = cpBodyGetPos(body);
    cpFloat angle = cpBodyGetAngle(body);
    cpVect rot = cpvforangle(angle);

    // get vectors
    int n = cpPolyShapeGetNumVerts(shape);
    SDL_Point* pts = new SDL_Point[n+1];

    // rotate vectors
    int i;
    for(i=0; i<n; i++) {
        cpVect p = cpPolyShapeGetVert(shape, i);
        cpVect vr = cpvrotate(cpv(p.x,p.y), rot);
        pts[i] = {
            static_cast<int>((vr.x+v.x)*Z+rx), 
            static_cast<int>((vr.y+v.y)*Z+ry)
        };
        if(i == 0)
            pts[n] = pts[i];
    }

    // draw
    SDL_RenderDrawLines(ren, pts, n+1);

    delete[] pts;
}


void 
WireframeUI::DrawStaticShape(cpBody *body, cpShape *shape, void* data)
{
    (void) body;

    WireframeUI* self = static_cast<WireframeUI*>(data);
    Rectangle r;
    self->GetVisibleArea(r);

    cpBB bb = cpShapeGetBB(shape);
    if(bb.l < r.P1().X() || bb.l > r.P2().X() || 
            bb.t < r.P1().Y() || bb.t > r.P2().Y()) {
        // out of bounds
        return;
    }

    SDL_SetRenderDrawColor(self->ren, 150, 0, 0, 255);
    SDL_Rect rect = { 
        static_cast<int>(bb.l * self->Z + self->rx), 
        static_cast<int>(bb.b * self->Z + self->ry), 
        static_cast<int>((bb.r - bb.l) * self->Z),
        static_cast<int>((bb.t - bb.b) * self->Z) 
    };
    SDL_RenderDrawRect(self->ren, &rect);
}


void
WireframeUI::RenderCircle(double x1, double y1, double r) const
{
    double x=r, y=0;
    int radiusError = 1-static_cast<int>(x);
    int i = 0;
    static SDL_Point circle_pixel[1000];

    while(x >= y) {
        circle_pixel[i++] = SDL_Point{ static_cast<int>( x + x1), static_cast<int>( y + y1) };
        circle_pixel[i++] = SDL_Point{ static_cast<int>( y + x1), static_cast<int>( x + y1) };
        circle_pixel[i++] = SDL_Point{ static_cast<int>(-x + x1), static_cast<int>( y + y1) };
        circle_pixel[i++] = SDL_Point{ static_cast<int>(-y + x1), static_cast<int>( x + y1) };
        circle_pixel[i++] = SDL_Point{ static_cast<int>(-x + x1), static_cast<int>(-y + y1) };
        circle_pixel[i++] = SDL_Point{ static_cast<int>(-y + x1), static_cast<int>(-x + y1) };
        circle_pixel[i++] = SDL_Point{ static_cast<int>( x + x1), static_cast<int>(-y + y1) };
        circle_pixel[i++] = SDL_Point{ static_cast<int>( y + x1), static_cast<int>(-x + y1) };
        y++;
        if(radiusError<0) {
            radiusError += static_cast<int>(2*y+1);
        } else {
            x--;
            radiusError += static_cast<int>(2*(y-x+1));
        }
    }

    SDL_RenderDrawPoints(ren, circle_pixel, i);
}


void
WireframeUI::WaitForKeypress() const
{
	// wait for keypress
	for(;;) {
		SDL_Event e;
		SDL_PollEvent(&e);
		const Uint8* k = SDL_GetKeyboardState(NULL);
		if(k[SDL_SCANCODE_SPACE])
			break;
		SDL_Delay(1000/60);
	}
	for(;;) {
		SDL_Event e;
		SDL_PollEvent(&e);
		const Uint8* k = SDL_GetKeyboardState(NULL);
		if(!k[SDL_SCANCODE_SPACE])
			break;
		SDL_Delay(1000/60);
	}
}


// vim: ts=4:sw=4:sts=4:expandtab
