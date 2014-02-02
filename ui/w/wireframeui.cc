#include "ui/w/wireframeui.h"

#include <chipmunk.h>
#include <SDL2/SDL.h>
#include <cstdlib>

#include "./globals.h"
#include "engine/command.h"
#include "engine/hero.h"
#include "engine/point.h"
#include "engine/rectangle.h"
#include "engine/vehicle.h"
#include "engine/world.h"
#include "ui/w/wminimap.h"

WireframeUI::WireframeUI()
	: active(true), win(nullptr), ren(nullptr), main_font(nullptr),
	  rx(0), ry(0)
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

	// initialize TTF_Init
	if(TTF_Init() == -1) {
		fprintf(stderr, "\nError initializing SDL2_ttf: %s\n", TTF_GetError());
		exit(1);
	}

	// load font
	main_font = TTF_OpenFont(DATADIR "/PressStart2P.ttf", 16);
	if(!main_font) {
		fprintf(stderr, "\nUnable to load font: %s\n", TTF_GetError());
		exit(1);
	}
	
	// create minimap
	minimap = new WMinimap(300, 300, *ren);
}


WireframeUI::~WireframeUI()
{
	minimap->DestroyImage();
	delete minimap;

	TTF_CloseFont(main_font);
	TTF_Quit();
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


void 
WireframeUI::GetEvents(std::vector<Command*>& cmds) const
{
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			cmds.push_back(new QuitCommand());
			break;
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym) {
			case SDLK_m:
				cmds.push_back(new ShowMinimapCommand());
				break;
			}
		}
	}

	const Uint8* k = SDL_GetKeyboardState(NULL);
	cmds.push_back(new MoveCommand(
			k[SDL_SCANCODE_UP], k[SDL_SCANCODE_DOWN],
			k[SDL_SCANCODE_LEFT], k[SDL_SCANCODE_RIGHT]));
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
	for(int x=rect.P1().X(); x<rect.P2().X(); x++) {
		for(int y=rect.P1().Y(); y<rect.P2().Y(); y++) {
			DrawTile(x, y);
		}
	}

	// draw static objects
	cpBodyEachShape(world->SpacePhysics()->staticBody, DrawStaticShape, 
			const_cast<WireframeUI*>(this));

	// draw objects
	for(auto const& object: world->Objects()) {
		Point pos = object->Position();
		if(rect.IsInside(pos)) {
			DrawObject(*object);
		}
	}
	
	SDL_RenderPresent(ren);
}


void 
WireframeUI::DrawTile(int x, int y) const
{
	const Block* b[10];
	int n = world->Tiles(b, x, y);
	if(n > 0) {
		const Block* block = b[0];
		if(block == Block::GRASS) {
			SDL_SetRenderDrawColor(ren, 230, 255, 230, 255);
		} else if(block == Block::FLOOR) {
			SDL_SetRenderDrawColor(ren, 255, 230, 230, 255);
		} else {
			SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		}

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
	int radiusError = 1-x;
	int i = 0;
	static SDL_Point circle_pixel[1000];

	while(x >= y) {
		circle_pixel[i++] = SDL_Point{static_cast<int>( x + x1), static_cast<int>( y + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>( y + x1), static_cast<int>( x + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>(-x + x1), static_cast<int>( y + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>(-y + x1), static_cast<int>( x + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>(-x + x1), static_cast<int>(-y + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>(-y + x1), static_cast<int>(-x + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>( x + x1), static_cast<int>(-y + y1) };
		circle_pixel[i++] = SDL_Point{static_cast<int>( y + x1), static_cast<int>(-x + y1) };
		y++;
		if(radiusError<0) {
			radiusError += 2*y+1;
		} else {
			x--;
			radiusError += 2*(y-x+1);
		}
	}

	SDL_RenderDrawPoints(ren, circle_pixel, i);
}
