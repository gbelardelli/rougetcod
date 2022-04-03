#include "libtcod.hpp"
#include "Actor.h"
#include "Map.h"
#include "Engine.h"


Engine::Engine() : fovRadius(10), gameStatus(GameStatus::STARTUP), computeFov(true)
{
    TCODConsole::setCustomFont("terminal16x16_ygs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW, 16, 16);

    TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);



	player = new Actor(40, 25, '@', "Player", TCODColor::white);
	actors.push(player);
	map = new Map(80, 45);
}

Engine::~Engine()
{
	actors.clearAndDelete();
	delete map;
}

void Engine::update()
{
    TCOD_key_t key;
    if (gameStatus == GameStatus::STARTUP) map->computeFov();
    gameStatus = GameStatus::IDLE;

    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
    int dx = 0, dy = 0;
    switch (key.vk) 
    {
        case TCODK_UP: dy = -1; break;
        case TCODK_DOWN: dy = 1; break;
        case TCODK_LEFT: dx = -1; break;
        case TCODK_RIGHT: dx = 1; break;
        default:break;
    }

    if (dx != 0 || dy != 0) 
    {
        gameStatus = GameStatus::NEW_TURN;
        if (player->moveOrAttack(player->x + dx, player->y + dy)) 
        {
            map->computeFov();
        }
    }

    if (gameStatus == GameStatus::NEW_TURN) 
    {
        for (Actor** iterator = actors.begin(); iterator != actors.end(); iterator++) 
        {
            Actor* actor = *iterator;
            if (actor != player) 
            {
                actor->update();
            }
        }
    }
}

void Engine::render()
{
    TCODConsole::root->clear();
    map->render();

    // draw the actors
    for (auto actor : actors) 
    {
        if (map->isInFov(actor->x, actor->y)) 
        {
            actor->render();
        }
    }
}