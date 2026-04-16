#include "Player.h"
#include "InputManager.h"
#include <raymath.h>
#include <cmath>

Player::Player(Texture2D tex) : texture(tex) {
    startPos = {(float)SCREEN_X / 2, (float)SCREEN_Y / 2};
    godMode = false;
    Reset();
}

void Player::Reset() {
    pos = startPos;
    lives = 3;
    invulnTimer = 0;
    walkTimer = 0;
    rotation = 0;
    scaleY = 1.0f;
    facingRight = true;
    activePower = NONE;
    powerTimer = 0;
    dashTimer = 0;
    dashCooldown = 0;
}

void Player::Update(float dt, float baseSpeed, const std::vector<Vector2>& obstacles, float obsW, float obsH, ParticleManager& pm) {
    if (invulnTimer > 0) invulnTimer -= dt;
    if (powerTimer > 0) {
        powerTimer -= dt;
        if (powerTimer <= 0) activePower = NONE;
    }
    if (dashCooldown > 0) dashCooldown -= dt;

    float curSpeed = baseSpeed * (activePower == SPEED ? 2.0f : 1.0f);
    
    if (InputManager::Instance().IsActionPressed(ACTION_DASH) && dashCooldown <= 0 && dashTimer <= 0) {
        dashTimer = 0.15f;
        dashCooldown = 0.6f;
    }

    Vector2 moveDir = {0, 0};
    if (InputManager::Instance().IsActionDown(MOVE_RIGHT)) { moveDir.x += 1; facingRight = true; }
    if (InputManager::Instance().IsActionDown(MOVE_LEFT))  { moveDir.x -= 1; facingRight = false; }
    if (InputManager::Instance().IsActionDown(MOVE_UP))    { moveDir.y -= 1; }
    if (InputManager::Instance().IsActionDown(MOVE_DOWN))  { moveDir.y += 1; }

    if (dashTimer > 0) {
        dashTimer -= dt;
        curSpeed = baseSpeed * 4.5f;
        if (moveDir.x == 0 && moveDir.y == 0) moveDir.x = (facingRight ? 1.0f : -1.0f);
        if ((int)(GetTime() * 100) % 2 == 0) {
            pm.particles.push_back({pos, {0,0}, SKYBLUE, 0.3f, 5.0f, true});
        }
    }

    if (moveDir.x != 0 || moveDir.y != 0) {
        Vector2 nextPos = Vector2Add(pos, Vector2Scale(Vector2Normalize(moveDir), curSpeed * dt));
        
        if (nextPos.x < texture.width/2.0f) nextPos.x = texture.width/2.0f;
        if (nextPos.x > SCREEN_X - texture.width/2.0f) nextPos.x = SCREEN_X - texture.width/2.0f;
        if (nextPos.y < 100 + texture.height/2.0f) nextPos.y = 100 + texture.height/2.0f;
        if (nextPos.y > SCREEN_Y - texture.height/2.0f) nextPos.y = SCREEN_Y - texture.height/2.0f;

        bool collision = false;
        for (const auto& obs : obstacles) {
            if (CheckCollisionRecs({nextPos.x - texture.width/2.0f, nextPos.y - texture.height/2.0f, (float)texture.width, (float)texture.height}, {obs.x, obs.y, obsW, obsH})) {
                collision = true;
                break;
            }
        }
        if (!collision) pos = nextPos;

        walkTimer += dt * (dashTimer > 0 ? 40.0f : (activePower == SPEED ? 25.0f : 15.0f));
        rotation = sinf(walkTimer) * (dashTimer > 0 ? 20.0f : 10.0f);
        scaleY = 1.0f + sinf(walkTimer * 2.0f) * 0.1f;
    } else {
        rotation = Lerp(rotation, 0, 0.2f);
        scaleY = Lerp(scaleY, 1.0f, 0.2f);
        walkTimer = 0;
    }
}

void Player::Draw(Vector2 shake) {
    if (!godMode && invulnTimer > 0 && (int)(GetTime() * 10) % 2 == 0) return;
    
    Color tint = WHITE;
    if (godMode) tint = PURPLE;
    else if (dashTimer > 0) tint = SKYBLUE;
    else if (activePower == INVINCIBLE) tint = SKYBLUE;
    else if (activePower == SPEED) tint = GOLD;
    else if (activePower == SLOW) tint = LIME;

    Rectangle src = { 0, 0, (float)texture.width * (facingRight ? 1.0f : -1.0f), (float)texture.height };
    DrawTexturePro(texture, src, 
                  { pos.x + shake.x, pos.y + shake.y, (float)texture.width, (float)texture.height * scaleY }, 
                  { (float)texture.width / 2, (float)texture.height / 2 }, rotation, tint);
}

Rectangle Player::GetRect() {
    return {pos.x - texture.width/2.0f, pos.y - texture.height/2.0f, (float)texture.width, (float)texture.height};
}
