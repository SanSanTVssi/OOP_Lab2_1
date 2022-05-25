//
//  CompositFabric.cpp
//  OOP_Lab1
//
//  Created by AAI on 01.05.2022.
//

#include "Group.hpp"

Shape* ShapeFabric::NewComposite(Shape* first, Shape*second) {
    return new Group(first, second);
}

Shape* ShapeFabric::NewComposite(Shape* first, int mode) {
    return new Group(dynamic_cast<Group*>(first), mode);
}
