    // ---- Data Types ---- //


    // ---- Globals ---- //


    // ---- Init Function ---- //

int Init_Player() {

}

    // ---- Other Functions ---- //

int UpdateViewPoint (void) {
    ViewPoint = SpriteUnits[0].location;
}

int GetMouse (void) {
    if (sfMouse_isButtonPressed (sfMouseRight)) {
        sfVector2i mousestate;
        XY_Point xymousestate;
        mousestate = sfMouse_getPositionRenderWindow (window);
        xymousestate.x = mousestate.x;
        xymousestate.y = mousestate.y;
        IJ_Point mousegoto = XYtoIJ (xymousestate);

        SpriteUnits[0].StartTime = NowTime;
        SpriteUnits[0].StartPoint = SpriteUnits[0].location;
        SpriteUnits[0].GoalPoint = mousegoto;
    }
}

int Select(sfEvent click) {
    SpriteUnit* q = UnitsHead.Next;
    SpriteUnit* closest = UnitsHead.Next;

    XY_Point UnitPos;
    int mindistsq = 1000000;


    while (q != &UnitsTail) {
        if (q != &SpriteUnits[0]) {
            if (q->Alive) {
                UnitPos = IJtoXY (q->location);
                UnitPos.y -= 45;
                int distsq = (UnitPos.x - click.mouseButton.x) * (UnitPos.x - click.mouseButton.x) + (UnitPos.y - click.mouseButton.y) * (UnitPos.y - click.mouseButton.y);
                if (distsq < mindistsq) {
                    mindistsq = distsq;
                    closest = q;
                }
            }
        }
        q = q->Next;
    }

    if (mindistsq < 45 * 45){
        UnitSelected = closest;
    }

}

int EnterSelect(void) {
    //If you press enter and no enemy selected, select closest unit except unit 0;

    SpriteUnit* q = UnitsHead.Next;
    SpriteUnit* closest = UnitsHead.Next;

    XY_Point UnitPos;
    int mindistsq = 1000000;

    while (q != &UnitsTail) {
        if (q != &SpriteUnits[0]) {
            if (q->Alive) {
                int distsq = (q->location.i - SpriteUnits[0].location.i) * (q->location.i - SpriteUnits[0].location.i) + (q->location.j - SpriteUnits[0].location.j) * (q->location.j - SpriteUnits[0].location.j);
                if (distsq < mindistsq) {
                    mindistsq = distsq;
                    closest = q;
                }
            }
        }
        q = q->Next;
    }

    if (mindistsq < 160000){
        UnitSelected = closest;
        return 1;
    }
    return 0;

}

int CheckSelection(void) {
    //If the selected unit is not alive or is out of range, set unitselected to null.
    if (UnitSelected != NULL) {
        if (!UnitSelected->Alive ||                                                       //If not Alive
                        //If within VIEW_RADIUS
           ((UnitSelected->location.i - ViewPoint.i) * (UnitSelected->location.i - ViewPoint.i)
           + (UnitSelected->location.j - ViewPoint.j) * (UnitSelected->location.j - ViewPoint.j))
           >= VIEW_RADIUS * VIEW_RADIUS) {
               UnitSelected = NULL;
        }
    }

}

int GetKey(void) {
    sfEvent InputEvent;

    while (sfRenderWindow_pollEvent(window, &InputEvent) == sfTrue) {
        switch (InputEvent.type) {
            case sfEvtKeyPressed:
                switch(InputEvent.key.code) {
                    case sfKeySubtract:
                        printf("-");
                        break;        
                    case sfKeyAdd:
                        printf("+");
                        break;                
                    case sfKeyPeriod:
                        printf(".");
                        break;          
                    case sfKeyNum0:
                    case sfKeyNumpad0:
                        printf("0");
                        input_num(0);
                        break;                
                    case sfKeyNum1:
                    case sfKeyNumpad1:
                        printf("1");
                        input_num(1);
                        break;
                    case sfKeyNum2:
                    case sfKeyNumpad2:
                        printf("2");
                        input_num(2);
                        break;
                    case sfKeyNum3:
                    case sfKeyNumpad3:
                        printf("3");
                        input_num(3);
                        break;
                    case sfKeyNum4:
                    case sfKeyNumpad4:
                        printf("4");
                        input_num(4);
                        break;
                    case sfKeyNum5:
                    case sfKeyNumpad5:
                        printf("5");
                        input_num(5);
                        break;
                    case sfKeyNum6:
                    case sfKeyNumpad6:
                        printf("6");
                        input_num(6);
                        break;
                    case sfKeyNum7:
                    case sfKeyNumpad7:
                        printf("7");
                        input_num(7);
                        break;
                    case sfKeyNum8:
                    case sfKeyNumpad8:
                        printf("8");
                        input_num(8);
                        break;
                    case sfKeyNum9:
                    case sfKeyNumpad9:
                        printf("9");
                        input_num(9);
                        break;
                    case sfKeyReturn:
                        if (UnitSelected != NULL) {                          //This is a test
                            DamageUnit(UnitSelected, 100);
                        } else {
                            if (EnterSelect()) {
                                 DamageUnit(UnitSelected, 100);
                            }
                        }
                        input_enter();
                        break;
                    default: {}
                }
                case sfEvtMouseButtonPressed:
                    if (InputEvent.mouseButton.button == sfMouseLeft) Select(InputEvent);
                break;
                case sfEvtMouseButtonReleased:
                break;
                case sfEvtMouseMoved:
                break;
            default: {}
        }
    }
}
    // ---- Release Function ---- //


int Release_Player() {

}
