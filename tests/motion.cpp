#include <doctest.h>

#include "../World.hpp"

TEST_CASE("particle motion physics") {
  World world({0, -10.f});

  auto particle = std::make_shared<Particle>();
  particle->mass = 10;

  world.AddParticle(particle);

  world.Step(1);
  CHECK(particle->position == Vector2f{0, -5});

  world.Step(1);
  CHECK(particle->position == Vector2f{0, -20});
}
