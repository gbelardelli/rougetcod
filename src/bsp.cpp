#include "bsp.h"
#include <cstdlib>
#include <cmath>
int rand_range(int min, int max)
{
    return rand()%(max - min + 1) + min;
}

PTree splitContainer( Container* container, int iter, int val)
{
    PTree root = new Tree(container);

    if (iter != 0) 
    {
        printf("splitContainer(%d): %d,%d,%d,%d {w:%d h:%d}   iter: %d\n", val, container->x, container->y, container->x+container->w, container->y+container->h, container->w, container->h, iter);

        Container *split1=nullptr, *split2=nullptr;
        random_split(container,&split1,&split2);
        
        if(split1 != nullptr)
            root->left=splitContainer(split1, iter-1, 1);
        if(split2 != nullptr)
            root->right=splitContainer(split2, iter-1, 2);
    }

    return root;
}

void random_split( Container* container,  Container** split1,  Container** split2 )
{
    constexpr int MIN_BSP_HEIGHT=8;
    constexpr int MIN_BSP_WIDTH=8;
    constexpr int SPLIT_TOLERANCE=4;

    Container *r1, *r2;
    bool horizontalSplit = ( rand_range(1,6) < 4 ? true : false );
    const int halfWidth = std::round(((float)container->w * 0.5f) + 0.5f);
    const int halfHeight = std::round(((float)container->h * 0.5f) + 0.5f);

    if(horizontalSplit)
    {
        if(container->h-MIN_BSP_HEIGHT < MIN_BSP_HEIGHT)
            horizontalSplit=false;
    }
    else
    {
        if(container->w-MIN_BSP_WIDTH < MIN_BSP_WIDTH)
            horizontalSplit=true;
    }

    if(horizontalSplit)
    {
        // Orizzontale
        if(container->h-MIN_BSP_HEIGHT < MIN_BSP_HEIGHT)
        {
            printf("   random_split 'HORIZONTAL' no split halfHeight: %d\n",halfHeight);
            r1=nullptr;
            r2=nullptr;
        }
        else
        {
            int rand_height = rand_range(MIN_BSP_HEIGHT, container->h-MIN_BSP_HEIGHT);
            if(rand_height < MIN_BSP_HEIGHT || rand_height > container->h)
            {
                printf("   random_split 'NOOOOOOOOOOOOOOOOOOOOOOOOO' halfHeight: %d\n",halfHeight);
                r1=nullptr;
                r2=nullptr;     
            }
            else
            {        
                if(container->h < 1)
                    printf("SPLIT H < 1!!!\n");

                printf("   random_split 'HORIZONTAL' rand_height: %d  halfHeight: %d\n", rand_height, halfHeight);
                r1=new Container(container->x, container->y, container->w, rand_height);
                r2=new Container(container->x, container->y+r1->h, container->w, container->h-r1->h);
            }
        }
    }
    else
    {
        if(container->w-MIN_BSP_WIDTH < MIN_BSP_WIDTH)
        {
            printf("   random_split 'VERTICAL' no split halfWidth: %d\n",halfWidth);
            r1=nullptr;
            r2=nullptr;
        }
        else
        {
            // Verticale
            int rand_width = rand_range(MIN_BSP_WIDTH, container->w-MIN_BSP_WIDTH);
            if(halfWidth < MIN_BSP_WIDTH || rand_width > container->w)
            {
                printf("   random_split 'NOOOOOOOOOOOOOOOOOOOOOOOOO' no split\n");
                r1=nullptr;
                r2=nullptr;
            }        
            else
            {
                if(container->w < 1)
                    printf("SPLIT W < 1!!!\n");
                
                printf("   random_split 'VERTICAL' rand_width: %d\n", rand_width);
                r1=new Container(container->x, container->y, rand_width, container->h);
                r2=new Container(container->x+r1->w, container->y, container->w-r1->w, container->h);
            }
        }
    }

    if(r1 != nullptr)
        printf("       r1: %d,%d,%d,%d {w:%d h:%d}\n", r1->x, r1->y, r1->x+r1->w, r1->y+r1->h, r1->w, r1->h);

    if(r2 != nullptr)
        printf("       r2: %d,%d,%d,%d {w:%d h:%d}\n", r2->x, r2->y, r2->x+r2->w, r2->y+r2->h, r2->w, r2->h);    

    *split1=r1;
    *split2=r2;

}

void random_split2( Container* container,  Container** split1,  Container** split2 )
{
    Container *r1, *r2;
    int val = rand_range(1,6);
    
    constexpr int MIN_BSP_HEIGHT=5;
    constexpr int MIN_BSP_WIDTH=5;
 
    // Se prova a splittare orizzontalmente una room con altezza minore
    // di 6 allora la splitta in verticale.
    /*if( val < 4 && container->h < 6)
        val = 4;

    // Se prova a splittare verticalmente una room con larghezza minore
    // di 6 allora la splitta in orizzontale.
    if( val > 3 && container->w < 6)
        val = 3;*/

    if(container->w > container->h && (float)container->w / (float)container->h >= 1.25f)
        val = 4;
    else if(container->h > container->w && (float)container->h / (float)container->w >= 1.25f)
        val = 3;
    
    if(val < 4 )
    {
        // Orizzontale
        int rand_height;
        if(container->h < 1)
            printf("SPLIT H < 1!!!\n");
        /*if(container->h < MIN_BSP_HEIGHT)
        {
            // Se la height non 
            printf("   random_split 'HORIZONTAL' no split\n");
            r1=new Container(container->x, container->y, container->w, container->h);
            r2=new Container(container->x, container->y, container->w, container->h);
        }
        else
        {
            if(container->h-MIN_BSP_HEIGHT < MIN_BSP_HEIGHT )
            {
                printf("   random_split 'HORIZONTAL' no split no min\n");
                r1=new Container(container->x, container->y, container->w, container->h);
                //r2=new Container(container->x, container->y, container->w, container->h);
                r2=nullptr;
            }
            else
            {*/
                rand_height = rand_range(MIN_BSP_HEIGHT, container->h-MIN_BSP_HEIGHT);
                printf("   random_split 'HORIZONTAL' rand_height: %d\n", rand_height);
                r1=new Container(container->x, container->y, container->w, rand_height);
                r2=new Container(container->x, container->y+r1->h, container->w, container->h-r1->h);
       /*     }
        }*/
    }
    else
    {
        // Verticale
        int rand_width;
        if(container->w < 1)
            printf("SPLIT W < 1!!!\n");
        
        /*if(container->w < MIN_BSP_WIDTH)
        {
            printf("   random_split 'VERTICAL' no split\n");
            r1=new Container(container->x, container->y, container->w, container->h);
            r2=new Container(container->x, container->y, container->w, container->h);
        }
        else
        {
            if(container->w-MIN_BSP_WIDTH < MIN_BSP_WIDTH)
            {
                printf("   random_split 'VERTICAL' no split no min\n");
                r1=new Container(container->x, container->y, container->w, container->h);
                //r2=new Container(container->x, container->y, container->w, container->h);
                r2=nullptr;
            }
            else
            {*/
                rand_width = rand_range(MIN_BSP_WIDTH, container->w-MIN_BSP_WIDTH);
                printf("   random_split 'VERTICAL' rand_width: %d\n", rand_width);
                r1=new Container(container->x, container->y, rand_width, container->h);
                r2=new Container(container->x+r1->w, container->y, container->w-r1->w, container->h);
       /*     }
        }*/
    }

    if(r1 != nullptr)
        printf("       r1: %d,%d,%d,%d {w:%d h:%d}\n", r1->x, r1->y, r1->x+r1->w, r1->y+r1->h, r1->w, r1->h);

    if(r2 != nullptr)
        printf("       r2: %d,%d,%d,%d {w:%d h:%d}\n", r2->x, r2->y, r2->x+r2->w, r2->y+r2->h, r2->w, r2->h);    

    *split1=r1;
    *split2=r2;
}

void generateBsp(int w, int h, int iterations)
{
    printf("============ NEW BSP ================\n");
    Container* rootCont = new Container(0,0,w-1,h-1);
    PTree mapTree = splitContainer(rootCont, iterations, 0);

    clearMap(' ');
    mapTree->paint();
    printf("============ END BSP ================\n");
}