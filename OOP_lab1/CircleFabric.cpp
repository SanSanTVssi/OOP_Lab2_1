//
//  CircleFabric.cpp
//  OOP_Lab1
//
//  Created by AAI on 01.05.2022.
//

#include "Circle.hpp"

Shape* ShapeFabric::NewCircle(float radius, float center_x, float center_y) {
    return new Circle(radius, center_x, center_y);
}

Shape* ShapeFabric::NewCircle(Shape* first, int mode) {
    return new Circle(dynamic_cast<Circle*>(first), mode);
}
