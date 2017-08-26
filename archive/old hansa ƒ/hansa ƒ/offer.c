/* this contains the code for the offer module.	It is responsible for	*//* openning and manipulating the offer window							*//*																		*//* 05 Jul 1990 tomc creation date. Split off from trade.c & cityInfo.c	*/#include "offer.h"#include "offerP.h"void CloseOfferWindow(void){	SetOfferCity(noCity);	if (offerP != NULL) {		DisposDialog(offerP);		offerP = NULL;	}}/* delete a specific offer to a city, if it exists */void DeleteAnOffer(short city, short league){	union offerUnion oH;		oH = GetOfferRoot(city);	while (oH.h != NULL)		if (((offerNode) *(oH.h))->league == league)			oH = deleteOfferNode(city, league);		else oH = ((offerNode) *(oH.h))->nextH;}/* delete the offers on a city. Optionally delete an offer to itself,	*//* which is how we keep track of the guarentee of an accepted city.		*/void DeleteOffers(short city, Boolean ownOffers){	union offerUnion oH;		oH = GetOfferRoot(city);	while (oH.h != NULL)		if (ownOffers || (((offerNode) *(oH.h))->league != GetLeague(city)))			oH = deleteOfferNode(city, ((offerNode) *(oH.h))->league);		else oH = ((offerNode) *(oH.h))->nextH;}short GetOfferCity(void){	return offerCity;}float GetOfferLevel(short city, short league){	union offerUnion oH;		oH = GetOfferRoot(city);	while ((oH.h != NULL) && (((offerNode) *(oH.h))->league != league))		oH = ((offerNode) *(oH.h))->nextH;	if (oH.h != NULL)		return ((offerNode) *(oH.h))->value;	else return (GetLeague(city) == league) ? 0.0 : noOffer;}DialogPtr GetOfferWindow(void){	return offerP;}short GetOfferingCity(short target, short league){	union offerUnion oH;		oH = GetOfferRoot(target);	while ((oH.h != NULL) && (((offerNode) *(oH.h))->league != league))		oH = ((offerNode) *(oH.h))->nextH;	return (oH.h != NULL) ? ((offerNode) *(oH.h))->offeringCity : noCity;}Boolean OfferProferred(short city){	union offerUnion oH;		oH = GetOfferRoot(city);	while (oH.h != NULL)		if ((((offerNode) *(oH.h))->offeringCity != city) &&			(GetLeague(((offerNode) *(oH.h))->offeringCity) == GetCurrentPlayer()))			return true;		else oH = ((offerNode) *(oH.h))->nextH;	return false;}void OpenOfferWindow(short targetCity){	ControlHandle 	goodH;	short 			type;	Rect			invalRect;		if (offerP != NULL)		DisposDialog(offerP);	SetPort(offerP = GetNewDialog(offerID, 0L, (WindowPtr)-1L));	GetDItem(offerP, offerBarItem, &type, (Handle *) &goodH, &invalRect);	SetCtlMax(goodH, 10);	ShowWindow(offerP);	SetOfferCity(targetCity);	DrawCities();}/* we have an event in an offer window */void ProcessOfferEvent(EventRecord *eventP, DialogPtr dialogP, int item){	ControlHandle	valH;	float val;	short type;	Rect drawRect;	Str255 numberString;		GetDItem(dialogP, item, &type, (Handle *) &valH, &drawRect);	if (item == doneOfferButtonItem) {		DisposDialog(dialogP);		offerP = NULL;		SetOfferCity(noCity);	} else if (type == (ctrlItem+resCtrl)) {		GlobalToLocal(&(eventP->where));		switch (TrackControl(valH, eventP->where, 0L)) {				case inDownButton:			if (GetCtlValue(valH) < GetCtlMax(valH))				changeOfferLevel(GetOfferCity(), GetCurrentPlayer(), .1);			break;		case inUpButton:			if (GetCtlValue(valH) > GetCtlMin(valH))				changeOfferLevel(GetOfferCity(), GetCurrentPlayer(), -.1);			break;					case inPageDown:			if (GetCtlValue(valH) < (GetCtlMax(valH)-5))				changeOfferLevel(GetOfferCity(), GetCurrentPlayer(), .5);			else				changeOfferLevel(GetOfferCity(), GetCurrentPlayer(), (GetCtlMax(valH)-GetCtlValue(valH)) / 10.0);			break;					case inPageUp:			if (GetCtlValue(valH) >= 5)				changeOfferLevel(GetOfferCity(), GetCurrentPlayer(), -.5);			else				changeOfferLevel(GetOfferCity(), GetCurrentPlayer(), (GetCtlMin(valH)-GetCtlValue(valH)) / 10.0);			break;					}		GetDItem(dialogP, offerBarItem, &type, (Handle *) &valH, &drawRect);		SetCtlValue(valH, (int) (GetOfferLevel(GetOfferCity(), GetCurrentPlayer()) * 10.0));		GetDItem(dialogP, offerAmountItem, &type, (Handle *) &valH, &drawRect);		val = GetOfferLevel(GetOfferCity(), GetCurrentPlayer());		val *= 10;		val = floor(val)  / 10.0;		FloatToString(numberString, val, 1);		SetIText((Handle) valH, numberString);	}			}void ResolveOffers(void){	short i, pickedLeague;	float maxOffer, testFloat;	union offerUnion oH;		for (i = 0; i < maxNumberOfCities; i++) {		oH = GetOfferRoot(i);		maxOffer = -1.0;		pickedLeague = noLeague;		while (oH.h != NULL) {			if (((offerNode) *(oH.h))->offeringCity != i) { /* skip offers to itself */				if (((offerNode) *(oH.h))->value == maxOffer) { /* don't resolve ties */					pickedLeague = noLeague;					break;				} else if ((((offerNode) *(oH.h))->value > maxOffer) &&							(((offerNode) *(oH.h))->league != GetLeague(i))) {					pickedLeague = ((offerNode) *(oH.h))->league;					maxOffer = ((offerNode) *(oH.h))->value;				}			}			oH = ((offerNode) *(oH.h))->nextH;		}		if (pickedLeague != noLeague) { /* must resolve */			if (GetLeague(i) == noLeague)				testFloat = GetOfferLevel(i, pickedLeague) * 100.0 + 500.0;			else if (GetPlayerStatus(GetLeague(i)) == computer)				testFloat = GetOfferLevel(i, pickedLeague) * 100.0;			else 				testFloat = (GetOfferLevel(i, pickedLeague) + GetAutarchicLevel(i) - GetUtility(i) - GetOfferLevel(i, GetLeague(i))) * 100.0 + 200.0;			if (test((int) testFloat)) {				ZeroTrade(i);				SetLeague(i, pickedLeague);//				GetTransportCosts(i, i, noGood);				SetOfferLevel(i, pickedLeague, maxOffer);			}		}		DeleteOffers(i, false);	}}void SetOfferCity(short city){	offerCity = city;}void SetOfferLevel(short targetCity, short offeringCity, float val){	union offerUnion oH;	short league;	oH = GetOfferRoot(targetCity);	league = GetLeague(offeringCity);	while ((oH.h != NULL) && (((offerNode) *(oH.h))->league != league))		oH = ((offerNode) *(oH.h))->nextH;	if (oH.h == NULL)		addOfferNode(targetCity, offeringCity, league, val);	((offerNode) *(oH.h))->value = val;}/* private procedures */void addOfferNode(short targetCity, short offeringCity, short league, float val){	union offerUnion oH;		oH.h = NewHandle(sizeof(struct offer));	((offerNode) *(oH.h))->league = league;	((offerNode) *(oH.h))->value = val;	((offerNode) *(oH.h))->offeringCity = offeringCity;	((offerNode) *(oH.h))->nextH = GetOfferRoot(targetCity);	SetOfferRoot(targetCity, oH.h);}void changeOfferLevel(short target, short league, float increment){	SetOfferLevel(target, GetOfferingCity(target, league), GetOfferLevel(target, league)+increment);}union offerUnion deleteOfferNode(short city, short league){	union offerUnion testH, nextNode;		testH = GetOfferRoot(city);	if (testH.h == NULL) /* no nodes to delete */		return testH;	else if (((offerNode) *(testH.h))->league == league) { /* delete first node in list */		SetOfferRoot(city, ((offerNode) *(testH.h))->nextH.h);		nextNode = ((offerNode) *(testH.h))->nextH;		DisposHandle(testH.h);		return nextNode;	} else /* Find the right node and delete it. */		while (((offerNode) *(testH.h))->nextH.h != NULL) {			nextNode = ((offerNode) *(testH.h))->nextH;			if (((offerNode) *(nextNode.h))->league == league) { /* found it */				((offerNode) *(testH.h))->nextH = ((offerNode) *(nextNode.h))->nextH;				DisposHandle(nextNode.h);				return ((offerNode) *(testH.h))->nextH;			} else testH = nextNode;		}	testH.h = NULL;	return testH;				}