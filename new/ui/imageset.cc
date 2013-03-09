#include "defines.h"

Imageset::Imageset()
	: tileset_(Image::LoadImage("tiles.png"))
{
	delete tileset_;
}
