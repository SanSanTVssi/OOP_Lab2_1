//
//  RectangleFabric.cpp
//  OOP_Lab1
//
//  Created by AAI on 01.05.2022.
//

#include "Rectengle.hpp"

Shape* ShapeFabric::NewRectangle(float width, float height, float center_x, float center_y) {
    return new Rectangle(width, height, center_x, center_y);
}

Shape* ShapeFabric::NewRectangle(Shape* first, int mode) {
    return new Rectangle(dynamic_cast<Rectangle*>(first), mode);
}

