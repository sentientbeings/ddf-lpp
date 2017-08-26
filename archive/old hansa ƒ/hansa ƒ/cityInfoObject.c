/* this file contains the accessor functions for the city primitives	*/#include "cityInfoObject.h"#include "cityInfoObjectP.h"void CreateCityInfoObject(void){	short i;	enum goodTypes j;		cityInfoH = NewHandle(maxNumberOfCities * sizeof(struct cityData));	for (i = 0; i < maxNumberOfCities; i++) {		((struct cityData *) *cityInfoH)[i].offerRoot.h = NULL;		for (j = wheat; j < numberOfGoods; j++)			((struct cityData *) *cityInfoH)[i].exports[j].h = NULL;	}	SetNumberOfCities(0);}/* what is the autarchy point for the passed city? */float GetAutarchicLevel(short city){	short itemsProduced, thisCost, i, cheapestCost;	float c[numberOfGoods], Q[numberOfGoods], excess;	enum goodTypes good;		for (i = 0; i < numberOfGoods; i++) {			c[i] = 0.0;			Q[i] = 0.0;		}		cheapestCost = 4;		for (itemsProduced = 0, good = wheat; good < numberOfGoods; good++)			if ((thisCost = GetProductionCost(city, good)) != 0) {				c[itemsProduced++] = thisCost;				if (thisCost < cheapestCost)					cheapestCost = thisCost;			}		if (itemsProduced == 0)			return 0.0;		for (i = 0; i < itemsProduced; i++)			Q[i] = floor(-1.0 + (50.0 + c[0]+c[1]+c[2]+c[3]+c[4]+c[5])/ (itemsProduced * c[i]));		if ((excess = 50.0 - (Q[0]*c[0]) - (Q[1]*c[1]) - (Q[2]*c[2]) - (Q[3]*c[3]) - (Q[4]*c[4]) - (Q[5]*c[5])) >= cheapestCost)			for (i = 0; i < itemsProduced; i++)				while (excess >= c[i]) {					Q[i]++;					excess -= c[i];				}		return pow(((Q[0]+1.0) * (Q[1]+1.0) * (Q[2]+1.0) * (Q[3]+1.0) * (Q[4]+1.0) * (Q[5]+1.0) )-1.0, 1.0/numberOfGoods);}int GetExport(short city1, short city2, enum goodTypes good){	int val;		return ((val = GetTradeFlow(city1, city2, good)) > 0) ? val : 0;	}int GetImport(short city1, short city2, enum goodTypes good){	int val;		return ((val = GetTradeFlow(city1, city2, good)) < 0) ? -val : 0;	}short GetLeague(short city){	return ((struct cityData *) *cityInfoH)[city].league;}short GetMissionCost(short city){	return ((struct cityData *) *cityInfoH)[city].missionCost;}Point GetLocation(short city){	return ((struct cityData *) *cityInfoH)[city].location;}union offerUnion GetOfferRoot(short city){	return ((struct cityData *) *cityInfoH)[city].offerRoot;}unsigned short GetProduction(short city, enum goodTypes good){	return ((struct cityData *) *cityInfoH)[city].production[good];}unsigned short GetSilkProductionCost(short city){	return ((struct cityData *) *cityInfoH)[city].silkProductionCost;}int GetTradeFlow(short city1, short city2, enum goodTypes good){	union tradeUnion rH;		if (((struct cityData *) *cityInfoH)[city1].exports[good].h == NULL)		return 0;	rH = ((struct cityData *) *cityInfoH)[city1].exports[good];	while ((rH.h != NULL) && (((tradeNode) *(rH.h))->tradeWith != city2))		if (((tradeNode)*(rH.h))->nextH.h != NULL)			rH = ((tradeNode) *(rH.h))->nextH;		else rH.h = NULL;	if (rH.h != NULL)		return ((tradeNode) *(rH.h))->tradeFlow;	else return 0;}/*unsigned GetTransportValue(short city1, short city2){	return ((struct cityData *) *cityInfoH)[city1].transportRate[city2];} */void KillCityInfoObject(void){	DisposHandle(cityInfoH);}void RestoreCityInfoObject(void){	short i;	enum goodTypes thisGood;	cityInfoH = GetNamedResource('TABL', "\pcityInfo");	DetachResource(cityInfoH);	if (cityInfoH != NULL) 		for (i = 0; i < maxNumberOfCities; i++) {			restoreOfferList(i);			for (thisGood = wheat; thisGood < numberOfGoods; thisGood++)				restoreTradeList(i, thisGood);		}	else CreateCityInfoObject();}	void DestroyCityInfoObject(void){	short i;	enum goodTypes thisGood;		for (i = 0; i < maxNumberOfCities; i++) {		destroyOfferList(((struct cityData *) *cityInfoH)[i].offerRoot);		for (thisGood = wheat; thisGood < numberOfGoods; thisGood++)			destroyTradeList(((struct cityData *) *cityInfoH)[i].exports[thisGood]);	}	DisposHandle(cityInfoH);}void SaveCityInfoObject(void){	short i;	enum goodTypes thisGood;		for (i = 0; i < maxNumberOfCities; i++) {		((struct cityData *) *cityInfoH)[i].offerRoot.resID = saveOfferList(((struct cityData *) *cityInfoH)[i].offerRoot);		for (thisGood = wheat; thisGood < numberOfGoods; thisGood++)			((struct cityData *) *cityInfoH)[i].exports[thisGood].resID = saveTradeList(((struct cityData *) *cityInfoH)[i].exports[thisGood]);	}	AddResource(cityInfoH, 'TABL', UniqueID('TABL'), "\pcityInfo");	WriteResource(cityInfoH);	DetachResource(cityInfoH);		/* now we must restore the base handles that were turned into resource ids */		for (i = 0; i < maxNumberOfCities; i++) {		restoreOfferList(i);		for (thisGood = wheat; thisGood < numberOfGoods; thisGood++)			restoreTradeList(i, thisGood);	}}void SetOfferRoot(short city, Handle oH){	((struct cityData *) *cityInfoH)[city].offerRoot.h = oH;}void InitTradeRoot(short city){	enum goodTypes good;		for (good = wheat; good < numberOfGoods; good++)		((struct cityData *) *cityInfoH)[city].exports[good].h = NULL;}void SetTradeFlow(short city1, short city2, enum goodTypes good, int val){	tradeNode tP;		if (((struct cityData *) *cityInfoH)[city1].exports[good].h == NULL)		tP = NULL;	else tP = (tradeNode) *(((struct cityData *) *cityInfoH)[city1].exports[good].h);	while ((tP != NULL) && (tP->tradeWith != city2))		if (tP->nextH.h != NULL)			tP = (tradeNode) *(tP->nextH.h);		else tP = NULL;	if (tP == NULL) /* trade node does not exist, so go make one */		addTradeNode(city1, city2, good, val);	else {		tP->tradeFlow = val;		if (tP->tradeFlow == 0) /* delete 0 trade beween cities */			deleteTradeNode(city1, city2, good);	}}void SetLeague(short city, short val){	((struct cityData *) *cityInfoH)[city].league = val;}void SetLocation(short city, Point loc){	((struct cityData *) *cityInfoH)[city].location = loc;}void SetMissionCost(short city, unsigned short val){	((struct cityData *) *cityInfoH)[city].missionCost = val;}void SetProduction(short city, enum goodTypes good, unsigned short val){	((struct cityData *) *cityInfoH)[city].production[good] = val;}void SetSilkProductionCost(short city, unsigned short val){	((struct cityData *) *cityInfoH)[city].silkProductionCost = val;}void SetTransportValue(short city1, short city2, unsigned val){	((struct cityData *) *cityInfoH)[city1].transportRate[city2] = val;}void addTradeNode(short city1, short city2, enum goodTypes good, int amount){	Handle tH;		if (amount != 0) {		tH = NewHandle(sizeof(struct trade));		((tradeNode)*tH)->nextH.h = ((struct cityData *) *cityInfoH)[city1].exports[good].h;		((tradeNode)*tH)->tradeWith = city2;		((tradeNode)*tH)->tradeFlow = amount;		((struct cityData *) *cityInfoH)[city1].exports[good].h = tH;	}}void deleteTradeNode(short city1, short city2, enum goodTypes good){	tradeNode testP, lastP;	Handle testH, lastH;		testH = ((struct cityData *) *cityInfoH)[city1].exports[good].h;	testP = (tradeNode) *testH;	if (testP->tradeWith == city2) { /* node is first node in list */		((struct cityData *) *cityInfoH)[city1].exports[good].h = testP->nextH.h;		DisposHandle(testH);	} else {		while (testP->tradeWith != city2) {			lastH = testH;			testH = testP->nextH.h;			testP = (tradeNode) *testH;		}		lastP = (tradeNode) *lastH;		lastP->nextH = testP->nextH;		DisposHandle(testH);	}}void restoreOfferList(short city){	union offerUnion rH;		if (((struct cityData *) *cityInfoH)[city].offerRoot.resID  == 0) {		((struct cityData *) *cityInfoH)[city].offerRoot.h = NULL;	} else { /* read in the list of offers for this city */		((struct cityData *) *cityInfoH)[city].offerRoot.h = GetResource('TABL', ((struct cityData *) *cityInfoH)[city].offerRoot.resID);		DetachResource(((struct cityData *) *cityInfoH)[city].offerRoot.h);		rH = ((struct cityData *) *cityInfoH)[city].offerRoot;		while (((offerNode)*(rH.h))->nextH.resID != 0) {			((offerNode)*(rH.h))->nextH.h = GetResource('TABL', ((offerNode)*(rH.h))->nextH.resID);			DetachResource(((offerNode)*(rH.h))->nextH.h);			rH = ((offerNode)*(rH.h))->nextH;		}		((offerNode)*(rH.h))->nextH.h = NULL; /* last element points to null */	}}void restoreTradeList(short city, enum goodTypes thisGood){	union tradeUnion rH;		if (((struct cityData *) *cityInfoH)[city].exports[thisGood].resID  == 0) {		((struct cityData *) *cityInfoH)[city].exports[thisGood].h = NULL;	} else { /* read in the list of trades for this city */		((struct cityData *) *cityInfoH)[city].exports[thisGood].h = GetResource('TABL', ((struct cityData *) *cityInfoH)[city].exports[thisGood].resID);		DetachResource(((struct cityData *) *cityInfoH)[city].exports[thisGood].h);		rH = ((struct cityData *) *cityInfoH)[city].exports[thisGood];		while (((tradeNode)*(rH.h))->nextH.resID != 0) {			((tradeNode)*(rH.h))->nextH.h = GetResource('TABL', ((tradeNode)*(rH.h))->nextH.resID);			DetachResource(((tradeNode)*(rH.h))->nextH.h);			rH = ((tradeNode)*(rH.h))->nextH;		}		((tradeNode)*(rH.h))->nextH.h = NULL; /* last element points to null */	}}void destroyOfferList(union offerUnion rH){	if (rH.h == NULL)		return;	destroyOfferList(((offerNode)*(rH.h))->nextH);	DisposHandle(rH.h);}int saveOfferList(union offerUnion rH){	int retVal;		if (rH.h == NULL)		return 0;	((offerNode) *(rH.h))->nextH.resID = saveOfferList(((offerNode)*(rH.h))->nextH);	AddResource(rH.h, 'TABL', retVal = UniqueID('TABL'), nullString);	WriteResource(rH.h);	ReleaseResource(rH.h);	return retVal; }void destroyTradeList(union tradeUnion rH){	if (rH.h == NULL)		return;	destroyTradeList(((tradeNode)*(rH.h))->nextH);	DisposHandle(rH.h);}/* this procedure recursively saves a linked list of trade nodes. It 	*//* leaves resource ID's in the high word of the handle locations, and	*//* destroys the list 													*/int saveTradeList(union tradeUnion rH){	int retVal;		if (rH.h == NULL)		return 0;	((tradeNode) *(rH.h))->nextH.resID = saveTradeList(((tradeNode)*(rH.h))->nextH);	AddResource(rH.h, 'TABL', retVal = UniqueID('TABL'), nullString);	WriteResource(rH.h);	ReleaseResource(rH.h);	return retVal; }