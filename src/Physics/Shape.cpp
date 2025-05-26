#include "Shape.h"

CircleShape::CircleShape(float radius): Shape(), radius(radius) {}

ShapeType CircleShape::GetType() const { return ShapeType::CIRCLE; }

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
