#include "Shape.h"
#include "../Graphics.h"

CircleShape::CircleShape(float radius): Shape(), radius(radius) {}

void CircleShape::DebugRender(Vec2 position, float rotation) const {
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
    Shape(), local_vertices(vertices) {}

ShapeType PolygonShape::GetType() const { return ShapeType::POLYGON; }

void PolygonShape::UpdateVertices(Vec2 position, float rotation) {
  world_vertices.clear();
  world_vertices.reserve(local_vertices.size());

  for (auto& vertex: local_vertices) {
    Vec2 world_vertex = vertex.Rotate(rotation) + position;
    world_vertices.push_back(world_vertex);
  }
}

void PolygonShape::DebugRender(Vec2 position, float) const {
  Graphics::DrawPolygon(position.x, position.y, world_vertices, 0xFFFFFFFF);
}

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

