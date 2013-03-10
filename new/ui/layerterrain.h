#ifndef UI_LAYERTERRAIN_H
#define UI_LAYERTERRAIN_H

class LayerTerrain : public Layer {
public:
	LayerTerrain(Terrain t) : terrain_(t) { }
	void Render() const;

private:
	void DrawTile(int x, int y) const;
	string TerrainStr() const;
	string TileSuffix(int x, int y) const;

	Terrain terrain_;
};

#endif
