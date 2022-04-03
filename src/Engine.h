#pragma once

class Actor;
class Map;

class Engine
{
public:
	enum class GameStatus 
	{
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT
	}gameStatus;

	TCODList<Actor*> actors;
	Actor* player;
	Map* map;
	int fovRadius;
	Engine();
	~Engine();
	void update();
	void render();

private:
	bool computeFov;
};

extern Engine engine;
