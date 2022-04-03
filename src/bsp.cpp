#include "bsp.h"
#include <cstdlib>

int rand_range(int min, int max)
{
    return rand()%(max - min + 1) + min;
}

PTree splitContainer( Container* container, int iter)
{
    PTree root = new Tree(container);

    if (iter != 0) 
    {
        Container *split1=nullptr, *split2=nullptr;
        random_split(container,&split1,&split2);

        root->left=splitContainer(split1, iter-1);
        root->right=splitContainer(split2, iter-1);
    }

    return root;
}

void random_split( Container* container,  Container** split1,  Container** split2 )
{
    Container *r1, *r2;
    int val = rand_range(1,6);
    
    // Se prova a splittare orizzontalmente una room con altezza minore
    // di 6 allora la splitta in verticale.
    if( val < 4 && container->h < 6)
        val = 4;

    // Se prova a splittare verticalmente una room con larghezza minore
    // di 6 allora la splitta in orizzontale.
    if( val > 3 && container->w < 6)
        val = 3;

    if(val < 4 )
    {
        // Orizzontale
        if(container->h < 1)
            printf("Minore di uno");
        r1=new Container(container->x, container->y, container->w,  rand_range(1, container->h));

        if(container->h-r1->h < 1)
            printf("Minore di uno");
        r2=new Container(container->x, container->y+r1->h, container->w, container->h-r1->h);
    }
    else
    {
        // Verticale
        if(container->w < 1)
            printf("Minore di uno");
        r1=new Container(container->x, container->y, rand_range(1, container->w), container->h);

        if(container->w-r1->w < 1)
            printf("Minore di uno");
        r2=new Container(container->x+r1->w, container->y, container->w-r1->w, container->h);
    }

    *split1=r1;
    *split2=r2;
}

void generateBsp(int w, int h, int iterations)
{
    Container* rootCont = new Container(0,0,w-1,h-1);
    PTree mapTree = splitContainer(rootCont, iterations);

    clearMap(' ');
    mapTree->paint();
}