#include "ui/background.h"

#include <stdlib.h>

#include "ui/ui.h"
#include "engine/world.h"

static void bg_redraw(Background* b);
static void bg_draw_tile(Background* b, int x, int y);


Background* bg_init(UI* ui)
{
	Background* b = calloc(sizeof(Background), 1);
	b->tx = NULL;
	b->ui = ui;
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
	SDL_GetWindowSize(b->ui->win, &_w, &_h);

	if(b->tx)
		SDL_DestroyTexture(b->tx);
	b->tx = SDL_CreateTexture(b->ui->ren, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, _w+(2*TILE_W), _h+(2*TILE_H));

	bg_redraw(b);
}


void bg_render(Background* b)
{
	int _w, _h;
	SDL_GetWindowSize(b->ui->win, &_w, &_h);
	SDL_Rect rd = { 
		-TILE_W - b->ui->rx,
		-TILE_H - b->ui->ry,
		_w+(2*TILE_W), _h+(2*TILE_H) };
	SDL_RenderCopy(b->ui->ren, b->tx, NULL, &rd);
}


/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/


static void bg_redraw(Background* b)
{
	SDL_SetRenderTarget(b->ui->ren, b->tx);
	SDL_SetRenderDrawColor(b->ui->ren, 255, 255, 255, 255);
	SDL_RenderClear(b->ui->ren);

	int x1, y1, x2, y2;
	ui_screen_limits(b->ui, &x1, &y1, &x2, &y2);

	for(int x=x1-1; x<=x2+2; x++) {
		for(int y=y1-1; y<=y2+2; y++) {
			bg_draw_tile(b, x, y);
		}
	}

	SDL_SetRenderTarget(b->ui->ren, NULL);
}


static void bg_draw_tile(Background* b, int x, int y)
{
	Object obj;
	Terrain t = world_xy(b->ui->w, x, y, &obj);
	SDL_Rect rs = resources_terrain_rect(b->ui->res, t);
	SDL_Rect rd = { .x = (x*TILE_W) - b->ui->rx, .y = (y*TILE_H) - b->ui->ry,
	                .w = TILE_W, .h = TILE_H };
	SDL_RenderCopy(b->ui->ren, b->ui->res->sprites, &rs, &rd);
}
