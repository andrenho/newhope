#ifndef UI_LAYERTERRAIN_H
#define UI_LAYERTERRAIN_H

class LayerTerrain : public Layer {
public:
	LayerTerrain(Terrain t) : terrain_(t) { }
	void Render() const;

private:
	void DrawTile(int x, int y) const;
	string TerrainStr() const;
	void TileSuffixes(int x, int y, vector<string>& s) const;
	void SpecialSuffix(int special, vector<string>& s) const;

	Terrain terrain_;
};

#endif
