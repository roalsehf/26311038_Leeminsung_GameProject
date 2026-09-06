#pragma once

#include <glc2d.h>

#include <string>

#include "Player.h"
#include "SceneBattlePreview.h"
#include "SceneHowToPlay.h"
#include "SceneMainMenu.h"

enum class SceneId
{
    MainMenu,
    HowToPlay,
    BattlePreview
};

struct GameResources
{
    int mainBackground = -1;
    int battleBackground = -1;
    int playerTexture = -1;
    int headingFont = -1;
    int menuFont = -1;
    int bodyFont = -1;
};

class CApplication
{
public:
    static constexpr int ScreenWidth = 1024;
    static constexpr int ScreenHeight = 640;

    int Init();
    int Update();
    int Render() const;
    int Destroy();

    void ChangeScene(SceneId nextScene);
    void RequestExit() const;

    const GameResources& GetResources() const;
    const Player& GetPlayer() const;

    void DrawFullScreenTexture(int textureKey) const;

    static bool IsKeyPressed(const KEYCODE* keys, int key);

private:
    bool CreateGameWindow();
    bool CreateFonts();
    bool LoadTextures();
    void ReleaseTexture(int& textureKey);
    std::string BuildTexturePath(const char* fileName) const;

    GameScene& ActiveScene();
    const GameScene& ActiveScene() const;

    bool initialized_ = false;
    bool windowCreated_ = false;
    SceneId currentScene_ = SceneId::MainMenu;
    GameResources resources_;
    Player player_;
    SceneMainMenu mainMenuScene_;
    SceneHowToPlay howToPlayScene_;
    SceneBattlePreview battlePreviewScene_;
};
