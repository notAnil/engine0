#include <iostream>
#include <raylib.h>
#include <rlgl.h>

#include "World.hpp"
#include "Collision.hpp"

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(1200, 800, "Engine");

  Camera2D camera{};
  camera.offset={GetScreenWidth() / 2.f, GetScreenHeight() / 2.f};
  camera.target={};
  camera.rotation={};
  camera.zoom = 1;

  World world({0, 10.f});

  float accumulator = 0;
  float physicsTimeStep = 0.01f;

  while (!WindowShouldClose()) {
    const float frameTime = GetFrameTime();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      Vector2 delta = GetMouseDelta();
      Vector2f deltaf = {delta.x, delta.y};
      deltaf *= -1.0f / camera.zoom;

      camera.target.x += deltaf.x;
      camera.target.y += deltaf.y;
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      camera.target = GetScreenToWorld2D(GetMousePosition(), camera);
      camera.offset = GetMousePosition();

      const float zoomIncrement = 0.125f;
      camera.zoom += wheel * zoomIncrement * camera.zoom;
      if (camera.zoom < zoomIncrement)
        camera.zoom = zoomIncrement;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      auto particle = std::make_shared<Particle>();
      particle->mass = 1;
      auto pos = GetScreenToWorld2D(GetMousePosition(), camera);
      particle->position = Vector2f{pos.x, pos.y};

      world.AddParticle(particle);
    }

    accumulator += frameTime;

    std::vector<Particle> lastState;
    if (accumulator < physicsTimeStep) {
      lastState = world.CopyState();
    }

    while (accumulator >= physicsTimeStep) {
      if (accumulator - physicsTimeStep < physicsTimeStep) {
        lastState = world.CopyState();
      }

      world.Step(physicsTimeStep);
      accumulator -= physicsTimeStep;
    }

    const auto interpolation = accumulator / physicsTimeStep;

    auto currentState = world.CopyState();
    std::vector<Vector2f> positions;
    std::transform(lastState.cbegin(), lastState.cend(),
                   currentState.cbegin(), std::back_inserter(positions),
                   [&interpolation](const auto &p1, const auto &p2) {
					 return Vector2f::Interpolate(p1.position, p2.position, interpolation);
                   });

    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode2D(camera);

    for (auto &position : positions) {
      DrawRectangle(position.x, position.y, 60, 60, WHITE);
    }

    EndMode2D();

    EndDrawing();
  }

  CloseWindow();
}
