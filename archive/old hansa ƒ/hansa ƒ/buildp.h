/* this is the .h file for the private part of the build module			*/#pragma once#include <stdlib.h>#include <math.h>#include "city.h"#include "cityNumberObject.h"#include "file.h"#include "map.h"#include "transport.h"#include "utilities.h"#include "hansaWindows.h" /* probabilities of various formations occurring.											*//* 																							*//* Land/ocean probabilities:																*//*																							*//* P1 starter hex is land																	*//* P2 outside edge switching from land/water to water land									*//* P3 outside edge remains the same.														*//* P4 land/water boundary grows a land inward.												*//* P5 water/water boundary switches to land													*//* P6 water/land/land/water grows a land													*//* P7 water/land/land/any grows a land														*//* P8 land/land grows a land																*//* P9 land starts a mountain range															*//* 																							*//* Mountian probabilities:																	*//* Note: if target mountain hexes are next to ocean lake, probabilities are cut in 4.		*//*																							*//* P10 mountain end with no neighbors grows a mountain										*//* P11 mountain end grows a mountain next to a mountain	adjecent to end						*//* P12 mountain end grows a mountain 1 away from mountain adjacent to end					*//* P13 mountain end grows a mountain 2 away from mountain adjacent to end					*//*																							*//* Forest Probabilities:																	*//*																							*//* P14 land starts a forest hex																*//* P15 forest spreads outward 1 hex ring													*//* P16 forest spreads out 2 hex rings														*//* P17 forest at edge randomly shoots out another forest on clear edge.						*//* 																							*//* River probabilities:																		*//*																							*//* P18 source of a river starts																*//* P19 river exits opposite side it entered													*//* P20 city founded on river site																*//* P21 city founded on coastal site										*/#define P1	500#define P2	003#define P3	996#define P4 	700#define P5 	200#define P6	600#define P7 	700#define P8 	980#define P9ocean 10#define P9land	40#define P10 80#define P11	20#define P12	30#define P13	60#define P14	80#define P15	500#define P16 250#define P17 100#define P18 80#define P19 800#define P20 300#define P21 100#define isRiverJoin(row, column, dir) (NumberOfBits(GetRiverType(row, column, dir)) > 3)#define isRiverHead(_row, _col, _dir) (NumberOfBits(GetRiverType((_row), (_col), (_dir))) == 1)static unsigned compassToRiverOut[numberOfDirections] = {	0, northOut, northeastOut, southeastOut, southOut, southwestOut, northwestOut};static unsigned compassToRiverIn[numberOfDirections] = {	0, northIn, northeastIn, southeastIn, southIn, southwestIn, northwestIn};void createForests(void);void createMountains(void);void decideTerrain(short row, short column);void digRivers(void);void drawBox(short box);void foundCities(void);short getClockDistance(short row, short column, enum direction dir, enum terrainType type);void growForest(short row,short column,short more);void growMountain(short row,short column);Boolean growRiver(short row,short column,enum direction dir,enum direction mustGo);void raiseContinents(void); 