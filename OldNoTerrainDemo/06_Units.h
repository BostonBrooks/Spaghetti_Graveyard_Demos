    // ---- Data Types ---- //

typedef struct {
    int index;
    char (*UnitName)[];
    sfSprite* sprite;
    float speed;
    int footprintsize;
    int height;     //How high to display health bar
    int maxhealth;
    int minrange;
    int maxrange;
    int minattack;
    int maxattack;
    int pausebefore;
    int pauseafter;
    Projectile* projectile;
    int projectilespeed;
    int agrorange; //if within weapon range, attack else if too close to attack, retreat else if too far to attack but within agro range, advance.
} UnitType;



typedef struct SpriteUnit {
    int index;
    UnitType* Type;
    int Alive;
    int Health;
    int DisplayHealthUntil;
    IJ_Point StartPoint;
    int StartTime;
    IJ_Point EndPoint;
    int EndTime;
    IJ_Point location;
    IJ_Point GoalPoint;
    IJ_Point Forces;
    int DirectionFacing;
    struct SpriteUnit* Next;
    struct SpriteUnit* Previous;
} SpriteUnit;




    // ---- Globals ---- //

#define HEALTHBARWIDTH 20 //( times 2)
#define HEALTHBARHEIGHT 5

#define MAXFORCES 10000
#define CRITICALDISTANCE 15

#define LEFTFACING 0
#define RIGHTFACING 1

SpriteUnit SpriteUnits[100];

SpriteUnit UnitsHead;
SpriteUnit UnitsTail;

SpriteUnit* UnitSelected;

sfSprite* Units[7];

UnitType UnitTypes[7];


    // ---- Init Function ---- //

int Init_Units (void) {

    UnitsHead.StartPoint.i = 0;
    UnitsHead.StartPoint.j = 0;
    UnitsHead.StartTime = 0;
    UnitsHead.EndPoint.i = 0;
    UnitsHead.EndPoint.j = 0;
    UnitsHead.EndTime = 0;
    UnitsHead.location.i = 0;
    UnitsHead.location.j = 0;
    UnitsHead.Next = &UnitsTail;
    UnitsHead.Previous = NULL;

    UnitsTail.StartPoint.i = 100000000;
    UnitsTail.StartPoint.j = 100000000;
    UnitsTail.StartTime = 0;
    UnitsTail.EndPoint.i = 100000000;
    UnitsTail.EndPoint.j = 100000000;
    UnitsTail.EndTime = 0;
    UnitsTail.location.i = 100000000;
    UnitsTail.location.j = 100000000;
    UnitsTail.Next = NULL;
    UnitsTail.Previous = &UnitsHead;

    Units[0] = sfSprite_create();
    Units[1] = sfSprite_create();
    Units[2] = sfSprite_create();
    Units[3] = sfSprite_create();
    Units[4] = sfSprite_create();
    Units[5] = sfSprite_create();
    Units[6] = sfSprite_create();

    sfSprite_setTexture(Units[0], SpriteSheet, sfTrue);
    sfSprite_setTexture(Units[1], SpriteSheet, sfTrue);
    sfSprite_setTexture(Units[2], SpriteSheet, sfTrue);
    sfSprite_setTexture(Units[3], SpriteSheet, sfTrue);
    sfSprite_setTexture(Units[4], SpriteSheet, sfTrue);
    sfSprite_setTexture(Units[5], SpriteSheet, sfTrue);
    sfSprite_setTexture(Units[6], SpriteSheet, sfTrue);

    sfIntRect spriterectangle;
    sfVector2f spriteorigin;

 
    //Units0 Crossbow

    spriterectangle.left = 2;
    spriterectangle.top = 2;
    spriterectangle.width = 67;
    spriterectangle.height = 99;
    spriteorigin.x = 23;
    spriteorigin.y = 96;

    sfSprite_setTextureRect (Units[0], spriterectangle);
    sfSprite_setOrigin (Units[0], spriteorigin);

    //Units1 Skeleton Archer

    spriterectangle.left = 78;
    spriterectangle.top = 3;
    spriterectangle.width = 60;
    spriterectangle.height = 103;
    spriteorigin.x = 19;
    spriteorigin.y = 100;

    sfSprite_setTextureRect (Units[1], spriterectangle);
    sfSprite_setOrigin (Units[1], spriteorigin);

    //Units2 Imp

    spriterectangle.left = 148;
    spriterectangle.top = 35;
    spriterectangle.width = 62;
    spriterectangle.height = 82;
    spriteorigin.x = 18;
    spriteorigin.y = 71;

    sfSprite_setTextureRect (Units[2], spriterectangle);
    sfSprite_setOrigin (Units[2], spriteorigin);

    //Units3 Zombie
    
    spriterectangle.left = 210;
    spriterectangle.top = 0;
    spriterectangle.width = 58;
    spriterectangle.height = 107;
    spriteorigin.x = 32;
    spriteorigin.y = 104;
    
    sfSprite_setTextureRect (Units[3], spriterectangle);
    sfSprite_setOrigin (Units[3], spriteorigin);
    
    //Units4 Possesed Cow
    
    spriterectangle.left = 280;
    spriterectangle.top = 9;
    spriterectangle.width = 93;
    spriterectangle.height = 155;
    spriteorigin.x = 33;
    spriteorigin.y = 152;
    
    sfSprite_setTextureRect (Units[4], spriterectangle);
    sfSprite_setOrigin (Units[4], spriteorigin);
    
    //Units5 Thorny Devil
    
    spriterectangle.left = 375;
    spriterectangle.top = 15;
    spriterectangle.width = 85;
    spriterectangle.height = 97;
    spriteorigin.x = 56;
    spriteorigin.y = 95;
    
    sfSprite_setTextureRect (Units[5], spriterectangle);
    sfSprite_setOrigin (Units[5], spriteorigin);
    
    //Units6 Frilled Neck
    
    spriterectangle.left = 473;
    spriterectangle.top = 13;
    spriterectangle.width = 85;
    spriterectangle.height = 96;
    spriteorigin.x = 55;
    spriteorigin.y = 95;
    
    sfSprite_setTextureRect (Units[6], spriterectangle);
    sfSprite_setOrigin (Units[6], spriteorigin);
    

    char Crossbow[] = "Crossbow";
    char Skeleton[] = "Skeleton";
    char Imp[] = "Imp";
    char Zombie[] = "Zombie";
    char PossessedCow[] = "Possessed Cow";
    char ThornyDevil[] = "Thorny Devil";
    char FrilledNeck[] = "Frilled Neck";

    UnitTypes[0].UnitName = &Crossbow;
    UnitTypes[0].sprite = Units[0];
    UnitTypes[0].speed = 3;
    UnitTypes[0].footprintsize = 5;
    UnitTypes[0].height = 110;
    UnitTypes[0].maxhealth = 1000;
    UnitTypes[0].minrange = 0;
    UnitTypes[0].maxrange = 500;
    UnitTypes[0].minattack = 0;
    UnitTypes[0].maxattack = 10;
    UnitTypes[0].pausebefore = 100;
    UnitTypes[0].pauseafter = 100;
    UnitTypes[0].projectile = NULL;
    UnitTypes[0].projectilespeed = 0;
    UnitTypes[0].agrorange = 500;

    UnitTypes[1].UnitName = &Skeleton;
    UnitTypes[1].sprite = Units[1];
    UnitTypes[1].speed = 4;
    UnitTypes[1].footprintsize = 5;
    UnitTypes[1].height = 110;
    UnitTypes[1].maxhealth = 1000;
    UnitTypes[1].minrange = 0;
    UnitTypes[1].maxrange = 500;
    UnitTypes[1].minattack = 0;
    UnitTypes[1].maxattack = 10;
    UnitTypes[1].pausebefore = 100;
    UnitTypes[1].pauseafter = 100;
    UnitTypes[1].projectile = NULL;
    UnitTypes[1].projectilespeed = 0;
    UnitTypes[1].agrorange = 500;

    UnitTypes[2].UnitName = &Imp;
    UnitTypes[2].sprite = Units[2];
    UnitTypes[2].speed = 6;
    UnitTypes[2].footprintsize = 3;
    UnitTypes[2].height = 90;
    UnitTypes[2].maxhealth = 1000;
    UnitTypes[2].minrange = 0;
    UnitTypes[2].maxrange = 500;
    UnitTypes[2].minattack = 0;
    UnitTypes[2].maxattack = 10;
    UnitTypes[2].pausebefore = 100;
    UnitTypes[2].pauseafter = 100;
    UnitTypes[2].projectile = NULL;
    UnitTypes[2].projectilespeed = 0;
    UnitTypes[2].agrorange = 500;

    UnitTypes[3].UnitName = &Zombie;
    UnitTypes[3].sprite = Units[3];
    UnitTypes[3].speed = 12;
    UnitTypes[3].footprintsize = 5;
    UnitTypes[3].height = 110;
    UnitTypes[3].maxhealth = 1000;
    UnitTypes[3].minrange = 0;
    UnitTypes[3].maxrange = 500;
    UnitTypes[3].minattack = 0;
    UnitTypes[3].maxattack = 10;
    UnitTypes[3].pausebefore = 100;
    UnitTypes[3].pauseafter = 100;
    UnitTypes[3].projectile = NULL;
    UnitTypes[3].projectilespeed = 0;
    UnitTypes[3].agrorange = 500;

    UnitTypes[4].UnitName = &PossessedCow;
    UnitTypes[4].sprite = Units[4];
    UnitTypes[4].speed = 6;
    UnitTypes[4].footprintsize = 10;
    UnitTypes[4].height = 150;
    UnitTypes[4].maxhealth = 1000;
    UnitTypes[4].minrange = 0;
    UnitTypes[4].maxrange = 500;
    UnitTypes[4].minattack = 0;
    UnitTypes[4].maxattack = 10;
    UnitTypes[4].pausebefore = 100;
    UnitTypes[4].pauseafter = 100;
    UnitTypes[4].projectile = NULL;
    UnitTypes[4].projectilespeed = 0;
    UnitTypes[4].agrorange = 500;

    UnitTypes[5].UnitName = &ThornyDevil;
    UnitTypes[5].sprite = Units[5];
    UnitTypes[5].speed = 6;
    UnitTypes[5].footprintsize = 5;
    UnitTypes[5].height = 110;
    UnitTypes[5].maxhealth = 1000;
    UnitTypes[5].minrange = 0;
    UnitTypes[5].maxrange = 500;
    UnitTypes[5].minattack = 0;
    UnitTypes[5].maxattack = 10;
    UnitTypes[5].pausebefore = 100;
    UnitTypes[5].pauseafter = 100;
    UnitTypes[5].projectile = NULL;
    UnitTypes[5].projectilespeed = 0;
    UnitTypes[5].agrorange = 500;

    UnitTypes[6].UnitName = &FrilledNeck;
    UnitTypes[6].sprite = Units[6];
    UnitTypes[6].speed = 6;
    UnitTypes[6].footprintsize = 5;
    UnitTypes[6].height = 110;
    UnitTypes[6].maxhealth = 1000;
    UnitTypes[6].minrange = 0;
    UnitTypes[6].maxrange = 500;
    UnitTypes[6].minattack = 0;
    UnitTypes[6].maxattack = 10;
    UnitTypes[6].pausebefore = 100;
    UnitTypes[6].pauseafter = 100;
    UnitTypes[6].projectile = NULL;
    UnitTypes[6].projectilespeed = 0;
    UnitTypes[6].agrorange = 500;

    int i,j;
    for (i = 0; i < 10; i++ ) {
        for (j = 0; j < 10; j++ ) {
            int utype = rand() % 11;
            if (utype < 2) SpriteUnits[i + 10 * j].Type = &UnitTypes[1];        //Skeleton
            else if (utype < 6)   SpriteUnits[i + 10 * j].Type = &UnitTypes[2]; //Imp
            else if (utype < 8)   SpriteUnits[i + 10 * j].Type = &UnitTypes[3]; //Zombie
            else if (utype < 9)   SpriteUnits[i + 10 * j].Type = &UnitTypes[4]; //Cow
            else if (utype < 10)  SpriteUnits[i + 10 * j].Type = &UnitTypes[5]; //Devil
            else                  SpriteUnits[i + 10 * j].Type = &UnitTypes[6]; //Frilled
            SpriteUnits[i + 10 * j].Alive = 1;
            SpriteUnits[i + 10 * j].Health = SpriteUnits[i + 10 * j].Type->maxhealth;
            SpriteUnits[i + 10 * j].DisplayHealthUntil = 0;
            SpriteUnits[i + 10 * j].StartPoint.i = i * 200 + rand() % 200;
            SpriteUnits[i + 10 * j].StartPoint.j = j * 200 + rand() % 200;
            SpriteUnits[i + 10 * j].StartTime = 0;
            SpriteUnits[i + 10 * j].EndPoint.i = i * 200 + rand() % 200;
            SpriteUnits[i + 10 * j].EndPoint.j = j * 200 + rand() % 200;
            SpriteUnits[i + 10 * j].EndTime = 0;
            SpriteUnits[i + 10 * j].location.i = SpriteUnits[i + 10 * j].StartPoint.i;
            SpriteUnits[i + 10 * j].location.j = SpriteUnits[i + 10 * j].StartPoint.j;
            SpriteUnits[i + 10 * j].GoalPoint.i = SpriteUnits[i + 10 * j].EndPoint.i;
            SpriteUnits[i + 10 * j].GoalPoint.j = SpriteUnits[i + 10 * j].EndPoint.j;
            SpriteUnits[i + 10 * j].Forces.i = 0;
            SpriteUnits[i + 10 * j].Forces.j = 0;
            SpriteUnits[i + 10 * j].DirectionFacing = LEFTFACING;
            SpriteUnits[i + 10 * j].Next = NULL;
            SpriteUnits[i + 10 * j].Previous = NULL;
        }
    }

    SpriteUnits[0].Type = &UnitTypes[0];
    SpriteUnits[0].StartPoint.i = 500;
    SpriteUnits[0].StartPoint.j = 500;
    SpriteUnits[0].StartTime = 0;
    SpriteUnits[0].EndPoint.i = 500;
    SpriteUnits[0].EndPoint.j = 500;
    SpriteUnits[0].EndTime = 6;
    SpriteUnits[0].location.i = 500;
    SpriteUnits[0].location.j = 500;
    SpriteUnits[0].GoalPoint.i = 500;
    SpriteUnits[0].GoalPoint.j = 500;
    SpriteUnits[0].Forces.i = 0;
    SpriteUnits[0].Forces.j = 0;
    SpriteUnits[0].DirectionFacing = RIGHTFACING;
    SpriteUnits[0].Next = NULL;
    SpriteUnits[0].Previous = NULL;

    UnitSelected = &SpriteUnits[0];

}

    // ---- Plot Function(s) ---- //


int Plot_Circles (void) {
//plot circle at base of selected unit.
    /*if (UnitSelected != NULL && UnitSelected->Alive)*/ {
        //Plot Circle
        if (UnitSelected != NULL) {
            SpriteUnit* q = UnitSelected;
            XY_Point UnitLocation = IJtoXY (q->location);
            sfCircleShape* SelectedCircle = sfCircleShape_create();
            sfVector2f origin;
            origin.x = UnitLocation.x - (q->Type->footprintsize + 13);
            origin.y = UnitLocation.y - (q->Type->footprintsize + 13)/2;
            sfCircleShape_setPosition(SelectedCircle, origin);
            sfCircleShape_setRadius(SelectedCircle, q->Type->footprintsize + 13);
            sfVector2f scale;
            scale.x = 1;
            scale.y = 0.5;
            sfCircleShape_setScale(SelectedCircle, scale);

            sfCircleShape_setFillColor(SelectedCircle, sfTransparent);
            sfCircleShape_setOutlineColor(SelectedCircle, sfCyan);  //Blue or Cyan
            sfCircleShape_setOutlineThickness(SelectedCircle, 2.0); 
	
            sfRenderWindow_drawCircleShape(window, SelectedCircle, NULL);
            sfCircleShape_destroy(SelectedCircle);
        }
    }
}

//Globals used in plotting
SpriteObject* p;
SpriteUnit* q;
ProjectileInstance* r;
XY_Point ObjectLocation;
XY_Point UnitLocation;
XY_Point ProjectileLocation;
sfVector2f newposition;

int IncrementP (void) {
            newposition.x = ObjectLocation.x;
            newposition.y = ObjectLocation.y;
            sfSprite_setPosition (p->sprite, newposition);
            sfRenderWindow_drawSprite(window, p->sprite, NULL);
            p = p->Next;
            ObjectLocation = IJtoXY (p->location);
}

int IncrementQ (void) {
            newposition.x = UnitLocation.x;
            newposition.y = UnitLocation.y;
            if (q->DirectionFacing == LEFTFACING) {
                sfSprite_setScale (q->Type->sprite, LeftScale);
            } else {
                sfSprite_setScale (q->Type->sprite, RightScale);
            }
            sfSprite_setPosition (q->Type->sprite, newposition);
            sfRenderWindow_drawSprite(window, q->Type->sprite, NULL);

            q = q->Next;
            UnitLocation = IJtoXY (q->location);
}

int IncrementR (void) {
            newposition.x = ProjectileLocation.x;
            newposition.y = ProjectileLocation.y;
            if (r->DirectionFacing == LEFTFACING) {
                sfSprite_setScale (r->Type->Facet[r->FacetNumber], LeftScale);
            } else {
                sfSprite_setScale (r->Type->Facet[r->FacetNumber], RightScale);
            }
            sfSprite_setPosition (r->Type->Facet[r->FacetNumber], newposition);
            sfRenderWindow_drawSprite(window, r->Type->Facet[r->FacetNumber], NULL);
            
            r = r->SortedNext;
            ProjectileLocation = IJtoXY (r->Location);
}



int Plot_Units_Objects_Projectiles (void) {

    p = Head.Next;
    q = UnitsHead.Next;
    r = PIHead.Next;
    ObjectLocation = IJtoXY (p->location);
    UnitLocation = IJtoXY (q->location);
    ProjectileLocation = IJtoXY (r->Location);

    //Draw Terrain (diferentiate from terrain objects like rocks and tufts of grass)
/*


if p (
    if q(
        if r (
	    if p < q (
                if p < r (
                     p++
                ) else //r < q (
                     r++
                )
             ) else //q < p (
                 if q < r (
                     q++
                 ) else // r < q (
                     r++
                 )
             )
        ) else p, q, !r (
             if p < q (
                 p++
             ) else q < p (
                 q++
             )
        )
    ) else p, !q (
        if r (
             if p < r (
                 p++
             ) else r < p (
                 r++
             )
        ) else p, !q, !r (
             p++
        )
    )
) else !p (
    if q (
        if r (
            if q < r (
                q++
            ) else r < q (
                r++
            )
        ) else !p, q, !r (
            q++
        )
    ) else !p, !q
        if r (
            r++
        ) else (
            return
        )
    )
)

*/
while (1) {
    if (p != &Tail) {
        if (q != &UnitsTail) {
            if (r != &PITail)  {
	        if (ObjectLocation.y < UnitLocation.y) {
                    if (ObjectLocation.y < ProjectileLocation.y) {
                         IncrementP();
                    } else {
                         IncrementR();
                    }
                 } else {
                     if (UnitLocation.y < ProjectileLocation.y) {
                         IncrementQ();
                     } else  {
                         IncrementR();
                     }
                 }    
            } else {
                 if (ObjectLocation.y < UnitLocation.y) {
                     IncrementP();
                 } else {
                     IncrementQ();
                 }
            }
        } else {
            if (r != &PITail) {
                 if (ObjectLocation.y < ProjectileLocation.y) {
                     IncrementP();
                 } else {
                     IncrementR();
                 }
            } else {
                 IncrementP();
            }
        }
    } else {
        if (q != &UnitsTail) {
            if (r != &PITail) {
                if (UnitLocation.y < ProjectileLocation.y) {
                    IncrementQ();
                } else {
                    IncrementR();
                }
            } else {
                IncrementQ();
            }
        } else
            if (r != &PITail) {
                IncrementR();
            } else {
                return;
            }
        }
    }
}



int Plot_Health_Bars (void) {

    sfVertex points[4];


    points[0].color = sfRed;
    points[1].color = sfRed;
    points[2].color = sfRed;
    points[3].color = sfRed;

    //plot health bars, red part:
    SpriteUnit* q = UnitsHead.Next;

    while (q != &UnitsTail) {
                                                                //If Alive
        if (q->DisplayHealthUntil > NowTime) {
             //draw health bar
             XY_Point Location = IJtoXY (q->location);
             int Left = Location.x - HEALTHBARWIDTH;
             int Top = Location.y - q->Type->height;
             int Right = Location.x + HEALTHBARWIDTH;
             int Bottom = Location.y - q->Type->height - HEALTHBARHEIGHT;

             points[0].position.x = Left;
             points[0].position.y = Top;
             points[1].position.x = Right;
             points[1].position.y = Top;
             points[2].position.x = Left;
             points[2].position.y = Bottom;
             points[3].position.x = Right;
             points[3].position.y = Bottom;

             sfRenderWindow_drawPrimitives 	( 	window,
		                                        &points[0],
		                                        4,
		                                        sfTrianglesStrip,
		                                        NULL//const sfRenderStates *  	states 
	                                        );


        }
        q = q->Next;
    }


    points[0].color = sfGreen;
    points[1].color = sfGreen;
    points[2].color = sfGreen;
    points[3].color = sfGreen;


    //plot health bars, green part:
    q = UnitsHead.Next;
    while (q != &UnitsTail) {
        if (q->DisplayHealthUntil > NowTime) {
                                                                //If Alive
             //draw health bar
             XY_Point Location = IJtoXY (q->location);
             int Left = Location.x - HEALTHBARWIDTH;
             int Top = Location.y - q->Type->height;
             int Right = Location.x - HEALTHBARWIDTH + 2 * HEALTHBARWIDTH * q->Health / q->Type->maxhealth;
             int Bottom = Location.y - q->Type->height - HEALTHBARHEIGHT;

             points[0].position.x = Left;
             points[0].position.y = Top;
             points[1].position.x = Right;
             points[1].position.y = Top;
             points[2].position.x = Left;
             points[2].position.y = Bottom;
             points[3].position.x = Right;
             points[3].position.y = Bottom;

             sfRenderWindow_drawPrimitives 	( 	window,
		                                        &points[0],
		                                        4,
		                                        sfTrianglesStrip,
		                                        NULL//const sfRenderStates *  	states 
	                                        );

        }
        q = q->Next;
    }

}





    // ---- Other Functions ---- //



int TargetPlayer(void) {
    SpriteUnit* p = UnitsHead.Next;
    
    while (p != &UnitsTail) {
        if (p != &SpriteUnits[0]) {
            p->GoalPoint = SpriteUnits[0].location;
        }
        p = p->Next;
    }
    
}

int CalcForces(void) {
    int subject, object, sqdist, idist, jdist, dist, footprint, foo;
    for (subject = 0; subject < 100; subject++) {
        if(SpriteUnits[subject].Alive){                                                        //If Alive
            SpriteUnits[subject].Forces.i = 0;
            SpriteUnits[subject].Forces.j = 0;
            for (object = 1; object < 100; object++) {
                if (SpriteUnits[object].Alive){                                                //If Alive
                    if (&SpriteUnits[subject] != &SpriteUnits[object]) {
                        idist = SpriteUnits[subject].location.i - SpriteUnits[object].location.i;
                        jdist = SpriteUnits[subject].location.j - SpriteUnits[object].location.j;
                        sqdist = idist*idist + jdist*jdist;
                        footprint = SpriteUnits[subject].Type->footprintsize + SpriteUnits[object].Type->footprintsize;
                        dist = sqrt(sqdist);
                    
                        if (dist > footprint + CRITICALDISTANCE) {
                    
                            SpriteUnits[subject].Forces.i += (10000*idist)/dist/(dist - footprint)/(dist - footprint);
                            SpriteUnits[subject].Forces.j += (10000*jdist)/dist/(dist - footprint)/(dist - footprint);
                        } else if (dist > 0){
                            SpriteUnits[subject].Forces.i += (10000*idist)/dist/CRITICALDISTANCE/CRITICALDISTANCE;
                            SpriteUnits[subject].Forces.j += (10000*jdist)/dist/CRITICALDISTANCE/CRITICALDISTANCE;
                        }
                    }
                }
            }
            //clip forces
            /*sqdist = SpriteUnits[subject].Forces.i*SpriteUnits[subject].Forces.i + SpriteUnits[subject].Forces.j*SpriteUnits[subject].Forces.j;
            dist = sqrt (sqdist);
            foo = dist;
            SpriteUnits[subject].Forces.i = SpriteUnits[subject].Forces.i * foo / dist;
            SpriteUnits[subject].Forces.j = SpriteUnits[subject].Forces.j * foo / dist;
            */
        }
    }
}

int CalcPath(void){
    int sqdist, idist, jdist, dist, subject, timetogo;
    /*subject = 0;
    SpriteUnits[subject].StartTime = NowTime;
    SpriteUnits[subject].StartPoint = SpriteUnits[subject].location;
    idist = SpriteUnits[subject].GoalPoint.i - SpriteUnits[subject].location.i;
    jdist = SpriteUnits[subject].GoalPoint.j - SpriteUnits[subject].location.j;
    sqdist = idist*idist + jdist*jdist;
    if ((idist - jdist) > 0){
        SpriteUnits[subject].DirectionFacing = RIGHTFACING;
    } else if ((idist - jdist) < 0){
        SpriteUnits[subject].DirectionFacing = LEFTFACING;
    }
    if (sqdist > 10000) {
        dist = sqrt(sqdist);
        SpriteUnits[subject].EndPoint.i = SpriteUnits[subject].StartPoint.i + (100 * idist) / dist;
        SpriteUnits[subject].EndPoint.j = SpriteUnits[subject].StartPoint.j + (100 * jdist) / dist;
        SpriteUnits[subject].EndTime = NowTime + 100 * SpriteUnits[subject].Type->speed;
    } else if (sqdist != 0) {
        SpriteUnits[subject].EndPoint.i = SpriteUnits[subject].StartPoint.i + idist;
        SpriteUnits[subject].EndPoint.j = SpriteUnits[subject].StartPoint.j + jdist;
        SpriteUnits[subject].EndTime = NowTime + 100 * SpriteUnits[subject].Type->speed;
    } else {
        SpriteUnits[subject].EndPoint.i = SpriteUnits[subject].GoalPoint.i;
        SpriteUnits[subject].EndPoint.j = SpriteUnits[subject].GoalPoint.j;
        SpriteUnits[subject].EndTime = NowTime + 100 * SpriteUnits[subject].Type->speed;
    }*/
    for (subject = 0/* or 1*/; subject < 100; subject++){
        if (SpriteUnits[subject].Alive){                                                        //If Alive
            SpriteUnits[subject].StartTime = NowTime;
            SpriteUnits[subject].StartPoint = SpriteUnits[subject].location;
            idist = SpriteUnits[subject].GoalPoint.i - SpriteUnits[subject].location.i;
            jdist = SpriteUnits[subject].GoalPoint.j - SpriteUnits[subject].location.j;
            sqdist = idist*idist + jdist*jdist;
            if ((idist - jdist) > 0){
                SpriteUnits[subject].DirectionFacing = RIGHTFACING;
            } else if ((idist - jdist) < 0){
                SpriteUnits[subject].DirectionFacing = LEFTFACING;
            }
            if (sqdist > 10000) {
                dist = sqrt(sqdist);
                SpriteUnits[subject].EndPoint.i = SpriteUnits[subject].StartPoint.i + (100 * idist) / dist + SpriteUnits[subject].Forces.i; //effect of forces must be bounded
                SpriteUnits[subject].EndPoint.j = SpriteUnits[subject].StartPoint.j + (100 * jdist) / dist + SpriteUnits[subject].Forces.j;
                SpriteUnits[subject].EndTime = NowTime + 100 * SpriteUnits[subject].Type->speed;
            } else if (sqdist != 0 ){
                dist = sqrt(sqdist);
                SpriteUnits[subject].EndPoint.i = SpriteUnits[subject].StartPoint.i + idist + SpriteUnits[subject].Forces.i;
                SpriteUnits[subject].EndPoint.j = SpriteUnits[subject].StartPoint.j + jdist + SpriteUnits[subject].Forces.j;
                SpriteUnits[subject].EndTime = NowTime + 100 * SpriteUnits[subject].Type->speed;
            } else {
                SpriteUnits[subject].EndPoint.i = SpriteUnits[subject].GoalPoint.i + SpriteUnits[subject].Forces.i;
                SpriteUnits[subject].EndPoint.j = SpriteUnits[subject].GoalPoint.j + SpriteUnits[subject].Forces.j;
                SpriteUnits[subject].EndTime = NowTime + 100 * SpriteUnits[subject].Type->speed;
            }
        }
    }
}  

int DamageUnit(SpriteUnit *Target, int Damage){
    if (Damage >= Target->Health) {
        Target->Health = 0;
        Target->Alive = 0;
    } else {
        Target->Health -= Damage;
        Target->DisplayHealthUntil = NowTime + 5000;
    }
}

int ClearVisibleUnits(void) {
    UnitsHead.Next = &UnitsTail; //Careful not to cause bug in using SortedInsert
    UnitsTail.Previous = &UnitsHead;
    // Head -> Tail, Tail -> Head
    return 5;
}

//increment through linked list and insert in the correct position
int SortedUnitInsert(SpriteUnit *ToBeInserted) {
    
    SpriteUnit* p = UnitsHead.Next;
    
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

int ListVisibleUnits(void) {
    //Generates linked list of visible units
    //Loop through unit locations.
    int i;
    for (i = 0; i < 100; i++) {
        if (SpriteUnits[i].Alive){                                                       //If Alive
        //If within VIEW_RADIUS
            if ((SpriteUnits[i].location.i - ViewPoint.i) * (SpriteUnits[i].location.i - ViewPoint.i)
                
                + (SpriteUnits[i].location.j - ViewPoint.j) * (SpriteUnits[i].location.j - ViewPoint.j)
            
                < VIEW_RADIUS * VIEW_RADIUS) {
            
                SortedUnitInsert(&SpriteUnits[i]);
            }
        }
    }
    return 5;
}

int UpdateUnitsLocation(void){
    
    int UnitNumber;
    for (UnitNumber = 0; UnitNumber < 100; UnitNumber++) {
        if (SpriteUnits[UnitNumber].Alive) {                                                        //If Alive
            if (NowTime < SpriteUnits[UnitNumber].EndTime) {
                int DeltaTime = SpriteUnits[UnitNumber].EndTime - SpriteUnits[UnitNumber].StartTime;

                SpriteUnits[UnitNumber].location.i = (SpriteUnits[UnitNumber].StartPoint.i * (SpriteUnits[UnitNumber].EndTime - NowTime) + SpriteUnits[UnitNumber].EndPoint.i * ( NowTime - SpriteUnits[UnitNumber].StartTime )) / DeltaTime;
                SpriteUnits[UnitNumber].location.j = (SpriteUnits[UnitNumber].StartPoint.j * (SpriteUnits[UnitNumber].EndTime - NowTime) + SpriteUnits[UnitNumber].EndPoint.j * ( NowTime - SpriteUnits[UnitNumber].StartTime )) / DeltaTime;
            
            } else {
                SpriteUnits[UnitNumber].location.i = SpriteUnits[UnitNumber].EndPoint.i;
                SpriteUnits[UnitNumber].location.j = SpriteUnits[UnitNumber].EndPoint.j;
            }
        }
    }
}

    // ---- Release Function ---- //

int Release_Units(void) {

}
