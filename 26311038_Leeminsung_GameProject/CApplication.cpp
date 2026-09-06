#include "CApplication.h"

#include <cstdio>

extern CApplication g_app;

namespace
{
int AppUpdate()
{
    return g_app.Update();
}

int AppRender()
{
    return g_app.Render();
}
}

int CApplication::Init()
{
    if (initialized_)
    {
        return 0;
    }

    const int initResult = g2_InitSdk();
    if (initResult != 0)
    {
        std::fprintf(stderr, "glc2d SDK initialization failed: %d\n", initResult);
        return 1;
    }

    g2_SetFrameMove(AppUpdate);
    g2_SetRender(AppRender);
    g2_SetClearColor(0xFF101827);
    g2_SetStateShow(0);
    g2_SetCursorShow(0);

    if (!CreateGameWindow() || !CreateFonts() || !LoadTextures())
    {
        Destroy();
        return 1;
    }

    currentScene_ = SceneId::MainMenu;
    initialized_ = true;
    std::printf("DUNGEON DECK initialized.\n");
    return 0;
}

int CApplication::Update()
{
    ActiveScene().Update(*this, g2_GetKeyboard());
    return 0;
}

int CApplication::Render() const
{
    ActiveScene().Render(*this);
    return 0;
}

int CApplication::Destroy()
{
    ReleaseTexture(resources_.playerTexture);
    ReleaseTexture(resources_.battleBackground);
    ReleaseTexture(resources_.mainBackground);

    if (windowCreated_)
    {
        g2_DestroyWin();
        windowCreated_ = false;
    }

    initialized_ = false;
    return 0;
}

void CApplication::ChangeScene(SceneId nextScene)
{
    currentScene_ = nextScene;
}

void CApplication::RequestExit() const
{
    PostMessage(g2_GetHwnd(), WM_CLOSE, 0, 0);
}

const GameResources& CApplication::GetResources() const
{
    return resources_;
}

const Player& CApplication::GetPlayer() const
{
    return player_;
}

void CApplication::DrawFullScreenTexture(int textureKey) const
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
        static_cast<float>(ScreenWidth) / static_cast<float>(textureWidth),
        static_cast<float>(ScreenHeight) / static_cast<float>(textureHeight));

    g2_Draw2D(textureKey, nullptr, &position, &scale);
}

bool CApplication::IsKeyPressed(const KEYCODE* keys, int key)
{
    return keys != nullptr && keys[key] == EINPUT_DOWN;
}

bool CApplication::CreateGameWindow()
{
    const int createResult = g2_CreateWin(
        100,
        70,
        ScreenWidth,
        ScreenHeight,
        "DUNGEON DECK - glc2d Card RPG",
        true);

    if (createResult != 0)
    {
        std::fprintf(stderr, "glc2d window creation failed: %d\n", createResult);
        return false;
    }

    windowCreated_ = true;
    return true;
}

bool CApplication::CreateFonts()
{
    resources_.headingFont = g2_FontCreate("Arial", 30, 0);
    resources_.menuFont = g2_FontCreate("Arial", 27, 0);
    resources_.bodyFont = g2_FontCreate("Consolas", 20, 0);

    if (resources_.headingFont < 0 ||
        resources_.menuFont < 0 ||
        resources_.bodyFont < 0)
    {
        std::fprintf(stderr, "glc2d font creation failed.\n");
        return false;
    }

    return true;
}

bool CApplication::LoadTextures()
{
    const std::string mainPath = BuildTexturePath("Main.png");
    const std::string battlePath = BuildTexturePath("InGame.png");
    const std::string playerPath = BuildTexturePath("Player.png");

    resources_.mainBackground = g2_TextureLoad(mainPath.c_str());
    resources_.battleBackground = g2_TextureLoad(battlePath.c_str());
    resources_.playerTexture = g2_TextureLoad(playerPath.c_str());

    if (resources_.mainBackground < 0 ||
        resources_.battleBackground < 0 ||
        resources_.playerTexture < 0)
    {
        std::fprintf(stderr, "One or more texture files could not be loaded.\n");
        return false;
    }

    player_.SetTexture(resources_.playerTexture);
    return true;
}

void CApplication::ReleaseTexture(int& textureKey)
{
    if (textureKey >= 0)
    {
        g2_TextureRelease(textureKey);
        textureKey = -1;
    }
}

std::string CApplication::BuildTexturePath(const char* fileName) const
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

GameScene& CApplication::ActiveScene()
{
    switch (currentScene_)
    {
    case SceneId::HowToPlay:
        return howToPlayScene_;

    case SceneId::BattlePreview:
        return battlePreviewScene_;

    case SceneId::MainMenu:
    default:
        return mainMenuScene_;
    }
}

const GameScene& CApplication::ActiveScene() const
{
    switch (currentScene_)
    {
    case SceneId::HowToPlay:
        return howToPlayScene_;

    case SceneId::BattlePreview:
        return battlePreviewScene_;

    case SceneId::MainMenu:
    default:
        return mainMenuScene_;
    }
}
