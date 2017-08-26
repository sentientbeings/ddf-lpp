/* this is the .c file dealing with world building.						*//*																		*//* Modification History:												*//*																		*//* 01 Feb 1990 tomc	creation. Split from terrain.c. Untangled from		*//*					mapmaker.											*/#include <Events.h>#include <Windows.h>#include "build.h"#include "buildp.h"/* public procedures *//* if this is a random world, go forth and build the map. Else, give us	*//* a blank map.															*/void BuildWorld(){	DoWaitWindow(raiseContinentPhase);	raiseContinents();		DoWaitWindow(createMountainPhase);	createMountains();	DoWaitWindow(createForestPhase);	createForests();	digRivers(); 	DoWaitWindow(foundCityPhase);	foundCities(); 	DoWaitWindow(finalPhase);}/* local procedures *//* populate the world with forests */void createForests(){	register short row, column;	/* first seed the map with the starts of forests */	row = rowSize;	while (--row >= 0) {		column = columnSize;		while (--column >= 0)			if ((GetTerrainType(row, column, same) == clear) && (test(P14)))				SetTerrainType(row, column, same, forestSeed);	}				/* next, grow the seeds into forests */	row = rowSize;	while (--row >= 0) {		column = columnSize;		while (--column >= 0)			if (GetTerrainType(row, column, same) == forestSeed) {				SetTerrainType(row, column, same, forest);				if (test(P15))					growForest(row, column, 1);				else if (test(P16))					growForest(row, column, 2);				else growForest(row, column, 0);			}	}}void createMountains(){	register short row, column, numberOfSeeds;	struct mountainData {		short h;		short v;	} *mountainSeeds;	/* first seed the map with starts of mountain ranges */		mountainSeeds = (struct mountainData *) NewPtr(sizeof(struct mountainData) * 500);	numberOfSeeds = 0;	row = rowSize;	while (--row >= 0) {		column = columnSize;		while (--column >= 0)			if ((GetTerrainType(row, column, same) == clear) &&				(IsAdjacentTerrain(row, column, mountainSeed) == 0)) {				if (test((IsAdjacentTerrain(row, column, ocean) > 0) ? P9ocean : P9land)) {					SetTerrainType(row, column, same, mountainSeed);					mountainSeeds[numberOfSeeds].h = row;					mountainSeeds[numberOfSeeds].v = column;					numberOfSeeds++;				}			}	}			/* then grow the mountain ranges from the seed */		while (--numberOfSeeds >= 0) {		SetTerrainType(mountainSeeds[numberOfSeeds].h, mountainSeeds[numberOfSeeds].v, same, mountain);		growMountain(mountainSeeds[numberOfSeeds].h, mountainSeeds[numberOfSeeds].v);	}		DisposPtr((char *) mountainSeeds);}void digRivers(){	short row, column;	enum terrainType thisTerrain;	row = rowSize-1;	while (--row >= 1) {		column = columnSize-1;		while (-- column >= 1)			if (((thisTerrain = GetTerrainType(row, column, same)) != ocean) &&				(thisTerrain != mountain) &&				(IsAdjacentTerrain(row, column, mountain) > 0) &&				((IsAdjacentTerrain(row, column, clear) > 0) || (IsAdjacentTerrain(row, column, forest) > 0)) &&				(IsAdjacentTerrain(row, column, ocean) == 0) &&				(GetRiverType(row, column, same) == 0) &&				(test(P18))) 				growRiver(row, column, same, same);	}}void foundCities(){	register short row, column;	enum terrainType t;	int i;	unsigned val;		i = 0;	InitCities();	while (true) {		row = Randomize((long) rowSize);		column = Randomize((long) columnSize);		val = GetRiverType(row, column, same);		if (((((IsAdjacentTerrain(row, column, ocean) > 0) && (GetRiverType(row, column, same) != 0)) ||				(isRiverHead(row, column, same) == true) || isRiverJoin(row, column, same)) &&				(IsCityHere(row, column, same) == false) && (IsAdjacentCity(row, column) == false) && (test(P20))) ||			((IsAdjacentTerrain(row, column, ocean) > 0) && (((t = GetTerrainType(row, column, same)) == clear) || (t == forest)) &&			(IsAdjacentCity(row, column) == false) && (IsCityHere(row, column, same) == false) && (test(P21)))) {			CreateCity(row, column);			if (GetNumberOfCities() == maxNumberOfCities)				return;		}	}}/* Is there a hex of around a base hex of a given type? If so, how far is it around the		*//* clock from a target hex? */short getClockDistance(short row, short column, enum direction dir, enum terrainType type){	enum direction clockwise, widdershins;		if ((IsAdjacentTerrain(row, column, type) == 0) || (dir == same))		return -1;	if ((GetTerrainType(row, column, (clockwise = GetClockwise(dir))) == type) ||		(GetTerrainType(row, column, (widdershins = GetWiddershins(dir))) == type))		return 0;	else if ((GetTerrainType(row, column, GetClockwise(clockwise)) == type) ||		(GetTerrainType(row, column, GetWiddershins(widdershins)) == type))		return 1;	else return 2;}void growForest(short row, short column, short more){	register enum direction thisDirection;	Point t;		if (GetTerrainType(row, column, same) == clear)		SetTerrainType(row, column, same, forest);	if (more > 0) {		thisDirection = northwestPlus;		while (--thisDirection >= north) {			t = GetHexCoordinates(row, column, thisDirection);			growForest(t.h, t.v, more-1);		}	}}void growMountain(short row, short column){	register enum direction testDirection;	register short probability, d;	register Boolean builtMountain[numberOfDirections];	Point t;		testDirection = northwestPlus;	while (--testDirection >= north)		builtMountain[testDirection] = false;		testDirection = northwestPlus;	while (--testDirection >= north)		if (GetTerrainType(row, column, testDirection) == clear) {			if ((IsAdjacentTerrain(row, column, mountain) == 0) &&				(IsAdjacentTerrain(row, column, mountainSeed) == 0))				probability = P10;			else if ((d = min(getClockDistance(row, column, testDirection, mountain), getClockDistance(row, column, testDirection, mountainSeed))) == 0)				probability = P11;			else if (d == 1)				probability = P12;			else if (d == 2)				probability = P13;			t = GetHexCoordinates(row, column, testDirection);			probability *= (IsAdjacentTerrain(t.h, t.v, ocean) > 0) ? 1 : 4;			if (test(probability)) {				SetTerrainType(row, column, testDirection, mountain);				builtMountain[testDirection] = true;			}		}		testDirection = northwestPlus;	while (--testDirection >= north)		if (builtMountain[testDirection] == true) {			t = GetHexCoordinates(row, column, testDirection);			growMountain(t.h, t.v);		}}/* given a direction, grow a river towards the sea. This uses a recursive procedure to do this*//* We try to grow stuff downstream around each of the clock directions. If one suceeds, we *//* return true, and set the river in this hex going that way. If none succeed, we report false*/Boolean growRiver(short row, short column, enum direction from, enum direction mustGo){	enum direction testDirection, choice[3], x;	Point t;	enum terrainType o;	int i;		if (from == same) { /* start of river */		testDirection = north;		do			if ((GetTerrainType(row, column, GetOppositeDirection(testDirection)) == mountain) &&				(((o = GetTerrainType(row, column, testDirection)) == clear) || (o == forest))){				t = GetHexCoordinates(row, column, testDirection);				if (growRiver(t.h, t.v, GetOppositeDirection(testDirection), testDirection) == true) {					SetRiverType(row, column, same, compassToRiverOut[testDirection]);					return true;					break;				}			}		while ((testDirection = GetClockwise(testDirection)) != north);		return false;	} else  {		if (GetRiverType(row, column, same) != 0) { /* this is the river join case */			if ((isRiverHead(row, column, same) == true) || 				isRiverJoin(row, column, same))				return false;			else {				SetRiverType(row, column, same, GetRiverType(row, column, same) | compassToRiverIn[from]);				return true;			}		}		if (test(P19)) {			choice[0] = GetOppositeDirection(from);			choice[1] = GetWiddershins(GetOppositeDirection(from));			choice[2] = GetClockwise(GetOppositeDirection(from));		} else {			choice[0] = GetWiddershins(GetOppositeDirection(from));			choice[1] = GetClockwise(GetOppositeDirection(from));			choice[2] = GetOppositeDirection(from);		}		if (GetOppositeDirection(from) != mustGo) {			i = 3;			while (-- i >= 0) 				if (choice[i] != GetOppositeDirection(from))					choice[i] = mustGo;		}		if (IsAdjacentTerrain(row, column, ocean) > 0)			if (GetTerrainType(row, column, mustGo) == ocean)				choice[0] = mustGo;			else {				choice[0] = GetWiddershins(mustGo);				while (GetTerrainType(row, column, choice[0]) != ocean)					choice[0] = GetClockwise(choice[0]);			}		for (i = 0; i < 3; i++) {			x = choice[i];			if (((o = GetTerrainType(row, column, choice[i])) == ocean) || (o == noTerrainType)) {				SetRiverType(row, column, same, compassToRiverIn[from] | compassToRiverOut[choice[i]]);				return true;			}			if ((o != mountain)  && 				(isRiverHead(row, column, choice[i]) == false) &&				!isRiverJoin(row, column, choice[i])) {				t = GetHexCoordinates(row, column, choice[i]);				if (growRiver(t.h, t.v, GetOppositeDirection(choice[i]), mustGo) == true) {					SetRiverType(row, column, same, compassToRiverIn[from] | compassToRiverOut[choice[i]]);					return true;				}			}		}		return false;	}}/* build the land masses */void raiseContinents(void){	short row, column, box;		for (row = 0; row < rowSize; row++)		for (column = 0; column < columnSize; column++) {			SetTerrainType(row, column, same, noTerrainType);			SetRiverType(row, column, same, 0);		}	for (box = 0; box <= min(rowSize, columnSize) / 2; box++)		drawBox(box); }void decideTerrain(short row, short column){	enum terrainType currentType;	switch (IsAdjacentTerrain(row, column, noTerrainType)) {				case 6: /* no surrounding hex is assigned land/water */		SetTerrainType(row, column, same, (test(P1)) ? clear : ocean);		break;					case 5: /* 1 hex nearby assigned */		currentType = (IsAdjacentTerrain(row, column, clear) == 1) ? clear : ocean;		if (test(P3))			SetTerrainType(row, column, same, currentType);		else			SetTerrainType(row, column, same, (currentType == ocean) ? clear: ocean);		break;					case 4: /* 2 hexes nearby are assigned */		if (IsAdjacentTerrain(row, column, clear) == 2)			SetTerrainType(row, column, same, (test(P8)) ? clear : ocean);		else if (IsAdjacentTerrain(row, column, ocean) == 2)			SetTerrainType(row, column, same, (test(P5)) ? clear : ocean);		else			SetTerrainType(row, column, same, (test(P4)) ? clear : ocean);		break;					default: /*  at least 3 nearby hexes assigned */		switch (IsAdjacentTerrain(row, column, clear)) {						case 2: /* clear-clear-ocean or ocean-clear-ocean */			SetTerrainType(row, column, same, (test(P7)) ? clear : ocean);			break;							case 1: /* ocean-ocean-clear or ocean-clear-ocean */			SetTerrainType(row, column, same, (test(P4)) ? clear : ocean);			break;							case 0: /* all ocean */			SetTerrainType(row, column, same, (test(P5)) ? ocean : clear);			break;		default: /* mostly land */			SetTerrainType(row, column, same, (test(P8)) ? clear : ocean);			break;							}	}}void drawBox(short box){	short row, column;		for (column = box; column < columnSize - box; column++)		decideTerrain(box, column);	for (row = box+1; row < rowSize - box; row++)		decideTerrain(row, columnSize-1-box);	for (column = columnSize-2-box; column >= box; column--)		decideTerrain(rowSize-1-box, column);	for (row = rowSize-2-box; row >= box; row--)		decideTerrain(row, box);	}