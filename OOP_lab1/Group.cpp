//
//  Composit.cpp
//  OOP_Lab1
//
//  Created by AAI on 30.04.2022.
//

#include "Group.hpp"

void Group::find_new_coord(Shape * figu)
{
    if (this->_maxX < figu->_maxX) { this->_maxX = figu->_maxX;}
    if (this->_maxY < figu->_maxY) { this->_maxY = figu->_maxY; }
    if (this->_minX > figu->_minX) { this->_minX = figu->_minX; }
    if (this->_minY > figu->_minY) { this->_minY = figu->_minY; }
}

Group::Group(Group* Copy, int mode)
{
    this->_maxX = Copy->_maxX;
    this->_minX = Copy->_minX;
    this->_maxY = Copy->_maxY;
    this->_minY = Copy->_minY;
    this->_figurType = Copy->_figurType;
    if (mode == 0) { _id_num = _id++; }
    else{this->_id_num = Copy->_id_num;}
}

Group::Group(Shape* first,Shape*second)
{
    list.push_back(first);
    list.push_back(second);
    _figurType = ShapeType::Composite;
    this->_maxX = first->_maxX;
    this->_minX = first->_minX;
    this->_maxY = first->_maxY;
    this->_minY = first->_minY;
    find_new_coord(second);
    _id_num = _id++;
}

std::list<std::pair<Shape*, int>> Group::GetStoredShapedHandle(float coord_x, float coord_y, int depth)
{
    std::list<std::pair<Shape*, int>> enumaration;
    for(auto it = list.begin();it != list.end();++it)
    {
        if (coord_x < (*it)->_maxX && coord_x >(*it)->_minX && coord_y < (*it)->_maxY && coord_y >(*it)->_minY)
        {
            enumaration.push_back(std::pair<Shape*, int>(*it, depth));
            if ((*it)->_figurType == ShapeType::Composite)
            {
                auto composite_dynamic_cast = dynamic_cast<Group*>(*it);
                std::list<std::pair<Shape*, int>> temp = composite_dynamic_cast->GetStoredShapedHandle(coord_x, coord_y, (depth+1));
                enumaration.insert(enumaration.end(), temp.begin(), temp.end());
            }
        }
    }
    return enumaration;
}
