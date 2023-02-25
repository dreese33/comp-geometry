#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

// TODO - write another function for calculating pie to precise values
#ifndef M_PI
  # define M_PI           3.14159265358979323846
#endif

/**
 * @brief Lightweight math alternative library (for fun)
 */
namespace Geometry {
  class Angles {
    public:
      /**
       * @brief Standard math function, convert degrees to radians
       *
       * @param angle Angle in degrees
       * @return float Angle in radians
       */
      static float degreeToRadian(float angle) {
        return (M_PI / 180.0f) * angle;
      }
  };
}

#endif