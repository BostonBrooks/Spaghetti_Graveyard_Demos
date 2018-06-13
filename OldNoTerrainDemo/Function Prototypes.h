XY_Point IJtoXY ( IJ_Point a );
// Take IJ coordinates and return XY coordinates

IJ_Point XYtoIJ ( XY_Point b );
// Take XY coordiates and return IJ coordinates


int InitTime(void);
//Sets game time to 0

int GetTime(void);
//Returns game time in ms

int PauseTime(void);
//Returns game time in ms, stops time passing

int UnpauseTime(void);
//Returns game time in ms, starts time passing


//increment through linked list and insert in the correct position
int SortedInsert(SpriteObject *ToBeInserted);
//loop forwards from Head until the next item is closer to the screen than ToBeInserted, insert Subject before next item. ensure Tail is always closer to screen than ToBeInserted


int SortedRemove(SpriteObject *ToBeRemoved);
//remove object from list of visible sprite objects and fill the gap
//What if we delete the first element of the list?
//// Need to take a sanity check in case ToBeRemoved is not in visible list.


int SortedClear(void);
//Modify Head and Tail to point to each other, so no objects appear in the list between them


int SortedResort(SpriteObject *Head);
//Best if say, 20% of items are out of order
//Sort objects that may have moved in relation to the other objects, no use when working with stationary objects. Will need to block access to list while sorting.

//take head and tail
//create list of rejected elements
//if empty return null
//pass new head and tail to SortedResort
//splice items into list

/*
An obvious strategy of reasonable practical efficiency (he claims, hand-wavingly) would be to filter out the decreasing elements as they're encountered leaving a non-decreasing list NN. Sort the filtered elements (using the same method since they're still roughly ascending) and merge back into NN. In your example you'd filter out the parenthesised elements:

1 5 (4) 8 (5) 8 9 9 9 12 12 16 18 17 20 (15) (19) (18) 20 21 23 (21) 27 (25) 26 30 31 33 (29) 34 35 (31) 35 38 37 37 37 37 40 41 41 45 47 (43) 50 (48) 52 (49) (50)

leaving

1 5 8 8 9 9 9 12 12 16 18 17 20 20 21 23 27 26 30 31 33 34 35 35 38 37 37 37 37 40 41 41 45 47 50 52

and the filtered elements in the order:

4 5 15 [19] 18 21 25 29 31 43 48 49 50

with only one element out of order in the filtered elements.

The filtering and merging operations are O(n)O(n). If a constant fraction rr of elements were filtered at each level, the complexity is roughly n(1+r+r2+⋯)n(1+r+r2+⋯) which is O(n)O(n)-ish (he said, hedging his bets because of the rustiness of his complexity theory).
*/

int SortedResort2(SpriteObject *Head);
//Best if only a few items are out of order.
//Will need to block access to list while sorting.
//Look through list, if one element is is less than the previous element, shift left until this is no longer the case.

int SortedResort3(SpriteObject *Head);
//Optimised for completely unordered lists
//Will need to block access to list while sorting.


int ListVisible (void);

//Search through list of all objects, if an object appears within view radius, SortedInsert into list of visible objects.

//Sanity check: first use SortedClear, or else see what happens when you SortedInsert something already in the list.

//Is it better to use SortedInsert or just normal insert then run SortedResort. May be better to use a different sorting algorithm because the list might not have the property of being weakly sorted.

int Plot(void);
//interleave lists of grass types, intercative items etc and plot to screen

int TargetPlayer(void);
//for all units within the view radius (except the player), set the target of the unit to the location of the player
int CalcForces(void);

int CalcPath(void);

int UpdateUnitsLocation(void);
//Extends interpolation between player's startpoint and endpoint to all units

int ClearVisibleUnits(void);

int ListVisibleUnits(void);
//Generates linked list of visible units


int TargetPlayer(void);
//Tells units to chase player

int SortedUnitInsert(SpriteUnit *ToBeInserted);

int UpdateUnitsLocation(void);

void Input_fn(void* arg);
