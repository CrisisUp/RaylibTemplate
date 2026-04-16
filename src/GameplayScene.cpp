#include "GameplayScene.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "ConfigManager.h"
#include "LocalizationManager.h"
#include "EndScenes.h"
#include <raymath.h>

GameplayScene::GameplayScene(Game* g) : game(g) {
    player = std::make_unique<Player>(ResourceManager::Instance().GetTexture("resources/Ralph.png"));
    game->score = 0;
    game->level = 1;
    game->dispScore = 0;
    freezeTimer = 0;
    flashAlpha = 0;
    shakeTimer = 0;
    ResetLevel();
}

void GameplayScene::ResetLevel() {
    player->Reset();
    obstacles.clear();
    breakables.clear();
    game->comboCount = 0;
    comboTimer = 0;

    if (game->level == 10) {
        boss = Boss();
        boss.active = true;
        boss.maxHp = ConfigManager::Instance().GetInt("bossMaxHp", 10);
        boss.hp = boss.maxHp;
        boss.speed = ConfigManager::Instance().GetFloat("bossSpeed", 120.0f);
    } else {
        boss.active = false;
        for (int i = 0; i < GetRandomValue(3, 6); i++) {
            Vector2 p;
            do {
                p = {(float)GetRandomValue(100, SCREEN_X-100), (float)GetRandomValue(150, SCREEN_Y-100)};
            } while (Vector2Distance(p, player->startPos) < 200.0f);
            obstacles.push_back(p);
        }
        for (int i = 0; i < GetRandomValue(4, 8); i++) {
            Vector2 p;
            do {
                p = {(float)GetRandomValue(100, SCREEN_X-100), (float)GetRandomValue(150, SCREEN_Y-100)};
            } while (Vector2Distance(p, player->startPos) < 150.0f);
            breakables.push_back(p);
        }
    }

    float bounceSpd = ConfigManager::Instance().GetFloat("enemyBouncerSpeed", 150.0f);
    Texture2D texObs = ResourceManager::Instance().GetTexture("resources/Obstacle.png");
    enemies = {
        {{200, 400}, {bounceSpd, bounceSpd}, (float)texObs.width, (float)texObs.height, BOUNCER},
        {{900, 200}, {-bounceSpd, bounceSpd}, (float)texObs.width, (float)texObs.height, BOUNCER},
        {{600, 100}, {100, 100}, (float)texObs.width, (float)texObs.height, HUNTER}
    };
    pUp.active = false;
    pUpTimer = 5.0f;
    pm.Clear();
    ftm.Clear();
}

void GameplayScene::TriggerImpact(float duration, float intensity, float flash) {
    freezeTimer = duration;
    shakeTimer = duration + 0.1f;
    shakeIntensity = intensity;
    flashAlpha = flash;
}

void GameplayScene::Update(float dt) {
    if (freezeTimer > 0) { freezeTimer -= dt; return; }
    if (flashAlpha > 0) flashAlpha -= dt * 5.0f;
    if (shakeTimer > 0) shakeTimer -= dt;
    if (comboTimer > 0) { comboTimer -= dt; if (comboTimer <= 0) game->comboCount = 0; }
    
    game->dispScore = Lerp(game->dispScore, (float)game->score, 0.1f);

    player->Update(dt, 300.0f, obstacles, 40, 40, pm);
    pm.Update(dt);
    ftm.Update(dt);
    
    if (boss.active) boss.Update(dt, player->pos);

    // Power-ups
    if (!pUp.active) {
        pUpTimer -= dt;
        if (pUpTimer <= 0) {
            pUp.pos = {(float)GetRandomValue(100, SCREEN_X-100), (float)GetRandomValue(150, SCREEN_Y-100)};
            pUp.type = (PowerUpType)GetRandomValue(0, 2);
            pUp.active = true;
        }
    } else {
        if (CheckCollisionRecs(player->GetRect(), {pUp.pos.x, pUp.pos.y, 40, 40})) {
            player->activePower = pUp.type;
            player->powerTimer = 5.0f;
            pUp.active = false;
            pUpTimer = 15.0f;
            ftm.Spawn(pUp.pos, LocalizationManager::Instance().Get("MSG_POWER_UP"), SKYBLUE);
            PlaySound(ResourceManager::Instance().GetSound("resources/sound/coin.wav"));
        }
    }

    // Boss Combat
    if (boss.active && CheckCollisionRecs(player->GetRect(), boss.GetRect())) {
        if (player->activePower == INVINCIBLE || player->dashTimer > 0) {
            boss.hp--;
            TriggerImpact(0.12f, 15.0f, 0.6f);
            pm.SpawnBurst(player->pos, GOLD, 30);
            if (boss.hp <= 0) {
                boss.active = false;
                game->score += 5000;
                game->ChangeScene(std::make_unique<LevelUpScene>(game));
            }
        } else if (player->invulnTimer <= 0 && !player->godMode) {
            player->lives--;
            player->pos = player->startPos;
            player->invulnTimer = 1.5f;
            TriggerImpact(0.2f, 25.0f, 0.8f);
            PlaySound(ResourceManager::Instance().GetSound("resources/sound/hit.wav"));
            if (player->lives <= 0) game->ChangeScene(std::make_unique<LostScene>(game));
        }
    }

    // Enemies
    float eMult = 1.0f + (game->level - 1) * 0.02f;
    for (auto& e : enemies) {
        e.Update(dt, player->pos, eMult, player->activePower == SLOW);
        if (e.active && CheckCollisionRecs(player->GetRect(), e.GetRect())) {
            if (player->activePower == INVINCIBLE) {
                e.active = false;
                game->score += 50;
                TriggerImpact(0.05f, 5.0f, 0.3f);
                pm.SpawnBurst(e.pos, RED, 10);
                PlaySound(ResourceManager::Instance().GetSound("resources/sound/coin.wav"));
            } else if (player->invulnTimer <= 0 && !player->godMode) {
                player->lives--;
                player->pos = player->startPos;
                player->invulnTimer = 1.5f;
                TriggerImpact(0.15f, 20.0f, 0.7f);
                PlaySound(ResourceManager::Instance().GetSound("resources/sound/hit.wav"));
                if (player->lives <= 0) game->ChangeScene(std::make_unique<LostScene>(game));
            }
        }
    }

    // Coins
    for (auto it = breakables.begin(); it != breakables.end();) {
        if (CheckCollisionRecs(player->GetRect(), {it->x, it->y, 40, 40})) {
            game->comboCount++;
            comboTimer = 2.0f;
            int pts = 100 * game->comboCount;
            pm.SpawnBurst(*it, YELLOW, 15);
            ftm.Spawn(*it, (game->comboCount > 1 ? "x"+std::to_string(game->comboCount)+"! +"+std::to_string(pts) : "+100"), (game->comboCount > 1 ? ORANGE : YELLOW));
            it = breakables.erase(it);
            game->score += pts;
            PlaySound(ResourceManager::Instance().GetSound("resources/sound/coin.wav"));
        } else ++it;
    }

    if (breakables.empty() && !boss.active) {
        game->ChangeScene(std::make_unique<LevelUpScene>(game));
    }
}

void GameplayScene::Draw() {
    Vector2 shake = {0,0};
    if (shakeTimer > 0) shake = {(float)GetRandomValue(-shakeIntensity, shakeIntensity), (float)GetRandomValue(-shakeIntensity, shakeIntensity)};
    
    Texture2D currentBg = (game->level > 10 ? ResourceManager::Instance().GetTexture("resources/EnvironmentBase2.png") : 
                          (game->level > 5 ? ResourceManager::Instance().GetTexture("resources/EnvironmentBase1.png") : 
                                           ResourceManager::Instance().GetTexture("resources/EnvironmentBase.png")));
    
    DrawTexture(currentBg, (int)shake.x, (int)shake.y, WHITE);
    
    if (pUp.active) DrawTexture(ResourceManager::Instance().GetTexture("resources/Breakable.png"), (int)(pUp.pos.x+shake.x), (int)(pUp.pos.y+shake.y), SKYBLUE);
    for (const auto& b : breakables) DrawTexture(ResourceManager::Instance().GetTexture("resources/Breakable.png"), (int)(b.x+shake.x), (int)(b.y+shake.y), WHITE);
    for (const auto& o : obstacles) DrawTexture(ResourceManager::Instance().GetTexture("resources/Obstacle.png"), (int)(o.x+shake.x), (int)(o.y+shake.y), WHITE);
    for (auto& e : enemies) e.Draw(ResourceManager::Instance().GetTexture("resources/Obstacle.png"), shake);
    if (boss.active) boss.Draw(ResourceManager::Instance().GetTexture("resources/Obstacle.png"), shake);
    
    pm.Draw(shake);
    player->Draw(shake);
    ftm.Draw(shake);
    
    // UI
    Texture2D texBar = ResourceManager::Instance().GetTexture("resources/ui/top-main-bar.png");
    DrawTexture(texBar, SCREEN_X/2-texBar.width/2, 0, WHITE);
    DrawText(TextFormat("%s: %06d  %s: %d", LocalizationManager::Instance().Get("HUD_SCORE").c_str(), (int)game->dispScore, LocalizationManager::Instance().Get("HUD_LEVEL").c_str(), game->level), SCREEN_X/2-250, 20, 30, RAYWHITE);
    for (int i = 0; i < player->lives; i++) {
        DrawTextureEx(ResourceManager::Instance().GetTexture("resources/Ralph.png"), { (float)SCREEN_X/2+150+(i*(player->lives>10?5:35)), 20 }, 0, 0.4f, WHITE);
    }
    
    if (game->comboCount > 1) DrawText(TextFormat("%s x%d", LocalizationManager::Instance().Get("HUD_COMBO").c_str(), game->comboCount), SCREEN_X-200, 650, 30, ORANGE);
    if (player->godMode) DrawText(LocalizationManager::Instance().Get("MSG_GOD_MODE").c_str(), 1000, 610, 20, PURPLE);
    if (player->dashCooldown <= 0) DrawText(LocalizationManager::Instance().Get("HUD_DASH_READY").c_str(), 50, 620, 15, SKYBLUE);
    
    if (flashAlpha > 0) DrawRectangle(0,0,SCREEN_X,SCREEN_Y, Fade(WHITE, flashAlpha));
}
