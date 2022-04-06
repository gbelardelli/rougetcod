#include <libtcod.hpp>
#include "Actor.h"
#include "Map.h"
#include "Engine.h"
#include "bsp.h"
#include "params.h"

Engine engine;

char map[MAP_HEIGHT][MAP_WIDTH];

int getRandDirection()
{
    const int dir = rand()%8+1; // Tra 1 e 8
    if( dir < 3 )
        return 1; // Su
    
    if( dir < 5 )
        return 2; // Dx

    if( dir < 7 )
        return 3; // Giù

    return 4;
}


void clearMap(char c)
{
    for(int x=0; x<MAP_WIDTH; x++)
        for(int y=0; y<MAP_HEIGHT; y++)
            map[y][x] = c;
}

void generateMap()
{
    clearMap('#');

    int numFloor=NUM_FLOOR;
    int x,y;

    x=rand()%MAP_WIDTH-2;
    y=rand()%MAP_HEIGHT-2;
    x++;y++;
    map[y][x]='S';

    while(numFloor>0)
    {
        int xdelta=0, ydelta=0;

        for(;;)
        {
            int dir=getRandDirection(); 
            if(dir==1 && y-1 > 0)
                ydelta=-1;
            else if(dir==2 && x+1 < MAP_WIDTH-1)
                xdelta=1;
            else if(dir==3 && y+1 < MAP_HEIGHT-1)
                ydelta=1;
            else if(dir==4 && x-1 > 0)
                xdelta=-1;

            if( xdelta != 0 || ydelta != 0)
                break;
        }

        x+=xdelta;
        y+=ydelta;
        if(map[y][x] == '#')
        {
            map[y][x]= ' ';
            numFloor--;
        }
    }
}

void generateBlob(bool fromMap=true)
{
    // Il blob dovrebbe essere grande quindi dovrebbe occupare
    // almeno il 30% della mappa ma non oltre il 70%.
    char **blobMap;

    int minWidth = std::round(MAP_WIDTH / (100.0f / 30.0f));
    int minHeight = std::round(MAP_HEIGHT / (100.0f / 30.0f));
    int maxWidth = std::round(MAP_WIDTH / (100.0f / 70.0f));
    int maxHeight = std::round(MAP_HEIGHT / (100.0f / 70.0f));

    printf("generateBlob: minw(%d), minh(%d), maxw(%d), maxh(%d)\n", minWidth,
        minHeight, maxWidth, maxHeight);

    int blobWidth = std::rand() % (maxWidth-minWidth+1)+minWidth;
    int blobHeight = std::rand() % (maxHeight-minHeight+1)+minHeight;

    printf("blobDimensions: width(%d) height(%d)\n", blobWidth, blobHeight);

    if(fromMap)
    {
        blobHeight = MAP_HEIGHT;
        blobWidth = MAP_WIDTH;
    }
    blobMap = new char*[blobHeight];
    for(int i=0; i<blobHeight; i++)
        blobMap[i] = new char[blobWidth];

    // Il 45% sono muri
    int numWalls = (blobHeight*blobWidth) / (100.0f / 40.0f);
    if(!fromMap)
    {
        while(numWalls > 0)
        {
            for(;;)
            {
                int x = std::rand() % blobWidth;
                int y = std::rand() % blobHeight;

                if(blobMap[y][x] == ' ')
                {
                    blobMap[y][x] = '#';
                    break;
                }
            }
            numWalls--;
        }

        printf("=====================================================================================\n");
        for( int y=0; y < blobHeight; y++ )
        {
            for( int x=0; x < blobWidth; x++)
            {
                if(blobMap[y][x] == ' ')
                    printf(" ");
                else
                    printf("#");
            }
            printf("\n");
        }
        printf("\n");
    }
    else
    {
        // copia la mappa nel blob
        for(int x=0; x<blobWidth; x++)
            for(int y=0; y<blobHeight; y++)
                blobMap[y][x] = map[y][x];

    }
    // Smoothing
    // In ogni ciclo di smoothing, ogni cella del pavimento con meno di quattro celle 
    // del pavimento adiacenti diventa un muro e ogni cella del muro con sei o più celle 
    // del pavimento adiacenti diventa un pavimento.
    // 0 1 2
    // 3 * 4
    // 5 6 7
    for(int smoothIter=0; smoothIter<5; smoothIter++)
    {
        for( int y=0; y < blobHeight; y++ )
        {
            for( int x=0; x < blobWidth; x++)
            {
                int emptyCnt=0;
                if(blobMap[y][x] == ' ')
                { // Pavimento
                    if(y-1 > 0 && x-1 > 0) // alto a sx
                    {
                        if(blobMap[y-1][x-1] == ' ')
                            emptyCnt++;
                    }
                    if(y-1 > 0) // alto
                    {
                        if(blobMap[y-1][x] == ' ')
                            emptyCnt++;
                    }
                    if(y-1 > 0 && x+1 < blobWidth) // alto a dx
                    {
                        if(blobMap[y-1][x+1] == ' ')
                            emptyCnt++;
                    }
                    if(x-1 > 0) // sx
                    {
                        if(blobMap[y][x-1] == ' ')
                            emptyCnt++;
                    }
                    if(x+1 < blobWidth) // dx
                    {
                        if(blobMap[y][x+1] == ' ')
                            emptyCnt++;
                    }
                    if(y+1 < blobHeight-1 && x-1 > 0) // basso a sx
                    {
                        if(blobMap[y+1][x-1] == ' ')
                            emptyCnt++;
                    }
                    if(y+1 < blobHeight-1) // basso
                    {
                        if(blobMap[y+1][x] == ' ')
                            emptyCnt++;
                    }
                    if(y+1 < blobHeight-1 && x-1 < blobWidth) // basso a dx
                    {
                        if(blobMap[y+1][x+1] == ' ')
                            emptyCnt++;
                    }
                    if(emptyCnt<4)
                        blobMap[y][x] = '#';
                }
                else
                { // Muro
                    int emptyCnt=0;
                    if(y-1 > 0 && x-1 > 0) // alto a sx
                    {
                        if(blobMap[y-1][x-1] == ' ')
                            emptyCnt++;
                    }
                    if(y-1 > 0) // alto
                    {
                        if(blobMap[y-1][x] == ' ')
                            emptyCnt++;
                    }
                    if(y-1 > 0 && x+1 < blobWidth) // alto a dx
                    {
                        if(blobMap[y-1][x+1] == ' ')
                            emptyCnt++;
                    }
                    if(x-1 > 0) // sx
                    {
                        if(blobMap[y][x-1] == ' ')
                            emptyCnt++;
                    }
                    if(x+1 < blobWidth) // dx
                    {
                        if(blobMap[y][x+1] == ' ')
                            emptyCnt++;
                    }
                    if(y+1 < blobHeight-1 && x-1 > 0) // basso a sx
                    {
                        if(blobMap[y+1][x-1] == ' ')
                            emptyCnt++;
                    }
                    if(y+1 < blobHeight-1) // basso
                    {
                        if(blobMap[y+1][x] == ' ')
                            emptyCnt++;
                    }
                    if(y+1 < blobHeight-1 && x-1 < blobWidth) // basso a dx
                    {
                        if(blobMap[y+1][x+1] == ' ')
                            emptyCnt++;
                    }
                    if(emptyCnt>5)
                        blobMap[y][x] == ' ';
                }
            }
        }

        /*printf("Iterazione %d\n", smoothIter+1);
        printf("=====================================================================================\n");
        for( int y=0; y < blobHeight; y++ )
        {
            for( int x=0; x < blobWidth; x++)
            {
                if(blobMap[y][x] == ' ')
                    printf(" ");
                else
                    printf("#");
            }
            printf("\n");
        }
        printf("\n");*/
    }

    clearMap(' ');

    // Copia il blob
    for(int x=0; x<blobWidth; x++)
        for(int y=0; y<blobHeight; y++)
            map[y][x] = blobMap[y][x];

    for(int i = 0; i < blobHeight; ++i)
        delete [] blobMap[i];

    delete [] blobMap;
}

void removeIsolatedWalls(int num)
{
    for(int x=0; x<MAP_WIDTH; x++)
    {
        for(int y=0; y<MAP_HEIGHT; y++)
        {
            if( map[y][x] == '#' )
            {
                int emptyCnt=0;

                if(y-1 > 0 && x-1 > 0) // alto a sx
                {
                    if(map[y-1][x-1] == ' ')
                        emptyCnt++;
                }
                if(y-1 > 0) // alto
                {
                    if(map[y-1][x] == ' ')
                        emptyCnt++;
                }
                if(y-1 > 0 && x+1 < MAP_WIDTH) // alto a dx
                {
                    if(map[y-1][x+1] == ' ')
                        emptyCnt++;
                }
                if(x-1 > 0) // sx
                {
                    if(map[y][x-1] == ' ')
                        emptyCnt++;
                }
                if(x+1 < MAP_WIDTH) // dx
                {
                    if(map[y][x+1] == ' ')
                        emptyCnt++;
                }
                if(y+1 < MAP_HEIGHT-1 && x-1 > 0) // basso a sx
                {
                    if(map[y+1][x-1] == ' ')
                        emptyCnt++;
                }
                if(y+1 < MAP_HEIGHT-1) // basso
                {
                    if(map[y+1][x] == ' ')
                        emptyCnt++;
                }
                if(y+1 < MAP_HEIGHT-1 && x-1 < MAP_WIDTH) // basso a dx
                {
                    if(map[y+1][x+1] == ' ')
                        emptyCnt++;
                }
                if(emptyCnt>=num)
                    map[y][x] = ' ';                
            }
        }
    }
}

void generateRoom(int maxWidth)
{

}

int main() 
{
    unsigned int seed=time(NULL);
    srand( seed );

    printf("SEED = '%d'\n",seed);
    

    generateMap();

    TCODConsole::setCustomFont("terminal16x16_ygs_ro.png", TCOD_FONT_LAYOUT_ASCII_INROW, 16, 16);
    TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);
    
    int cursorX = 0, cursorY = 0;
    while ( !TCODConsole::isWindowClosed() )
    {
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
        //TCODConsole::root->clear();
        if(key.vk != TCODK_NONE && key.c != 0)
        {
            //TCODConsole::root->putCharEx(cursorX,cursorY,key.c, TCOD_azure, TCOD_blue);
            //cursorX++;
            TCODConsole::root->clear();

            if(key.c == 'u')
                generateMap();
            else if(key.c == 's')
                generateBlob();
            else if(key.c == 'i')
                removeIsolatedWalls(6);
            else if(key.c == 'b')
                generateBsp(MAP_WIDTH,MAP_HEIGHT,5);
        }

        int x,y;
        for( y=0;y<MAP_HEIGHT;y++)
        {
            for( x=0;x<MAP_WIDTH;x++)
            {
                TCODConsole::root->printf(x,y,"%c", map[y][x]);
            }
            TCODConsole::root->printf(x,y,"\n");
        }

        TCODConsole::root->printf(0,42, "(u) - Ubriacone");
        TCODConsole::root->printf(0,43, "(s) - Smooth");
        TCODConsole::root->printf(0,44, "(i) - Remove isolated");
        TCODConsole::root->printf(0,45, "(b) - Bsp");
    
        //engine.update();
        //engine.render();
        TCODConsole::flush();
    }
    return 0;
}
