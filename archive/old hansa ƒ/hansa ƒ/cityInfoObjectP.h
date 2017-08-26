/* this is the .h file for the private section of the cityInfoObject	*//* module																*/#pragma once#include <math.h>#include "globals.h"#include "city.h"#include "cityNumberObject.h"#include "map.h"#include "utilities.h"/* sometimes we have to store a resource id into the handle in order to *//* save it */union tradeUnion {	Handle	h;	/* normal handle reference */	short 		resID;	/* resource ID for saved resource */};typedef struct trade {	union tradeUnion	nextH;	short				tradeWith;	int					tradeFlow;} *tradeNode;struct cityData {	Point 				location;	short				missionCost;	union offerUnion	offerRoot;	short 				league;	unsigned short 		production[numberOfGoods];	unsigned short 		silkProductionCost;	union tradeUnion	exports[numberOfGoods];	unsigned short		transportRate[maxNumberOfCities]; };/* static struct cityData cityInfo[maxNumberOfCities]; */static Handle cityInfoH;void 				addOfferDialog(short targetCity, short league, DialogPtr dialogP);void 				addOfferNode(short targetCity, short offeringCity, short league, float val);	void 				addTradeNode(short city1, short city2, enum goodTypes good, int amount);union offerUnion	deleteOfferNode(short city, short league);void 				deleteTradeNode(short city1, short city2, enum goodTypes good);void				restoreOfferList(short city);void				restoreTradeList(short city, enum goodTypes thisGood);void				destroyOfferList(union offerUnion rH);void				destroyTradeList(union tradeUnion rH);int					saveOfferList(union offerUnion rH);int					saveTradeList(union tradeUnion rH);