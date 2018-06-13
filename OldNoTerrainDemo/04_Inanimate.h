    // ---- Data Types ---- //

typedef struct Jules123 {
    sfSprite* sprite;
    IJ_Point location;
    int ObjectSize; //draw to screen if this radius overlaps view radius
    int Midpoint; //vertical distance down from anchor and midpoint of sprite 
    struct Jules123* Next;
    struct Jules123* Previous;
} SpriteObject;

    // ---- Globals ---- //


SpriteObject SpriteObjects[100];

SpriteObject Head;
SpriteObject Tail;

sfTexture* BridgeTexture;
sfTexture* SpriteSheet;   //I should split up the sprite sheet so theres one for objects and one for units.

sfSprite* Grass[6];

    // ---- Init Function ---- //

int Init_Inanimate (void) {

    Head.location.i = 0;
    Head.location.j = 0;
    Head.Next = &Tail;
    Head.Previous = NULL;

    Tail.location.i = 100000000;
    Tail.location.j = 100000000;
    Tail.Next = NULL;
    Tail.Previous = &Head;

    SpriteSheet = sfTexture_createFromFile("SpriteSheet.bmp", NULL);
    BridgeTexture = sfTexture_createFromFile("Tree.bmp", NULL);

    Grass[0] = sfSprite_create();
    Grass[1] = sfSprite_create();
    Grass[2] = sfSprite_create();
    Grass[3] = sfSprite_create();
    Grass[4] = sfSprite_create();
    Grass[5] = sfSprite_create();

    sfSprite_setTexture(Grass[0], SpriteSheet, sfTrue);
    sfSprite_setTexture(Grass[1], SpriteSheet, sfTrue);
    sfSprite_setTexture(Grass[2], SpriteSheet, sfTrue);
    sfSprite_setTexture(Grass[3], SpriteSheet, sfTrue);
    sfSprite_setTexture(Grass[4], SpriteSheet, sfTrue);
    sfSprite_setTexture(Grass[5], BridgeTexture, sfTrue);

    sfIntRect spriterectangle;
    sfVector2f spriteorigin;

    //Grass0

    spriterectangle.left = 6;
    spriterectangle.top = 122;
    spriterectangle.width = 36;
    spriterectangle.height = 43;
    spriteorigin.x = 19;
    spriteorigin.y = 42;
    sfSprite_setTextureRect (Grass[0], spriterectangle);
    sfSprite_setOrigin (Grass[0], spriteorigin);
    sfSprite_setScale (Grass[0], RightScale);

    //Grass1

    spriterectangle.left = 58;
    spriterectangle.top = 115;
    spriterectangle.width = 24;
    spriterectangle.height = 47;
    spriteorigin.x = 13;
    spriteorigin.y = 48;

    sfSprite_setTextureRect (Grass[1], spriterectangle);
    sfSprite_setOrigin (Grass[1], spriteorigin);
    sfSprite_setScale (Grass[1], RightScale);

    //Grass2

    spriterectangle.left = 98;
    spriterectangle.top = 143;
    spriterectangle.width = 42;
    spriterectangle.height = 20;
    spriteorigin.x = 20;
    spriteorigin.y = 18;

    sfSprite_setTextureRect (Grass[2], spriterectangle);
    sfSprite_setOrigin (Grass[2], spriteorigin);
    sfSprite_setScale (Grass[2], RightScale);

    //Grass3

    spriterectangle.left = 153;
    spriterectangle.top = 137;
    spriterectangle.width = 43;
    spriterectangle.height = 25;
    spriteorigin.x = 23;
    spriteorigin.y = 23;

    sfSprite_setTextureRect (Grass[3], spriterectangle);
    sfSprite_setOrigin (Grass[3], spriteorigin);
    sfSprite_setScale (Grass[3], RightScale);

    //Grass4

    spriterectangle.left = 208;
    spriterectangle.top = 132;
    spriterectangle.width = 51;
    spriterectangle.height = 32;
    spriteorigin.x = 26;
    spriteorigin.y = 28;

    sfSprite_setTextureRect (Grass[4], spriterectangle);
    sfSprite_setOrigin (Grass[4], spriteorigin);
    sfSprite_setScale (Grass[4], RightScale);

    //Grass5 Tree

    spriterectangle.left = 0;
    spriterectangle.top = 0;
    spriterectangle.width = 300;
    spriterectangle.height = 450;
    spriteorigin.x = 158;
    spriteorigin.y = 421;

    sfSprite_setTextureRect (Grass[5], spriterectangle);
    sfSprite_setOrigin (Grass[5], spriteorigin);
    sfSprite_setScale (Grass[5], RightScale);



    int i,j;
    for (i = 0; i < 10; i++ ) {
        for (j = 0; j < 10; j++ ) {
            int k = rand() % 6;
            SpriteObjects[i + 10 * j].sprite = Grass[k];
            SpriteObjects[i + 10 * j].location.i = i * 200 + rand() % 200;
            SpriteObjects[i + 10 * j].location.j = j * 200 + rand() % 200;
            if (k == 5) {
                SpriteObjects[i + 10 * j].ObjectSize = 800;
                SpriteObjects[i + 10 * j].Midpoint = 220;
            } else {
                SpriteObjects[i + 10 * j].ObjectSize = 20;
                SpriteObjects[i + 10 * j].Midpoint = 20;
            }
            SpriteObjects[i + 10 * j].Next = NULL;
            SpriteObjects[i + 10 * j].Previous = NULL;
        }
    }

}

    // ---- Plot Function ----//

// As objects may be in front of or behind units, the plot function for units interleaves objects and units. 



    // ---- Other Functions ---- //



//increment through linked list and insert in the correct position
int SortedInsert(SpriteObject *ToBeInserted) {
    
    SpriteObject* p = Head.Next;
    
    
    while (ToBeInserted->location.i + ToBeInserted->location.j
        > p->location.i + p->location.j) {
        p = p->Next;
        }
        
        ToBeInserted->Next = p;
    ToBeInserted->Previous = p->Previous;
    p->Previous->Next = ToBeInserted;
    p->Previous = ToBeInserted;
    return 5;
}


//Delete object and fix the gap created.
//What if we delete the first element of the list?
//// Need to take a sanity check in case ToBeRemoved is not in visible list.
int SortedRemove(SpriteObject *ToBeRemoved) {
    if (ToBeRemoved->Next->Previous == ToBeRemoved && ToBeRemoved->Previous->Next == ToBeRemoved) {
        ToBeRemoved->Previous->Next = ToBeRemoved->Next;
        ToBeRemoved->Next->Previous = ToBeRemoved->Previous; //got to get these in the right order.
        ToBeRemoved->Next = NULL;
        ToBeRemoved->Previous = NULL;
    }
    return 5;
}

//Clear all objects on the list
int SortedClear(void) {
    Head.Next = &Tail; //Careful not to cause bug in using SortedInsert
    Tail.Previous = &Head;
    // Head -> Tail, Tail -> Head
    return 5;
}

//Sort objects that may have moved in relation to the other objects, no use when working with stationary objects. Will need to block access to list while sorting.
int SortedResort(SpriteObject *Head) {
    
    //sem_post(&ProtectSorted); // Move semaphores to outside of functions.
    return 5;
    
}

int SortedResort2(SpriteObject *Head) {
    //sem_wait(&ProtectSorted); // Move semaphores to outside of functions.
    
    //Will need to block access to list while sorting.
    //Look through list
    //if one element is is less than the previous element, shift left until it fits.
    //sem_post(&ProtectSorted); // Move semaphores to outside of functions.
    return 5;
}

int ListVisible (void) {  //this function cant see spriteunits[0]
    int i;
    for (i = 0; i < 100; i++) {
        //If within VIEW_RADIUS
        if (
            (SpriteObjects[i].location.i - ViewPoint.i + SpriteObjects[i].Midpoint) * (SpriteObjects[i].location.i - ViewPoint.i + SpriteObjects[i].Midpoint)
            
            + (SpriteObjects[i].location.j - ViewPoint.j) * (SpriteObjects[i].location.j - ViewPoint.j)
            
            < VIEW_RADIUS * VIEW_RADIUS+ 2 * VIEW_RADIUS * SpriteObjects[i].ObjectSize + SpriteObjects[i].ObjectSize * SpriteObjects[i].ObjectSize

        ) {
            SortedInsert(&SpriteObjects[i]);
        }
        
    }
    return 5;
}
    // ---- Release Function ---- //

int Release_Inanimate() {

}
