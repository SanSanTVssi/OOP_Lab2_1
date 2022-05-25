//
//  Circle.hpp
//  OOP_Lab1
//
//  Created by AAI on 30.04.2022.
//

#ifndef Circle_hpp
#define Circle_hpp

#include "Shape.hpp"

class Circle : public Shape
{
public:
    Circle(float radius,float center_x,float center_y);
    Circle(Circle* other, int mode);
private:
    friend class Editor;
    std::list<std::pair<Shape*, int>> GetStoredShapedHandle(float coord_x, float coord_y, int depth)
    { throw ShapeException("get_into_composite_false Circle"); };
};

#endif /* Circle_hpp */
