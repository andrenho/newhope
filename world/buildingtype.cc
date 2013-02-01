#include "world/buildingtype.h"

/* MEDIEVAL
 *   w1..9 = house wall
 *   wl    = logo
 *   d1..2 = house door
 *   R1..9 = roof corners
 *   RL/RR = roof inner corners
 *   r1..9 = roof
 */

const BuildingType BuildingType::BANK = BuildingType(9, 7,
		{ "R1r1r2r2r2r2r2r3R3",
		  "R4r7r8r8r8r8r5r6R6",
		  "R7R8R8R8R8RLr7r9R6",
		  "w1w2w2w2w2R7R8R8R9",
		  "w4w5w5w5w5w1w2w2w3",
		  "w7w8w8w8w8w4wld1w6",
		  "          w7w8d2w9" });
