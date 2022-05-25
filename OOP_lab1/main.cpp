//
//  main.cpp
//  OOP_Lab1
//
//  Created by AAI on 30.04.2022.
//

#include <iostream>
#include "Editor.hpp"

int main()
{
    Editor Area;
    auto circleFigure = Area.CreateCircle(1,2,0);
    auto rectangleFigure = Area.CreateRectangle(1, 2, 4, 3);
    Editor::Iterator groupFigure = Area.CreateGroup(circleFigure, rectangleFigure);
    Area.Undo();
    auto circle_2 = Area.CreateCircle(4, 3, 2);
    auto rectangle_2 = Area.CreateRectangle(1, 2, 3, 1);
    auto group_2 = Area.CreateGroup(circle_2, rectangle_2);
    auto circle_2_1_ = Area.CreateCircle(1, 4, 0);
    Area.MoveFigure(4, 3, circleFigure);
    Area.Undo();
    Area.ChangeLocation(circle_2_1_, group_2);
    Area.Undo();
    Area.SetLocationOnRoot(circle_2_1_);
    Area.CreateCircle(3, 2, 1);
    Area.Undo();
    Area.Redo();
    Area.Undo();
    Area.Redo();
    Area.FigureInfo(Area.FindMoreDeapthFigureHandler(3, 2));
    Area.PrintAllFigureInfo();
    Editor::Iterator copy_global = Area.CopyFigur(group_2);
    Editor::Iterator circle_3 = Area.CreateCircle(1, 2, 3);
    Editor::Iterator rectangle_3 = Area.CreateRectangle(1, 2, 3, 4);
    Editor::Iterator rectangle_3_1_ = Area.CreateRectangle(5, 6, 7, 8);
    Editor::Iterator group_3 = Area.CreateGroup(circle_3, rectangle_3);
    Area.ChangeLocation(rectangle_3_1_, group_3);
    Area.ChangeLocation(group_2, group_3);
    Area.CopyFigur(group_2);
    Area.FigureInfo(Area.FindMoreDeapthFigureHandler(0,3));
    Area.PrintAllFigureInfo();
    Editor::Iterator circle_last = Area.CreateCircle(2, 1, 9);
    while(true)
    {
        try {
            Area.FigureInfo(Area.InctIter(circleFigure));
        }
        catch (ShapeException ex) {
            std::cout << ex.what() << std::endl;
        }
    }
    return 0;
}
