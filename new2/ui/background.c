#include "ui/background.h"

#include <stdlib.h>

#include "ui/ui.h"
#include "engine/world.h"

static void bg_draw_tile(Background* b, int x, int y);
static void bg_draw_person(Background* b, Person* p);


Background* bg_init(UI* ui)
{
	Background* b = calloc(sizeof(Background), 1);
	b->tx = NULL;
	b->ui = ui;
	b->win = ui->win;
	b->ren = ui->ren;
	bg_recreate(b);
	return b;
}


void bg_free(Background** b)
{
	if((*b)->tx)
		SDL_DestroyTexture((*b)->tx);
	free(*b);
	*b = NULL;
}


void bg_recreate(Background* b)
{
	int _w, _h;
	SDL_GetWindowSize(b->win, &_w, &_h);

	if(b->tx)
		SDL_DestroyTexture(b->tx);
	b->tx = SDL_CreateTexture(b->ren, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, _w, _h);
}


void bg_render(Background* b)
{
	int _w, _h;
	SDL_GetWindowSize(b->win, &_w, &_h);
	SDL_RenderCopy(b->ren, b->tx, NULL, NULL);
}


void bg_redraw(Background* b)
{
	SDL_SetRenderTarget(b->ren, b->tx);
	SDL_SetRenderDrawColor(b->ren, 255, 255, 255, 255);
	SDL_RenderClear(b->ren);

	int x1, y1, x2, y2;
	ui_screen_limits(b->ui, &x1, &y1, &x2, &y2);

	// draw tiles
	for(int x=x1-1; x<=x2+2; x++) {
		for(int y=y1-1; y<=y2+2; y++) {
			bg_draw_tile(b, x, y);
		}
	}

	// draw people
	FOREACH(b->ui->w->people, Person*, p) {
		if(p->x >= (x1-1) && p->x <= x2 && p->y >= (y1-1) && p->y <= y2) {
			bg_draw_person(b, p);
		}
	}

	SDL_SetRenderTarget(b->ren, NULL);
}


/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/


static void bg_draw_tile(Background* b, int x, int y)
{
	Object obj;
	Terrain t = world_xy(b->ui->w, x, y, &obj);

	// draw terrain
	SDL_Rect rs = resources_terrain_rect(b->ui->res, t);
	SDL_Rect rd = { .x = (x*TILE_W) - b->ui->rx, .y = (y*TILE_H) - b->ui->ry,
	                .w = TILE_W, .h = TILE_H };
	SDL_RenderCopy(b->ren, b->ui->res->sprites, &rs, &rd);

	// draw object
	SDL_Rect rso = resources_obj_rect(b->ui->res, obj);
	SDL_RenderCopy(b->ren, b->ui->res->sprites, &rso, &rd);
}


static void bg_draw_person(Background* b, Person* p)
{
	// body
	SDL_Rect rp, ra;
	resources_person_rect(b->ui->res, p, &rp, &ra);
	SDL_Rect rd = { .x = (p->x * TILE_W) - b->ui->rx - TILE_W/2, 
		        .y = (p->y * TILE_H) - b->ui->ry - TILE_H/2,
			.w = TILE_W, .h = TILE_H };
	SDL_RenderCopy(b->ren, b->ui->res->sprites, &rp, &rd);
	SDL_RenderCopyEx(b->ren, b->ui->res->sprites, &ra, &rd, p->direction, 
			NULL, SDL_FLIP_NONE);
}
