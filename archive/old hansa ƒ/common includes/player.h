/* this contains the public definitions for the player module			*/#pragma onceenum playerType {	human, computer, inactive};#define Player1		1#define Player2		2#define Player3		3#define Player4		4#define noPlayer	-1#define playerError -1 #define maxNumberOfPlayers 4#define GetFirstCity(_league) (GetNextCity((_league), -1))short			AddPlayer(enum playerType type); void			DestroyPlayerInfo(void);void 			DropPlayer(short league);short			GetCurrentPlayer(void);short			GetFirstActivePlayer(void);short			GetLastActivePlayer(void);short			GetNextActivePlayer(short which);short			GetNextCity(short league, short city);short			GetNumberOfHumanPlayers(void);short			GetNumberOfPlayers(void);enum playerType	GetPlayerStatus(short league);void			InitPlayerInfo(void);void			RestorePlayerInfo(void);void			SavePlayerInfo(void);void			SetPlayerStatus(short league, enum playerType status);Boolean			SetCurrentPlayer(short player); 