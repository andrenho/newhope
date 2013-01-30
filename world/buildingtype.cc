#include "world/buildingtype.h"

/* w1..9 = house wall
 * d1..2 = house door
 */

const BuildingType BuildingType::BANK = BuildingType(7, 3,
		{ "w1w2w2w2w2w2w3",
		  "w4w5w5w5w5d1w6",
		  "w7w8w8w8w8d2w9" });
