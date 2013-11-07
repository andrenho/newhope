#include "background.h"

#include <assert.h>
#include <stdlib.h>

#include "ui.h"

static Background bg;

static void bg_draw_tile(int x, int y);
//static void bg_draw_person(Person* p);


void bg_init(UI* ui)
{
	bg.tx = NULL;
	bg.ren = ui->ren;
	bg.win = ui->win;
	bg_recreate();
}


void bg_free(Background** b)
{
	if(bg.tx)
		SDL_DestroyTexture(bg.tx);
}


void bg_recreate(Background* b)
{
	int _w, _h;
	SDL_GetWindowSize(bg.win, &_w, &_h);

	if(bg.tx)
		SDL_DestroyTexture(bg.tx);
	bg.tx = SDL_CreateTexture(bg.ren, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, _w, _h);
}


void bg_render()
{
	int _w, _h;
	SDL_GetWindowSize(bg.win, &_w, &_h);
	SDL_RenderCopy(bg.ren, bg.tx, NULL, NULL);
}


void bg_redraw()
{
	SDL_SetRenderTarget(bg.ren, bg.tx);
	SDL_SetRenderDrawColor(bg.ren, 255, 255, 255, 255);
	SDL_RenderClear(bg.ren);

	int x1, y1, x2, y2;
	ui_screen_limits(&x1, &y1, &x2, &y2);

	// draw tiles
	for(int x=x1-1; x<=x2+2; x++) {
		for(int y=y1-1; y<=y2+2; y++) {
			bg_draw_tile(x, y);
		}
	}

	// draw people
	/* TODO
	FOREACH(bg.ui->w->people, Person*, p) {
		if(p->x >= (x1-1) && p->x <= x2 && p->y >= (y1-1) && p->y <= y2) {
			bg_draw_person(p);
		}
	} */

	SDL_SetRenderTarget(bg.ren, NULL);
}


/********************
 *                  *
 * STATIC FUNCTIONS *
 *                  *
 ********************/

extern UI ui;
static void bg_draw_tile(int x, int y)
{
	BLOCK stack[10];
	uint8_t n = if_world_tile_stack(x, y, stack);
	assert(n > 0);

	// draw block
	SDL_Rect rs = resources_terrain_rect(stack[n-1]);
	SDL_Rect rd = { .x = (x*TILE_W) - ui.rx, .y = (y*TILE_H) - ui.ry,
	                .w = TILE_W, .h = TILE_H };
	SDL_RenderCopy(bg.ren, sprites, &rs, &rd);

	// TODO draw object - ?
}


/*
static void bg_draw_person(Background* b, Person* p)
{
	// body
	SDL_Rect rp, ra;
	resources_person_rect(bg.ui->res, p, &rp, &ra);
	SDL_Rect rd = { .x = (p->x * TILE_W) - bg.ui->rx - TILE_W/2, 
		        .y = (p->y * TILE_H) - bg.ui->ry - TILE_H/2,
			.w = TILE_W, .h = TILE_H };
	SDL_RenderCopy(bg.ren, bg.ui->res->sprites, &rp, &rd);
	SDL_RenderCopyEx(bg.ren, bg.ui->res->sprites, &ra, &rd, p->direction, 
			NULL, SDL_FLIP_NONE);
}*/
