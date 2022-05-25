//
//  Circle.cpp
//  OOP_Lab1
//
//  Created by AAI on 30.04.2022.
//

#include "Circle.hpp"

Circle::Circle(float radius,float center_x,float center_y)
{
    _maxX = center_x + radius;
    _minX = center_x - radius;
    _maxY = center_y + radius;
    _minY = center_y - radius;
    _figurType = ShapeType::Circle;
    _id_num = _id++;
}

Circle::Circle(Circle* other, int mode)
{
    this->_maxX = other->_maxX;
    this->_minX = other->_minX;
    this->_maxY = other->_maxY;
    this->_minY = other->_minY;
    this->_figurType = other->_figurType;
    if (mode == 0) { _id_num = _id++; }
    else { this->_id_num = other->_id_num; }
}
