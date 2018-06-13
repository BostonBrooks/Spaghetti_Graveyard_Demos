//To do: implement flying eagle as projectile








   // ---- Data Types ---- //

typedef struct {
    sfSprite* Facet[13];
    int ProjectileSpeed;
//0 up, 7 right, 14 down
} Projectile;

/*
typedef struct {
    int index;
    char WeaponName[16];
    Projectile* Sprite;
    void (Ineract_Weapon)(WeaponInstance*, Char[]*)
//how to represent diverse array of weapons, with differing interfaces? use different functions
} WeaponType;
*/
struct SpriteUnit;

typedef struct Chrystle {
    Projectile* Type;
    int FacetNumber;
    int DirectionFacing;
    //int Damage;
    //void (*Effects)(struct Chrystle*);
    IJ_Point StartPoint;
    int StartTime;
    IJ_Point EndPoint;
    int EndTime;
    IJ_Point Location;
    int kHeight;
    //struct SpriteUnit *Target;
    struct Chrystle* Next;
    struct Chrystle* Previous;
    struct Chrystle* SortedNext;
    struct Chrystle* SortedPrevious;
} ProjectileInstance;
/*
typedef struct {
    WeaponType* Type;
    int Loaded;
    int Durbility;
    //...
} WeaponInstance;
*/
    // ---- Globals ---- //


sfTexture* ProjectileTexture;
Projectile Eagle;
ProjectileInstance PIHead;
ProjectileInstance PITail;


    // ---- Init Function ---- //

int Init_Weapons (void) {

    PIHead.StartPoint.i = 0;
    PIHead.StartPoint.j = 0;
    PIHead.StartTime = 0;
    PIHead.EndPoint.i = 0;
    PIHead.EndPoint.j = 0;
    PIHead.EndTime = 0;
    PIHead.Location.i = 0;
    PIHead.Location.j = 0;
    PIHead.Next = &PITail;
    PIHead.Previous = NULL;
    PIHead.SortedNext = &PITail;
    PIHead.SortedPrevious = NULL;

    PITail.StartPoint.i = 100000000;
    PITail.StartPoint.j = 100000000;
    PITail.StartTime = 0;
    PITail.EndPoint.i = 100000000;
    PITail.EndPoint.j = 100000000;
    PITail.EndTime = 0;
    PITail.Location.i = 100000000;
    PITail.Location.j = 100000000;
    PITail.Next = NULL;
    PITail.Previous = &PIHead;
    PITail.SortedNext = NULL;
    PITail.SortedPrevious = &PIHead;

// Init ProjectileTexture
    ProjectileTexture = sfTexture_createFromFile("Projectiles.bmp", NULL);
// Init Eagle
    Eagle.Facet[0] = sfSprite_create(); //down                  //Projectiles have FLAT trajectory
    Eagle.Facet[1] = sfSprite_create();
    Eagle.Facet[2] = sfSprite_create();
    Eagle.Facet[3] = sfSprite_create();
    Eagle.Facet[4] = sfSprite_create();
    Eagle.Facet[5] = sfSprite_create();
    Eagle.Facet[6] = sfSprite_create();
    Eagle.Facet[7] = sfSprite_create();//right 
    Eagle.Facet[8] = sfSprite_create();
    Eagle.Facet[9] = sfSprite_create();
    Eagle.Facet[10] = sfSprite_create();
    Eagle.Facet[11] = sfSprite_create();
    Eagle.Facet[12] = sfSprite_create();//up

    sfSprite_setTexture(Eagle.Facet[0], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[1], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[2], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[3], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[4], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[5], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[6], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[7], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[8], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[9], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[10], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[11], ProjectileTexture, sfTrue);
    sfSprite_setTexture(Eagle.Facet[12], ProjectileTexture, sfTrue);

    sfIntRect spriterectangle;
    sfVector2f spriteorigin;

    //Eagle Facet 0

    spriterectangle.left = 72;
    spriterectangle.top = 136;
    spriterectangle.width = 102;
    spriterectangle.height = 30;
    spriteorigin.x = 52;
    spriteorigin.y = 14;
    sfSprite_setTextureRect (Eagle.Facet[0], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[0], spriteorigin);
    sfSprite_setScale (Eagle.Facet[0], RightScale);

    //Eagle Facet 1

    spriterectangle.left = 182;
    spriterectangle.top = 130;
    spriterectangle.width = 98;
    spriterectangle.height = 40;
    spriteorigin.x = 47;
    spriteorigin.y = 22;
    sfSprite_setTextureRect (Eagle.Facet[1], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[1], spriteorigin);
    sfSprite_setScale (Eagle.Facet[1], RightScale);

    //Eagle Facet 2

    spriterectangle.left = 303;
    spriterectangle.top = 128;
    spriterectangle.width = 87;
    spriterectangle.height = 42;
    spriteorigin.x = 47;
    spriteorigin.y = 22;
    sfSprite_setTextureRect (Eagle.Facet[2], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[2], spriteorigin);
    sfSprite_setScale (Eagle.Facet[2], RightScale);

    //Eagle Facet 3

    spriterectangle.left = 416;
    spriterectangle.top = 122;
    spriterectangle.width = 80;
    spriterectangle.height = 55;
    spriteorigin.x = 44;
    spriteorigin.y = 28;
    sfSprite_setTextureRect (Eagle.Facet[0], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[0], spriteorigin);
    sfSprite_setScale (Eagle.Facet[0], RightScale);

    //Eagle Facet 4

    spriterectangle.left = 512;
    spriterectangle.top = 123;
    spriterectangle.width = 55;
    spriterectangle.height = 61;
    spriteorigin.x = 32;
    spriteorigin.y = 32;
    sfSprite_setTextureRect (Eagle.Facet[4], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[4], spriteorigin);
    sfSprite_setScale (Eagle.Facet[4], RightScale);

    //Eagle Facet 5 

    spriterectangle.left = 613;
    spriterectangle.top = 116;
    spriterectangle.width = 43;
    spriterectangle.height = 74;
    spriteorigin.x = 25;
    spriteorigin.y = 40;
    sfSprite_setTextureRect (Eagle.Facet[5], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[5], spriteorigin);
    sfSprite_setScale (Eagle.Facet[5], RightScale);

    //Eagle Facet 6

    spriterectangle.left = 702;
    spriterectangle.top = 118;
    spriterectangle.width = 38;
    spriterectangle.height = 69;
    spriteorigin.x = 20;
    spriteorigin.y = 39;
    sfSprite_setTextureRect (Eagle.Facet[6], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[6], spriteorigin);
    sfSprite_setScale (Eagle.Facet[6], RightScale);

    //Eagle Facet 7

    spriterectangle.left = 776;
    spriterectangle.top = 117;
    spriterectangle.width = 40;
    spriterectangle.height = 73;
    spriteorigin.x = 21;
    spriteorigin.y = 43;
    sfSprite_setTextureRect (Eagle.Facet[7], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[7], spriteorigin);
    sfSprite_setScale (Eagle.Facet[7], RightScale);

    //Eagle Facet 8

    spriterectangle.left = 838;
    spriterectangle.top = 128;
    spriterectangle.width = 56;
    spriterectangle.height = 63;
    spriteorigin.x = 30;
    spriteorigin.y = 38;
    sfSprite_setTextureRect (Eagle.Facet[8], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[8], spriteorigin);
    sfSprite_setScale (Eagle.Facet[8], RightScale);


    //Eagle Facet 9

    spriterectangle.left = 912;
    spriterectangle.top = 133;
    spriterectangle.width = 68;
    spriterectangle.height = 53;
    spriteorigin.x = 36;
    spriteorigin.y = 33;
    sfSprite_setTextureRect (Eagle.Facet[9], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[9], spriteorigin);
    sfSprite_setScale (Eagle.Facet[9], RightScale);


    //Eagle Facet 10

    spriterectangle.left = 1010;
    spriterectangle.top = 144;
    spriterectangle.width = 88;
    spriterectangle.height = 42;
    spriteorigin.x = 44;
    spriteorigin.y = 26;
    sfSprite_setTextureRect (Eagle.Facet[10], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[10], spriteorigin);
    sfSprite_setScale (Eagle.Facet[10], RightScale);


    //Eagle Facet 11

    spriterectangle.left = 1104;
    spriterectangle.top = 144;
    spriterectangle.width = 90;
    spriterectangle.height = 42;
    spriteorigin.x = 46;
    spriteorigin.y = 23;
    sfSprite_setTextureRect (Eagle.Facet[11], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[11], spriteorigin);
    sfSprite_setScale (Eagle.Facet[11], RightScale);


    //Eagle Facet 12

    spriterectangle.left = 1225;
    spriterectangle.top = 154;
    spriterectangle.width = 97;
    spriterectangle.height = 28;
    spriteorigin.x = 49;
    spriteorigin.y = 15;
    sfSprite_setTextureRect (Eagle.Facet[12], spriterectangle);
    sfSprite_setOrigin (Eagle.Facet[12], spriteorigin);
    sfSprite_setScale (Eagle.Facet[12], RightScale);




}

    // ---- Plot Function ---- //

// As projectiles may be in front of or behind units, the plot function for units interleaves projectiles and units. 

    // ---- Other Functions ---- //
/*
int Interact_Xbow(WeaponInstance *Instance, char[] *TextInput);
*/


int Delete_Projectile(ProjectileInstance *TBDeleted){
   TBDeleted->Next->Previous = TBDeleted->Previous;
   TBDeleted->Previous->Next = TBDeleted->Next;
   TBDeleted->SortedNext->SortedPrevious = TBDeleted->SortedPrevious;
   TBDeleted->SortedPrevious->SortedNext = TBDeleted->SortedNext;
   free(TBDeleted);

}

int ProjectileHit(ProjectileInstance *Proj, struct SpriteUnit *Target) {

   Delete_Projectile(Proj);
}

int Reach_Endpoint (ProjectileInstance *Proj) { //with the projectile as an argument, this is a subset of Update_Projectile
//   if (Proj->Target != NULL && Proj->Target->Alive) {
//       DamageUnit(Proj->Target, Proj->Damage);
//   }
//   if (Proj->Effects != NULL) {
//       (Proj->Effects)(Proj);
//   }
   Delete_Projectile(Proj);
}

int ClearVisibleProjectiles(void) {
    PIHead.SortedNext = &PITail; //Careful not to cause bug in using SortedInsert
    PITail.SortedPrevious = &PIHead;
    // Head -> Tail, Tail -> Head
    return 5;
}

int SortedProjectileInsert(ProjectileInstance *ToBeInserted) {

    ProjectileInstance* p = PIHead.SortedNext;

    while (ToBeInserted->Location.i + ToBeInserted->Location.j
        > p->Location.i + p->Location.j) {
        p = p->SortedNext;
        }
        
        ToBeInserted->SortedNext = p;
    ToBeInserted->SortedPrevious = p->SortedPrevious;
    p->SortedPrevious->SortedNext = ToBeInserted;
    p->SortedPrevious = ToBeInserted;
    return 5;
}



int ListVisibleProjectiles(void) {
    //Generates linked list of visible units
    //Loop through unit locations.
    ProjectileInstance* p = PIHead.Next;
    while (p != &PITail) {                                                    //If Alive
        //If within VIEW_RADIUS
        if ((p->Location.i - ViewPoint.i) * (p->Location.i - ViewPoint.i)
                
            + (p->Location.j - ViewPoint.j) * (p->Location.j - ViewPoint.j)
            
            < VIEW_RADIUS * VIEW_RADIUS) {
        
            SortedProjectileInsert(p);
        }
        p = p->Next;
    }
    return 5;
}


int UpdateProjectiles(void) {
//Check for collisions

//Check if the projectile has reached its endpoint

//Update location 
}

int CreateProjectile(void) { //with properties.

//Set type
//Set start and end points
//set start and end time
//set facet number
//set left/right

}


    // ---- Release Function ---- //

int Release_Weapons(void) {

}
