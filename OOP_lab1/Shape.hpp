#define FIGURE_HPP
#ifdef FIGURE_HPP

#include <iostream>
#include <list>
#include <string>

enum class ShapeType {
  Composite = 0,
  Circle = 1,
  Rectangle = 2
};

class Shape
{
public:
    int Id() { return _id_num; }
    
    ShapeType Type() { return _figurType; }
    
    virtual ~Shape() = default;
protected:
    float _maxX;
    float _minX;
    float _maxY;
    float _minY;
    int _id_num;
    static int _id;
    ShapeType _figurType;
    
    virtual std::list<std::pair<Shape*, int>> GetStoredShapedHandle(float coord_x, float coord_y, int depth) = 0;
    
private:
    friend class Group;
    friend class Editor;
};


class ICanContainShapes : public Shape {
public:
    virtual std::list<Shape*> GetList() = 0;
    virtual void add_figure(Shape* first) = 0;
};

class ShapeFabric {
public:
    static Shape* NewCircle(float radius, float center_x, float center_y);
    static Shape* NewCircle(Shape* first, int mode);
    
    static Shape* NewRectangle(float width, float height, float center_x, float center_y);
    static Shape* NewRectangle(Shape* first, int mode);
    
    static Shape* NewComposite(Shape* first, Shape*second);
    static Shape* NewComposite(Shape* first, int mode);
};

class ShapeException : public std::runtime_error
{
public:
    ShapeException(std::string ex) : std::runtime_error(ex) { }
};


#endif // FIGURE_HPP
