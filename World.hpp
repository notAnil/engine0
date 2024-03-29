#pragma once

#include "CR.hpp"
#include "Collision.hpp"
#include "Constraint.hpp"
#include "Particle.hpp"
#include <algorithm>
#include <memory>
#include <vector>

class World {
public:
  World(Vector2f gravity);
  void Step(float dt);
  void AddParticle(std::shared_ptr<Particle> particle);
  void RemoveParticle(std::shared_ptr<Particle> particle);
  std::vector<Particle> CopyState();
  AABB BuildBox(std::shared_ptr<Particle> pickle);
  void setBoxList();
  Vector2f gravity{0};
  void Echoes();
  void gebouw();

private:
  std::vector<std::shared_ptr<Particle>> particles;
  std::vector<debut> activeList;
  std::vector<ImpulseSolver> constraints;
};
