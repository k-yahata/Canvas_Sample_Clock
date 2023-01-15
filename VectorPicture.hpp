#ifndef __VECTOR_PICTURE_HPP__
#define __VECTOR_PICTURE_HPP__

#include "ColoredPolygon.hpp"
#include <vector>

class VectorPicture{

    public:
    std::vector<ColoredPolygon2D> p;

    public:
    void addColoredPolygon(ColoredPolygon2D p);

};

#endif