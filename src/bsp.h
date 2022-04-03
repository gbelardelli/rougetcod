#ifndef _SRC_BSP_H_
#define _SRC_BSP_H_

#include "params.h"
#include <stdio.h>

// Sarebbe la room
class Container
{
public:
    int x;
    int y;
    int w;
    int h;
    int centerX;
    int centerY;

public:
    Container() : x(0), y(0), w(0), h(0), centerX(0), centerY(0) {}

    Container(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {
        centerX = x + (w/2);
        centerY = y + (h/2);
    }

    void paint() const
    {
        for(int i=0;i<w; i++)
        {
            if(y<0 || y>MAP_HEIGHT || y+h>MAP_HEIGHT || x<0 || x>MAP_WIDTH || x+i >MAP_WIDTH )
                printf("Eccetelo\n");

            map[y][x+i]='#';
            map[y+h][x+i]='#';
        }

        for(int i=0;i<h; i++)
        {
            if(y<0 || y>MAP_HEIGHT || y+h>MAP_HEIGHT || x<0 || x>MAP_WIDTH || x+i >MAP_WIDTH )
                printf("Eccetelo\n");

            map[y+i][x]='#';
            map[y+i][x+w]='#';
        }
    }
};


typedef struct _snode
{
    Container *container;
    struct _snode *left;
    struct _snode *right;

    _snode() : left(nullptr), right(nullptr), container(nullptr) {}
    _snode(Container *cont) : left(nullptr), right(nullptr), container(cont) {}

    void paint() const
    {
        container->paint();
        if( left != nullptr )
            left->paint();
        if( right != nullptr )
            right->paint();

    }
}Tree, *PTree;

PTree splitContainer( Container* container, int iter);
void random_split( Container* container,  Container** split1,  Container** split2 );
int rand_range(int min, int max);

void generateBsp(int w, int h, int iterations);
#endif// _SRC_BSP_H_
