#include "Shape.h"
#include "../Graphics.h"

CircleShape::CircleShape(float radius): Shape(), radius(radius) {}

void CircleShape::Render(Vec2 position, float rotation) const {
  Graphics::DrawCircle(
    static_cast<int>(position.x),
    static_cast<int>(position.y),
    static_cast<int>(radius),
    rotation,
    0xFFFFFFFF
  );
}

ShapeType CircleShape::GetType() const { return ShapeType::CIRCLE; }

float CircleShape::GetMomentOfInertia(float mass) const {
  return mass * 0.5f * (radius * radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2>& vertices):
    Shape(), vertices(vertices) {}

ShapeType PolygonShape::GetType() const { return ShapeType::POLYGON; }

BoxShape::BoxShape(float width, float height):
    PolygonShape([width, height]() -> std::vector<Vec2> {
      float h_width = width / 2.f;
      float h_height = height / 2.f;

      return {
        {h_width,  h_height },
        {-h_width, h_height },
        {-h_width, -h_height},
        {h_width,  -h_height},
      };
    }()),
    width(width),
    height(height) {};

ShapeType BoxShape::GetType() const { return ShapeType::BOX; }

float BoxShape::GetMomentOfInertia(float mass) const {
  return (1.f / 12.f) * (width * width + height * height) * mass;
}
