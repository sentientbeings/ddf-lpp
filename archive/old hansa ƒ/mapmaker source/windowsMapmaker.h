/* this file contains the external prototypes for the window module.	*/#include "mainMapmaker.h"enum buildPhase {	raiseContinentPhase, createMountainPhase, createForestPhase, digRiverPhase,	foundCityPhase, challengePhase, finalPhase};#define numberOfPhases 7void DoContent(WindowPtr whichWindow, EventRecord *theEvent,enum mouseEventType typeOfEvent);void DrawCities(void);void DrawLaborBar(DialogPtr	dPtr, int laborBar, int amountBar, Boolean mustDraw);void DrawUtilityBar(DialogPtr	dPtr, int laborBar, int amountBar);void LostGame(void);void ProcessDialogEvent(EventRecord *eventP, DialogPtr dialogP, int item);void SetUpWindows(void);void UpdateWindow(WindowPtr theWindow);void WonGame(void);void DoWaitWindow(enum buildPhase phase);void DrawCity(Point location);