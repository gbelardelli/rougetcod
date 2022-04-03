#pragma once
struct Tile {
	bool canWalk; // can we walk through this tile?
	bool explored;
	Tile() : canWalk(false),explored(false) {}
};

class Map {
public:
	int width, height;

	Map(int width, int height);
	~Map();
	bool isWall(int x, int y) const;
	void render() const;

	bool isInFov(int x, int y) const;
	bool isExplored(int x, int y) const;
	void computeFov();
	bool canWalk(int x, int y) const;
	void addMonster(int x, int y);
protected:
	Tile* tiles;
	TCODMap* map;
	void setWall(int x, int y);

	friend class BspListener;

	void dig(int x1, int y1, int x2, int y2);
	void createRoom(bool first, int x1, int y1, int x2, int y2);
};