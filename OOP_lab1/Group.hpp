//
//  Composit.hpp
//  OOP_Lab1
//
//  Created by AAI on 30.04.2022.
//

#ifndef Composit_hpp
#define Composit_hpp

#include "Shape.hpp"

class Group : public ICanContainShapes
{
public:
    std::list<Shape*> list;
    void find_new_coord(Shape * figu);
    Group(Group* Copy, int mode);
    Group(Shape* first,Shape*second);
    
    void add_figure(Shape* first) override
    {
        list.push_back(first);
        find_new_coord(first);
    }
    
    std::list<std::pair<Shape*, int>> GetStoredShapedHandle(float coord_x, float coord_y, int depth);
    std::list<Shape*> GetList() override { return list; }
private:
    friend class Editor;
    friend class iterator;
};


#endif /* Composit_hpp */
