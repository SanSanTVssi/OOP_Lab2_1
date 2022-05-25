//
//  Rectengle.hpp
//  OOP_Lab1
//
//  Created by AAI on 30.04.2022.
//

#ifndef Rectengle_hpp
#define Rectengle_hpp

#include "Shape.hpp"

class Rectangle : public Shape
{
public:
    Rectangle(float width,float height,float center_x,float center_y);
    Rectangle(Rectangle* other,int mode);
private:
    friend class Editor;
    std::list<std::pair<Shape*, int>> GetStoredShapedHandle(float coord_x, float coord_y, int depth)
    { throw ShapeException("get_into_composite_false Rectangle"); };
};

#endif /* Rectengle_hpp */
