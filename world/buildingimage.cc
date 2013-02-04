#include "world/buildingimage.h"

/* MEDIEVAL
 *   w1..9 = house wall
 *   ws    = sign
 *   dn..s = house door (dN: with extra decoration)
 *   R1..9 = roof corners
 *   RL/RR = roof inner corners
 *   r1..9 = roof
 *   C6    = roof with chimney
 *   Ww    = window (bottom, top is automatic, WW: extra decor)
 */

const BuildingImage BuildingImage::VICTORIAN_BANK = BuildingImage(9, 7,
		{ "R1r1r2r2r2r2r2r3R3",
		  "R4r7r8r8r8r8r5C6R6",
		  "R7R8R8R8R8RLr7r9R6",
		  "w1w2w2w2w2R7R8R8R9",
		  "w4WWw5WWw5w1w2w2w3",
		  "w7w8w8w8w8w4wsdNw6",
		  "          w7w8dsw9" });
