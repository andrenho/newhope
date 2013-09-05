#ifndef ENGINE_BUILDINGTYPE_H
#define ENGINE_BUILDINGTYPE_H

typedef enum BuildingClass {
	HOUSE,
} BuildingClass;

typedef struct BuildingLayout {
	int w, h;
	int door_x, door_y;
} BuildingLayout;

typedef struct BuildingType {
	BuildingClass type;
	BuildingLayout *layout;
} BuildingType; 

extern BuildingType bd_type[];

#endif
