#include "Shape.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include "../Graphics.h"
#include "Vec2.h"

bool Shape::IsPoly() const { return false; }

CircleShape::CircleShape(float radius): Shape(), radius(radius) {}

void CircleShape::DebugRender(
  Vec2 position,
  float rotation,
  Uint32 color
) const {
  Graphics::DrawCircle(
    static_cast<int>(position.x),
    static_cast<int>(position.y),
    static_cast<int>(radius),
    rotation,
    color
  );
}

ShapeType CircleShape::GetType() const { return ShapeType::CIRCLE; }

float CircleShape::GetMomentOfInertia(float mass) const {
  return mass * 0.5f * (radius * radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2>& vertices):
    Shape(), local_vertices(vertices) {
  // Sorting all the vertices to be in counter clockwise order
  Vec2 center =
    std::accumulate(local_vertices.begin(), local_vertices.end(), Vec2())
    * (1.f / static_cast<float>(local_vertices.size()));

  std::ranges::sort(
    local_vertices,
    [center](const Vec2& lhs, const Vec2& rhs) -> bool {
      const auto angle_to = [](Vec2 a, Vec2 b) {
        Vec2 to = b - a;
        return atan2(to.y, to.x);
      };

      return angle_to(center, lhs) < angle_to(center, rhs);
    }
  );
}

ShapeType PolygonShape::GetType() const { return ShapeType::POLYGON; }

void PolygonShape::UpdateVertices(Vec2 position, float rotation) {
  world_vertices.clear();
  world_vertices.reserve(local_vertices.size());

  for (auto& vertex: local_vertices) {
    Vec2 world_vertex = vertex.Rotate(rotation) + position;
    world_vertices.push_back(world_vertex);
  }
}

void PolygonShape::DebugRender(Vec2 position, float, Uint32 color) const {
  Graphics::DrawPolygon(position.x, position.y, world_vertices, color);
}

std::pair<Vec2, Vec2> PolygonShape::get_edge(size_t i) const {
  return std::make_pair(
    world_vertices[i],
    world_vertices[(i + 1) % world_vertices.size()]
  );
}

bool PolygonShape::IsPoly() const { return true; }

Vec2 PolygonShape::support_point(Vec2 direction) const {
  if (local_vertices.empty()) {
    return Vec2{};
  }

  return *std::max_element(
    world_vertices.begin(),
    world_vertices.end(),
    [direction](const Vec2& a, const Vec2& b) -> bool {
      return a.Dot(direction) < b.Dot(direction);
    }
  );
}

std::pair<Vec2, Vec2> PolygonShape::support_edge(Vec2 direction) const {
  float max_dot = 0.f;
  size_t index = 0;

  for (size_t i = 0; i < world_vertices.size(); i++) {
    const auto [start, end] = get_edge(i);
    const Vec2 normal = (end - start).Normal();

    const float dot = direction.Dot(normal);
    if (dot > max_dot) {
      index = i;
      max_dot = dot;
    }
  }

  return get_edge(index);
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
