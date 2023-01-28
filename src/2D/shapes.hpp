#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <cmath>
#include "../graphics/graphics.hpp"

namespace Shapes {
  enum ShapeType {
    POLYGON
  };

  enum ShapeDrawingStyle {
    VERTEX_SHAPE, LINE_SHAPE
  };

  class Shape2D {
    public:
      // getters
      float getRadius();
      unsigned int getNumberOfSides();
      float getSideLen();
      Vector2D getCenterPt();
      Vector2D getStartPt();
      Vector2D *getVertices();

      // setters
      void setRadius(float radius);
      void setNumberOfSides(unsigned int numberOfSides);

      // not all shapes will have a center point
      void setCenterPt(Vector2D &centerPt);
      void setStartPt(Vector2D &startPt);

    protected:
      float omega;
      float radius;
      float sideLen;
      unsigned int numberOfSides;

      Vector2D centerPt;
      Vector2D startPt;
      Vector2D *vertices;

      // other functions
      virtual void calculateVertices() {};
  };

  class Polygon: public Shape2D {
    public:
      Vector2D *calculatePolygonVertex(unsigned int vertex);
      virtual void calculateVertices();
      Polygon(ShapeDrawingStyle drawingStyle);
    private:
      ShapeDrawingStyle drawingStyle;
  };

  class ShapeFactory {
    public:
      ShapeFactory() {};
      ~ShapeFactory() {};

      /**
       * @brief Shape constructor from shape factory pattern
       *
       * @param type Type of shape to be drawn
       * @param drawingStyle Style of shape to be drawn
       * @return Shape2D object
       */
      static Shape2D *constructShape(ShapeType type, ShapeDrawingStyle drawingStyle) {
        switch (type) {
          case POLYGON:
            return new Polygon(drawingStyle);
        }
      }
  };
}

#endif