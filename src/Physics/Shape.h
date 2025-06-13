#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include "SDL_stdinc.h"
#include "Vec2.h"

enum class ShapeType {
  CIRCLE,
  POLYGON,
  BOX,
};

struct Shape {

  Shape() = default;
  virtual ~Shape() = default;

  Shape(const Shape&) = default;
  Shape(Shape&&) = delete;
  Shape& operator=(const Shape&) = default;
  Shape& operator=(Shape&&) = delete;

  template<typename T>
  T* as() {
    return dynamic_cast<T*>(this);
  }

  [[nodiscard]] virtual ShapeType GetType() const = 0;

  [[nodiscard]] virtual float GetMomentOfInertia(float) const { return 0.f; }

  virtual void DebugRender(
    Vec2 position,
    float rotation,
    Uint32 color
  ) const = 0;

  [[nodiscard]] virtual bool IsPoly() const;
};

struct CircleShape : public Shape {
  float radius;

  explicit CircleShape(float radius);
  ~CircleShape() override = default;

  CircleShape(const CircleShape&) = default;
  CircleShape(CircleShape&&) = delete;
  CircleShape& operator=(const CircleShape&) = default;
  CircleShape& operator=(CircleShape&&) = delete;

  [[nodiscard]] ShapeType GetType() const override;

  [[nodiscard]] float GetMomentOfInertia(float mass) const override;

  void DebugRender(Vec2 position, float rotation, Uint32 color) const override;

  [[nodiscard]] Vec2 support_point(Vec2 position, Vec2 direction) const {
    return position + (direction * radius);
  }
};

struct PolygonShape : public Shape {
  std::vector<Vec2> local_vertices;
  std::vector<Vec2> world_vertices{};

  explicit PolygonShape(const std::vector<Vec2>& vertices);

  ~PolygonShape() override = default;

  PolygonShape(const PolygonShape&) = default;
  PolygonShape(PolygonShape&&) = delete;
  PolygonShape& operator=(const PolygonShape&) = default;
  PolygonShape& operator=(PolygonShape&&) = delete;

  [[nodiscard]] ShapeType GetType() const override;

  void UpdateVertices(Vec2 position, float rotation);

  void DebugRender(Vec2 position, float rotation, Uint32 color) const override;

  [[nodiscard]] bool IsPoly() const override;

  [[nodiscard]] Vec2 support_point(Vec2 direction) const;
};

struct BoxShape : public PolygonShape {
  float width;
  float height;

  explicit BoxShape(float width, float height);

  ~BoxShape() override = default;

  BoxShape(const BoxShape&) = default;
  BoxShape(BoxShape&&) = delete;
  BoxShape& operator=(const BoxShape&) = default;
  BoxShape& operator=(BoxShape&&) = delete;

  [[nodiscard]] ShapeType GetType() const override;

  [[nodiscard]] float GetMomentOfInertia(float mass) const override;
};

#endif
