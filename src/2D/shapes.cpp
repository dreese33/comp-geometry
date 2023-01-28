#include <iostream>
#include "shapes.hpp"

namespace Shapes {
   /**
   * @brief Calculate the vertex's coordinates based on the start point, which begins at the bottom of the circle
   * bounding the polygon to be drawn. In this configuration, we will draw a non-rotated polygon, with a line along
   * the x-axis at the bottom.
   *
   * @param vertex Starting at the bottom right, vertex to calculate coordinates for (counterclockwise)
   * @return Vector vertex coordinates
   */
  Vector2D *Polygon::calculatePolygonVertex(unsigned int vertex) {
    Vector2D *polygonVertex = new Vector2D();
    polygonVertex->vector[0] = this->centerPt.vector[0]
      + this->radius*cos(270 + this->omega/2 + vertex*this->omega);
    polygonVertex->vector[1] = this->centerPt.vector[1]
      + this->radius*sin(270 + this->omega/2 + vertex*this->omega);
    std::cout << this->omega << " " << vertex << " " << polygonVertex->vector[0] << " " << polygonVertex->vector[1] << std::endl;
    std::cout << this->radius*cos(270+this->omega/2 + vertex*this->omega) << std::endl;
    return polygonVertex;
  }

  /**
   * @brief Calculates vertex positions for polygon with n sides
   */
  void Polygon::calculateVertices() {
    this->vertices = new Vector2D[this->numberOfSides];
    // this->vertices[1].vector[0] = 0.5f;
    // this->vertices[1].vector[1] = 0.5f;

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
