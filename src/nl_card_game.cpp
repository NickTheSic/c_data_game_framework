#include <nl_game.h>
#include <nl_renderer.h>
#include <nl_shader.h>

#include <string>

unsigned int g_random_seed = 13;

unsigned int SimpleRandom(unsigned int seed = g_random_seed)
{
    g_random_seed = seed;

    g_random_seed ^= g_random_seed << 13;
    g_random_seed ^= g_random_seed >> 17;
    g_random_seed ^= g_random_seed << 5;

    return g_random_seed;
}

typedef unsigned char byte;

/*
     D   H  S   C  8  4  2  1
    128 64 32  16
*/

enum class Suit : byte
{
    clubs   =  16,
    spades  =  32,
    hearts  =  64,
    diamods = 128,
};

#define log(m, ...) fprintf(stdout, m,##__VA_ARGS__);

struct Card
{
    v2f pos;
    byte data;
    bool visible;
};

struct Deck 
{
    Card card[52];
};

struct GameData
{
    Deck deck;
    SpriteHandle card_sprites[52];
    SpriteHandle card_back_sprite;
    SpriteHandle card_empty_sprite;
    SpriteSheet sheet;
    Shader shader;
    Camera camera;

    float active_card;
};

// 0 < idx < 52
byte 
MakeCard(byte idx)
{
    const byte suit =1 << (4+(idx/13));
    const byte val = idx%13;
    return (suit | val);
}

byte 
GetCardValue(byte card)
{
    return card & 0x0f;
}

byte 
GetCardSuit(byte card)
{
    return card & 0xf0;
}

void 
CheckCard(byte card)
{
    switch (card & 0x0f)
    {
        case 0:
            log("%s", "Ace");
        break;

        case 10:
            log("%s", "Jack");
        break;

        case 11:
            log("%s", "Queen");
        break;

        case 12:
            log("%s", "King");
        break;

        default:
            log("%d", 1 + (card & 0x0f));
        break;
    }

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

void 
CheckDeck(Deck* deck)
{
    for (byte i = 0; i < 52; ++i)
    {
        CheckCard(deck->card[i].data);
    }
    log("%s", "\n");
}

void 
ResetDeck(Deck* deck)
{
    for (byte i = 0; i < 52; ++i)
    {
        deck->card[i].data = MakeCard(i);
    }
}

void 
ShuffleDeck(Deck* deck)
{
    for (byte i = 0; i < 104; ++i)
    {
        int sr1, sr2;
        sr1 = SimpleRandom() % 52;
        sr2 = SimpleRandom() % 52;

        byte temp = deck->card[sr1].data;
        deck->card[sr1].data = deck->card[sr2].data;
        deck->card[sr2].data = temp;
    }
}

void
DeckInputCallback(KeyState state, Key key, void* data)
{
    Deck* deck = static_cast<Deck*>(data);
    if (state == KeyState::Down)
    {
        if (key == Key::R)
        {
            ResetDeck(deck);
        }
        if (key == Key::S)
        {
            ShuffleDeck(deck);
        }
        CheckDeck(deck);
    }
}

GameData* 
GameInitialize()
{
    GameData* data = new GameData;

    ResetDeck(&data->deck);
    CheckDeck(&data->deck);

    ShuffleDeck(&data->deck);
    CheckDeck(&data->deck);

    AddActionCallback(&data->deck, &DeckInputCallback);

    // Todo: Extract out of Game Init, we can probably specify 4 pieces of data back to the platform
    data->camera.size.x = 300.f;
    data->camera.size.y = 200.f;
    InitializeRenderer(&data->sheet.renderer, 52, sizeof(SpriteVertexData));
    InitializeSpriteSheet(&data->sheet, 32 * 14, 48*5);
    CreateViewMatrixFollow(&data->camera, {0.0f,0.f,0.f});
    CompileSpriteShaderProgram(&data->sheet.renderer);
    data->shader.program = data->sheet.renderer.shader_program;
    SetUniform(&data->shader, "view", data->camera.view);

    data->card_empty_sprite = LoadSprite(&data->sheet, "data/cards/card_0000.png");
    data->card_back_sprite = LoadSprite(&data->sheet, "data/cards/card_0001.png");

    for (int i = 0; i < 52; ++i)
    {
        std::string path = "data/cards/card_00";
        if (i+2 < 10) 
        {
            path.append("0");
        }
        path.append(std::to_string(i+2));
        path.append(".png");

        data->card_sprites[i] = LoadSprite(&data->sheet, path.c_str());
    }

    return data;
}

void 
GameUpdate(GameData* data, float delta_time)
{
    data->active_card += delta_time * 4;
}

void
GameRender(GameData* data)
{
    BeginRender(&data->sheet);

    DisplayEntireSheet(&data->sheet, {0.f,0.f,0.f}, {100.f,100.f});

    for (int i = 0; i < 52; ++i)
    {
        float z_order = 0.0f;
        if (i == (int)data->active_card) z_order = 1.f;
        AddSpriteToRender(&data->sheet, data->card_sprites[i], {-data->camera.size.x + (i*10), -30.f, z_order});
    }

    EndRender(&data->sheet.renderer);
}

void 
GameCleanup(GameData*& data)
{
    CleanupRenderer(&data->sheet.renderer);
    CleanupSpriteSheet(&data->sheet);
    delete data;
}
