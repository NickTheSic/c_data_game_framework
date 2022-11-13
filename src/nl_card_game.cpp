#include <nl_game.h>

/*

     D   H  S   C  8  4  2  1
    128 64 32  16
*/

typedef unsigned char byte;

#define log(m, ...) fprintf(stdout, m,##__VA_ARGS__);

struct GameData
{
    byte card[52];
};

// 0 < idx < 52
byte MakeCard(byte idx)
{
    const byte suit =1 << (4+(idx/13));
    const byte val = idx%13;
    return (suit | val);
}

void CheckCard(byte card)
{
    log("%d", card & 0x0f);    

    if (card & 16)
    {
        log("%s", " of Clubs\n");
    }

    if (card & 32)
    {
        log("%s"," of Spades\n");
    }
    
    if (card & 64)
    {
        log("%s"," of Hearts\n");
    }
    
    if (card & 128)
    {
        log("%s"," of Diamonds\n");
    }
}

GameData* 
GameInitialize()
{
    for (byte i = 0; i < 52; ++i)
    {
        byte card = MakeCard(i);
        CheckCard(card);
    }

    return new GameData;
}

void 
GameUpdate(GameData* data, float delta_time)
{
    (void)data; (void)delta_time;
}

void
GameRender(GameData* data)
{
    (void)data;
}

void 
GameCleanup(GameData*& data)
{
    delete data;
}
