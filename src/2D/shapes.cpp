#include <iostream>
#include "shapes.hpp"
#include "../math/geometry.hpp"

using namespace Geometry;

namespace Shapes {
   /**
   * @brief Calculate the polygons vertex coordinates based on the start point, which begins at the bottom of the circle
   * bounding the polygon to be drawn. In this configuration, we will draw a non-rotated polygon, with a line along
   * the x-axis at the bottom.
   *
   * @param vertex Starting at the bottom right, vertex to calculate coordinates for (counterclockwise)
   * @return Vector vertex coordinates
   */
  Vector2D *Polygon::calculatePolygonVertex(unsigned int vertex) {
    Vector2D *polygonVertex = new Vector2D();
    polygonVertex->vector[0] = this->centerPt.vector[0]
      + this->radius * cos(
        Angles::degreeToRadian(vertex * this->omega)
      );
    polygonVertex->vector[1] = this->centerPt.vector[1]
      + this->radius * sin(
        Angles::degreeToRadian(vertex * this->omega)
      );
    std::cout << polygonVertex->vector[0] << std::endl;
    std::cout << polygonVertex->vector[1] << std::endl;
    return polygonVertex;
  }

  /**
   * @brief Calculates vertex positions for polygon with n sides
   */
  void Polygon::calculateVertices() {
    this->vertices = new Vector2D[this->numberOfSides];
    for (unsigned int i = 0; i < this->numberOfSides; i++) {
      this->vertices[i] = *calculatePolygonVertex(i);
    }
  }

  Polygon::Polygon(ShapeDrawingStyle drawingStyle) {
    this->drawingStyle = drawingStyle;
  }

  // getters
  unsigned int Shape2D::getNumberOfSides() { return this->numberOfSides; }
  float Shape2D::getRadius() { return this->radius; }
  float Shape2D::getSideLen() { return this->sideLen; }
  Vector2D Shape2D::getCenterPt() { return this->centerPt; }
  Vector2D Shape2D::getStartPt() { return this->startPt; }
  Vector2D *Shape2D::getVertices() { return this->vertices; }

  // setters
  void Shape2D::setRadius(float radius) { this->radius = radius; }
  void Shape2D::setNumberOfSides(unsigned int numberOfSides) {
    if (numberOfSides > 0) {
      this->numberOfSides = numberOfSides;
      this->omega = 360.0f / this->numberOfSides;
    }
  }
  void Shape2D::setCenterPt(Vector2D &centerPt) { this->centerPt = centerPt; }
  void Shape2D::setStartPt(Vector2D &startPt) { this->startPt = startPt; }
}
