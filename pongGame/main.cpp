#include "raylib.h"
#include <vector>

using namespace std;

int player_score = 0;
int algo_score = 0;
class CustomBackground {
private:
  struct Star {
    float x, y, speed;
  };

  std::vector<Star> stars;
  int screenWidth;
  int screenHeight;

public:
  CustomBackground(int width, int height, int starCount = 100)
      : screenWidth(width), screenHeight(height) {
    for (int i = 0; i < starCount; i++) {
      stars.push_back({(float)GetRandomValue(0, screenWidth),
                       (float)GetRandomValue(0, screenHeight),
                       (float)GetRandomValue(50, 200) / 100.0f});
    }
  }

  void update() {
    for (auto &star : stars) {
      star.x -= star.speed;
      if (star.x < 0) {
        star.x = (float)screenWidth;
        star.y = (float)GetRandomValue(0, screenHeight);
      }
    }
  }

  void draw() {
    ClearBackground(BLACK); // You can replace this with a gradient later
    for (const auto &star : stars) {
      DrawPixel((int)star.x, (int)star.y, RAYWHITE);
    }
  }
};
class Ball {
public:
  float x, y, radius;
  int speed_x, speed_y;
  int color_idx;
  Ball(float x, float y, float radius) {
    this->x = x;
    this->y = y;
    this->radius = radius;
    this->speed_x = 7;
    this->speed_y = 7;
    this->color_idx = 0;
  }

  void init() {
    this->x = (float)GetScreenWidth() / 2;
    this->y = (float)GetScreenHeight() / 2;
  }

  void draw() {
    std::vector<Color> colors{
        LIGHTGRAY, GRAY,   DARKGRAY, LIGHTGRAY,  GOLD,      ORANGE,  PINK,
        RED,       MAROON, GREEN,    LIME,       DARKGREEN, SKYBLUE, BLUE,
        DARKBLUE,  PURPLE, VIOLET,   DARKPURPLE, BEIGE,     BROWN,   DARKBROWN,
        WHITE,     BLACK,  BLANK,    MAGENTA,    RAYWHITE};

    if (color_idx >= colors.size())
      color_idx = 0;
    DrawCircle(x, y, radius, colors[color_idx]);
  }

  void update() {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    this->x = x + speed_x;
    this->y = y + speed_y;

    if (x + radius >= sw) {
      player_score++;
    }

    if (x - radius <= 0) {
      algo_score++;
    }

    if (x + radius >= sw || x - radius <= 0) {
      // this->speed_x *= -1;
      // this->color_idx += 1;
      int dir = 1;
      if (x <= 0)
        dir = -1;
      this->speed_x = dir * 7;
      init();
    }
    if (y + radius >= sh || y - radius < 0) {
      this->speed_y *= -1;
    }
  }
};

class Paddle {
public:
  float x, y;
  float width, height;
  int speed = 7;
  Paddle(float x, float y, float width, float height) {
    this->x = x;
    this->y = y;
    this->height = height;
    this->width = width;
  }

  void draw() {
    // Rectangle rect();
    DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    // DrawRectangle(x, y, (int)width, (int)height, WHITE);
  }

  void update() {
    if (IsKeyDown(KEY_UP)) {
      if (y >= 0) {
        this->y = y - speed;
      }
    }
    if (IsKeyDown(KEY_DOWN)) {
      if (y + height <= GetScreenHeight()) {
        this->y = y + speed;
      }
    }
  }
};

class AlgoPaddle : public Paddle {
public:
  AlgoPaddle(float x, float y, float width, float height)
      : Paddle(x, y, width, height) {}
  void update(float ball_y) {
    if (y + height / 2 >= ball_y) {
      y -= speed;
    }
    if (y + height / 2 < ball_y) {
      y += speed;
    }
    if (y + height >= GetScreenHeight()) {
      y = GetScreenHeight() - height;
    }
    if (y <= 0) {
      y = 0;
    }
  }
};

int main() {
  const int screen_width = 1280;
  const int screen_height = 800;

  InitWindow(screen_width, screen_height, "Pong Game");
  CustomBackground background(screen_width, screen_height);
  Ball ball((float)screen_width / 2, (float)screen_height / 2, 20);
  float paddle_width = 25;
  float paddle_height = 120;
  Paddle player1(paddle_width,
                 (float)screen_height / 2 - (float)paddle_height / 2,
                 paddle_width, paddle_height);
  AlgoPaddle player2((float)screen_width - (float)2 * paddle_width,
                     (float)screen_height / 2 - (float)paddle_height / 2,
                     paddle_width, paddle_height);

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    // first update all objs
    ball.update();
    player1.update();
    player2.update(ball.y);
    background.update();

    if (CheckCollisionCircleRec(
            Vector2{ball.x, ball.y}, ball.radius,
            Rectangle{player1.x, player1.y, player1.width, player1.height})) {
      ball.speed_x *= -1;
      ball.color_idx += 1;
    }
    if (CheckCollisionCircleRec(
            Vector2{ball.x, ball.y}, ball.radius,
            Rectangle{player2.x, player2.y, player2.width, player2.height})) {
      ball.speed_x *= -1;
      ball.color_idx += 1;
    }

    BeginDrawing();
    background.draw();
    DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
    ball.draw();
    player1.draw();
    player2.draw();
    DrawText(TextFormat("%i", player_score),
             3 * screen_width / 4 - (int)paddle_width, 20, 80, GREEN);
    DrawText(TextFormat("%i", algo_score), screen_width / 4 - (int)paddle_width,
             20, 80, GREEN);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
