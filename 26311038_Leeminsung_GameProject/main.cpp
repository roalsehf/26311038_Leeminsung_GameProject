#include <glc2d.h>

#include <cstdio>
#include <string>

#if defined(_DEBUG)
    #if defined(_M_X64)
        #pragma comment(lib, "glc2d_x64_debug.lib")
    #elif defined(_M_IX86)
        #pragma comment(lib, "glc2d_win32_debug.lib")
    #endif
#else
    #if defined(_M_X64)
        #pragma comment(lib, "glc2d_x64_release.lib")
    #elif defined(_M_IX86)
        #pragma comment(lib, "glc2d_win32_release.lib")
    #endif
#endif

namespace
{
constexpr int kScreenWidth = 1024;
constexpr int kScreenHeight = 640;
constexpr int kMenuCount = 3;

enum class GameState
{
    MainMenu,
    HowToPlay,
    BattlePreview
};

struct TextureResources
{
    int mainBackground = -1;
    int battleBackground = -1;
    int player = -1;
};

class Player
{
public:
    void SetTexture(int textureKey)
    {
        textureKey_ = textureKey;
    }

    int GetHp() const
    {
        return hp_;
    }

    int GetMaxHp() const
    {
        return maxHp_;
    }

    int GetEnergy() const
    {
        return energy_;
    }

    void Draw() const
    {
        if (textureKey_ < 0)
        {
            return;
        }

        VEC2 position(650.0f, 225.0f);
        VEC2 scale(0.26f, 0.26f);

        g2_DrawAlphaOption(1);
        g2_Draw2D(textureKey_, nullptr, &position, &scale);
        g2_DrawAlphaOption(0);
    }

private:
    int textureKey_ = -1;
    int hp_ = 50;
    int maxHp_ = 50;
    int energy_ = 3;
};

GameState g_gameState = GameState::MainMenu;
int g_selectedMenu = 0;
int g_headingFont = -1;
int g_menuFont = -1;
int g_bodyFont = -1;
TextureResources g_textures;
Player g_player;

bool IsKeyPressed(const KEYCODE* keys, int key)
{
    return keys != nullptr && keys[key] == EINPUT_DOWN;
}

void RequestExit()
{
    PostMessage(g2_GetHwnd(), WM_CLOSE, 0, 0);
}

std::string GetTexturePath(const char* fileName)
{
    char executablePath[MAX_PATH] = {};
    const DWORD pathLength = GetModuleFileNameA(nullptr, executablePath, MAX_PATH);

    if (pathLength == 0 || pathLength >= MAX_PATH)
    {
        return std::string("texture\\") + fileName;
    }

    std::string directory(executablePath, pathLength);
    const std::string::size_type slashPosition = directory.find_last_of("\\/");

    if (slashPosition != std::string::npos)
    {
        directory.erase(slashPosition + 1);
    }
    else
    {
        directory.clear();
    }

    return directory + "texture\\" + fileName;
}

bool LoadTextures()
{
    const std::string mainPath = GetTexturePath("Main.png");
    const std::string battlePath = GetTexturePath("InGame.png");
    const std::string playerPath = GetTexturePath("Player.png");

    g_textures.mainBackground = g2_TextureLoad(mainPath.c_str());
    g_textures.battleBackground = g2_TextureLoad(battlePath.c_str());
    g_textures.player = g2_TextureLoad(playerPath.c_str());

    if (g_textures.mainBackground < 0 ||
        g_textures.battleBackground < 0 ||
        g_textures.player < 0)
    {
        std::fprintf(stderr, "One or more texture files could not be loaded.\n");
        return false;
    }

    g_player.SetTexture(g_textures.player);
    return true;
}

void ReleaseTexture(int& textureKey)
{
    if (textureKey >= 0)
    {
        g2_TextureRelease(textureKey);
        textureKey = -1;
    }
}

void ReleaseTextures()
{
    ReleaseTexture(g_textures.player);
    ReleaseTexture(g_textures.battleBackground);
    ReleaseTexture(g_textures.mainBackground);
}

void DrawFullScreenTexture(int textureKey)
{
    if (textureKey < 0)
    {
        return;
    }

    const int textureWidth = g2_TextureWidth(textureKey);
    const int textureHeight = g2_TextureHeight(textureKey);

    if (textureWidth <= 0 || textureHeight <= 0)
    {
        return;
    }

    VEC2 position(0.0f, 0.0f);
    VEC2 scale(
        static_cast<float>(kScreenWidth) / static_cast<float>(textureWidth),
        static_cast<float>(kScreenHeight) / static_cast<float>(textureHeight));

    g2_Draw2D(textureKey, nullptr, &position, &scale);
}

void SelectMenuItem()
{
    switch (g_selectedMenu)
    {
    case 0:
        g_gameState = GameState::BattlePreview;
        break;

    case 1:
        g_gameState = GameState::HowToPlay;
        break;

    case 2:
        RequestExit();
        break;

    default:
        break;
    }
}

void UpdateMainMenu(const KEYCODE* keys)
{
    if (IsKeyPressed(keys, VK_UP) || IsKeyPressed(keys, 'W'))
    {
        --g_selectedMenu;
        if (g_selectedMenu < 0)
        {
            g_selectedMenu = kMenuCount - 1;
        }
    }

    if (IsKeyPressed(keys, VK_DOWN) || IsKeyPressed(keys, 'S'))
    {
        ++g_selectedMenu;
        if (g_selectedMenu >= kMenuCount)
        {
            g_selectedMenu = 0;
        }
    }

    if (IsKeyPressed(keys, VK_RETURN))
    {
        SelectMenuItem();
    }

    if (IsKeyPressed(keys, VK_ESCAPE))
    {
        RequestExit();
    }
}

int FrameMove()
{
    const KEYCODE* keys = g2_GetKeyboard();

    if (g_gameState == GameState::MainMenu)
    {
        UpdateMainMenu(keys);
    }
    else if (IsKeyPressed(keys, VK_RETURN) || IsKeyPressed(keys, VK_ESCAPE))
    {
        g_gameState = GameState::MainMenu;
    }

    return 0;
}

void DrawMainMenu()
{
    DrawFullScreenTexture(g_textures.mainBackground);

    const char* menuItems[kMenuCount] =
    {
        "GAME START",
        "HOW TO PLAY",
        "EXIT"
    };

    for (int index = 0; index < kMenuCount; ++index)
    {
        const int top = 345 + index * 58;
        const DWORD color = index == g_selectedMenu
            ? 0xFFFFD166
            : 0xFFE6EDF3;

        g2_FontDrawText(
            g_menuFont,
            { 410, top, 740, top + 44 },
            color,
            "%s",
            menuItems[index]);
    }

    g2_FontDrawText(
        g_bodyFont,
        { 345, 555, 875, 590 },
        0xFFB8C5D6,
        "W, S or Arrow Keys: Move");
    g2_FontDrawText(
        g_bodyFont,
        { 390, 592, 830, 625 },
        0xFFB8C5D6,
        "Enter: Select   Esc: Exit");
}

void DrawHowToPlay()
{
    DrawFullScreenTexture(g_textures.battleBackground);

    g2_FontDrawText(
        g_headingFont,
        { 380, 38, 760, 88 },
        0xFFFFD166,
        "HOW TO PLAY");

    g2_FontDrawText(
        g_bodyFont,
        { 115, 120, 920, 155 },
        0xFFE6EDF3,
        "Draw five cards at the beginning of each player turn.");
    g2_FontDrawText(
        g_bodyFont,
        { 115, 168, 920, 203 },
        0xFFE6EDF3,
        "Use Energy to play Attack and Skill cards.");
    g2_FontDrawText(
        g_bodyFont,
        { 115, 216, 920, 251 },
        0xFFE6EDF3,
        "Strike deals damage. Guard adds Block. Focus draws a card.");
    g2_FontDrawText(
        g_bodyFont,
        { 115, 264, 920, 299 },
        0xFFE6EDF3,
        "End the turn to let the enemy perform its displayed action.");
    g2_FontDrawText(
        g_bodyFont,
        { 115, 312, 920, 347 },
        0xFFE6EDF3,
        "Defeat the Dungeon Warden in Stage 3 to clear the game.");
    g2_FontDrawText(
        g_bodyFont,
        { 115, 400, 920, 435 },
        0xFF70E000,
        "Starting HP: 50   Energy: 3   Hand: 5 cards");
    g2_FontDrawText(
        g_bodyFont,
        { 310, 550, 820, 590 },
        0xFFB8C5D6,
        "Enter or Esc: Return to Main Menu");
}

void DrawBattlePreview()
{
    DrawFullScreenTexture(g_textures.battleBackground);
    g_player.Draw();

    g2_FontDrawText(
        g_headingFont,
        { 405, 28, 760, 78 },
        0xFFFFD166,
        "STAGE 1");

    g2_FontDrawText(
        g_headingFont,
        { 75, 110, 450, 155 },
        0xFFFF7B7B,
        "TRAINING GOBLIN");
    g2_FontDrawText(
        g_bodyFont,
        { 80, 165, 430, 200 },
        0xFFE6EDF3,
        "HP: 32 / 32");
    g2_FontDrawText(
        g_bodyFont,
        { 80, 205, 430, 240 },
        0xFFE6EDF3,
        "Next Action: Attack 7");

    g2_FontDrawText(
        g_headingFont,
        { 720, 110, 960, 155 },
        0xFF48CAE4,
        "PLAYER");
    g2_FontDrawText(
        g_bodyFont,
        { 735, 165, 990, 200 },
        0xFFE6EDF3,
        "HP: %d / %d",
        g_player.GetHp(),
        g_player.GetMaxHp());
    g2_FontDrawText(
        g_bodyFont,
        { 735, 205, 990, 240 },
        0xFFE6EDF3,
        "Energy: %d / 3",
        g_player.GetEnergy());

    g2_FontDrawText(
        g_bodyFont,
        { 70, 455, 590, 490 },
        0xFFFFD166,
        "Week 2: Texture Load and Draw");
    g2_FontDrawText(
        g_bodyFont,
        { 70, 500, 600, 535 },
        0xFFB8C5D6,
        "Card battle rules will be connected next week.");
    g2_FontDrawText(
        g_bodyFont,
        { 310, 570, 820, 605 },
        0xFFB8C5D6,
        "Enter or Esc: Return to Main Menu");
}

int Render()
{
    switch (g_gameState)
    {
    case GameState::MainMenu:
        DrawMainMenu();
        break;

    case GameState::HowToPlay:
        DrawHowToPlay();
        break;

    case GameState::BattlePreview:
        DrawBattlePreview();
        break;
    }

    return 0;
}
}

int main()
{
    const int initResult = g2_InitSdk();
    if (initResult != 0)
    {
        std::fprintf(stderr, "glc2d SDK initialization failed: %d\n", initResult);
        return 1;
    }

    g2_SetClearColor(0xFF101827);
    g2_SetStateShow(0);
    g2_SetCursorShow(0);
    g2_SetFrameMove(FrameMove);
    g2_SetRender(Render);

    const int createResult = g2_CreateWin(
        100,
        70,
        kScreenWidth,
        kScreenHeight,
        "DUNGEON DECK - glc2d Card RPG",
        true);

    if (createResult != 0)
    {
        std::fprintf(stderr, "glc2d window creation failed: %d\n", createResult);
        g2_DestroyWin();
        return 1;
    }

    g_headingFont = g2_FontCreate("Arial", 30, 0);
    g_menuFont = g2_FontCreate("Arial", 27, 0);
    g_bodyFont = g2_FontCreate("Consolas", 20, 0);

    if (g_headingFont < 0 || g_menuFont < 0 || g_bodyFont < 0)
    {
        std::fprintf(stderr, "glc2d font creation failed.\n");
        g2_DestroyWin();
        return 1;
    }

    if (!LoadTextures())
    {
        ReleaseTextures();
        g2_DestroyWin();
        return 1;
    }

    std::printf("DUNGEON DECK textures loaded. Close the game window or press Esc to exit.\n");
    const int runResult = g2_Run();

    ReleaseTextures();
    g2_DestroyWin();

    if (runResult != 0)
    {
        std::fprintf(stderr, "glc2d game loop failed: %d\n", runResult);
        return 1;
    }

    return 0;
}
