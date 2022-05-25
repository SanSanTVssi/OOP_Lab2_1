//
//  Rectengle.cpp
//  OOP_Lab1
//
//  Created by AAI on 30.04.2022.
//

#include "Rectengle.hpp"

Rectangle::Rectangle(float width,float height,float center_x,float center_y)
{
    _maxX = center_x + width / 2;
    _minX = center_x - width / 2;
    _maxY = center_y + height / 2;
    _minY = center_y - height / 2;
    _figurType = ShapeType::Rectangle;
    _id_num = _id++;
}
Rectangle::Rectangle(Rectangle* other,int mode)
{
    this->_maxX = other->_maxX;
    this->_minX = other->_minX;
    this->_maxY = other->_maxY;
    this->_minY = other->_minY;
    this->_figurType = other->_figurType;
    if (mode == 0) { _id_num = _id++; }
    else { this->_id_num = other->_id_num; }
}
