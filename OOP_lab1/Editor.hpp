//
//  Redactor.hpp
//  OOP_Lab1
//
//  Created by AAI on 30.04.2022.
//

#ifndef Redactor_hpp
#define Redactor_hpp

#include "Shape.hpp"
#include <stdexcept>

class Editor
{
public:
    class Iterator
    {
    private:
        friend class Editor;
        Shape* _iter;
        std::list<Shape*>::iterator _vec_iter;
        std::list<Shape*>* _ptr_on_vec;
        int id;
        
        Shape* FindID(std::list<Shape*>* list);
        std::list<Shape*>::iterator IncrHandle(std::list<Shape*>:: iterator temp_ptr_on_vec,std::list<Shape*>* list);
        std::list<Shape*>::iterator DecrHandle(std::list<Shape*>::iterator temp_ptr_on_vec, std::list<Shape*>* list);
        
    public:
        Iterator(Shape* first, std::list<Shape*>::iterator vec_iter, std::list<Shape*>* ptr_on_vec);
        void UpdateIterator(bool flg_chnged_memory_plce_rdcor, std::list<Shape*>* list);
        Iterator Incr(std::list<Shape*>* list);
        Iterator Decr(std::list<Shape*>* list);
    };
    
    Editor() { _flgChngedMemoryPlaceRdcor = false; }
    Iterator CreateCircle(float radius, float center_x, float center_y);
    Iterator CreateRectangle(float width, float height, float center_x, float center_y);
    Iterator CreateGroup(Iterator &first, Iterator &second);
    Iterator ChangeLocation(Iterator &element,Iterator &new_group);
    Iterator SetLocationOnRoot(Iterator& element);
    Iterator ChangeLocation(Iterator& element);
    Iterator FindMoreDeapthFigureHandler(float coord_x, float coord_y);
    void PrintAllFigureInfo();
    void MoveFigure(float x,float y, Iterator &it);
    void RemoveFigure(Iterator &figur);
    void FigureInfo(Iterator figur);
    Iterator CopyFigur(Iterator &figur);
    Iterator InctIter(Iterator &iter);
    void Dispose();
    void Undo();
    void Redo();
    ~Editor();
    bool howFlgChngeMemoryPlceRdcor() { return _flgChngedMemoryPlaceRdcor;}
    
private:
    static void Fabric(std::list<Shape*>* list, void(*func)(float x,float y,Shape* it), float x, float y);
    Editor(Editor& original);
    static std::list<Shape*> CopyGList(Editor& original);
    
    friend class iterator;
    std::list<Shape*> list;
    
    struct Buffer
    {
        std::list<std::list<Shape*>*> copy_undo;
        std::list<std::list<Shape*>*> copy_redo;
        void CopyForUndo(Editor& copied);
        std::list<Shape*>* take_undo_version(Editor& now_version);
        void CopyForRedo(Editor& copied);
        void Dispose(std::list<Shape*> list_del);
        std::list<Shape*>* GetProviousVersion();
        void RemovePreviousVersion();
    } storage;
    bool _flgChngedMemoryPlaceRdcor;
    
    static std::list<Shape*>* FindPtrOnVec(Shape* find, std::list<Shape*>* maybe_finded_ptr);
    static ICanContainShapes* FindCompositeIterator(std::list<Shape*>* list, Shape* find);
    static bool FindFigureInComposite(ICanContainShapes* composite, Shape* figur);
    Iterator ChangeLocation2(Iterator &element, Iterator &new_group);
    static Shape* FigureHandle(Shape* it, int mode);
    static void CopyMethHandler(ICanContainShapes* copy, ICanContainShapes* original, int mode);
};


#endif /* Redactor_hpp */
