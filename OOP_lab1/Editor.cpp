
#include "Editor.hpp"

Shape* Editor::Iterator::FindID(std::list<Shape*>* list)
{
    for (auto it: *list)
    {
        if (id == it->Id())
        {
            return it;
        }
        else if (it->Type() == ShapeType::Composite)
        {
            auto temp = (dynamic_cast<ICanContainShapes*>(it))->GetList();
            Shape* maybe_return_value = FindID(&temp);
            if (maybe_return_value == nullptr) {}
            else { return maybe_return_value; }
        }
    }
    return nullptr;
}

std::list<Shape*>::iterator Editor::Iterator::IncrHandle(std::list<Shape*>:: iterator temp_ptr_on_vec,std::list<Shape*>* list)
{
    if(temp_ptr_on_vec == _ptr_on_vec->end())
    {
        ICanContainShapes* Composite_find = Editor::FindCompositeIterator(list, _iter);
        if (Composite_find == nullptr) { std::cout << "end of numeration"; throw ShapeException("incr_iter::incr-end of numeration"); }
        _iter = dynamic_cast<Shape*>(Composite_find);
        _ptr_on_vec = Editor::FindPtrOnVec(_iter, list);
        _vec_iter = std::find(_ptr_on_vec->begin(), _ptr_on_vec->end(), _iter);
        std::list<Shape*>::iterator temp_vec_iter = _vec_iter;
        return IncrHandle(++temp_vec_iter,list);
    }
    else
    {
        return temp_ptr_on_vec;
    }
}

std::list<Shape*>::iterator Editor::Iterator::DecrHandle(std::list<Shape*>::iterator temp_ptr_on_vec, std::list<Shape*>* list)
{
    std::list<Shape*>::iterator temp = temp_ptr_on_vec;
    if (++temp == _ptr_on_vec->begin())
    {
        ICanContainShapes* Composite_find = Editor::FindCompositeIterator(list, _iter);
        if (Composite_find == nullptr) { std::cout << "end of numeration"; throw ShapeException("decr_iter::decr - end of numeration"); }
        _iter = dynamic_cast<Shape*>(Composite_find);
        _ptr_on_vec = Editor::FindPtrOnVec(_iter, list);
        _vec_iter = std::find(_ptr_on_vec->begin(), _ptr_on_vec->end(), _iter);
        std::list<Shape*>::iterator temp_vec_iter = _vec_iter;
        return DecrHandle(--temp_vec_iter, list);
    }
    else
    {
        return temp_ptr_on_vec;
    }

}

Editor::Iterator::Iterator(Shape* first, std::list<Shape*>::iterator vec_iter, std::list<Shape*>* ptr_on_vec)
{
    this->_iter = first;
    this->_vec_iter = vec_iter;
    this->_ptr_on_vec = ptr_on_vec;
    this->id = first->Id();
}

void Editor::Iterator::UpdateIterator(bool flg_chnged_memory_plce_rdcor, std::list<Shape*>* list)
{
    if (flg_chnged_memory_plce_rdcor == 1)
    {
        auto temp = FindID(list);
        if (temp == nullptr) { std::cout << "this figure doesn't exist"; throw ShapeException("error_get_iter_doesn't_exist_figure"); }
        else { _iter = temp; _ptr_on_vec = Editor::FindPtrOnVec(temp, list); _vec_iter = std::find(_ptr_on_vec->begin(), _ptr_on_vec->end(), temp); }
    }
}

Editor::Iterator Editor::Iterator::Incr(std::list<Shape*>* list)
{
    if (_iter->Type() == ShapeType::Composite)
    {
        auto composite = dynamic_cast<ICanContainShapes*>(_iter);
        auto lst = composite->GetList();
        _vec_iter = lst.begin();
        _ptr_on_vec = &(lst);
        _iter = *(lst.begin());
    }
    else
    {
        if (++_vec_iter != _ptr_on_vec->end())
        {
            _iter = *_vec_iter;
        }
        else
        {
            std::list<Shape*>::iterator temp_vec_iter = _vec_iter;
            temp_vec_iter = IncrHandle(temp_vec_iter,list);
            this->_iter = *temp_vec_iter;
            this->_ptr_on_vec = Editor::FindPtrOnVec(_iter, list);
            this->_vec_iter = std::find(_ptr_on_vec->begin(), _ptr_on_vec->end(), _iter);
        }
    }
    this->id = _iter->Id();
    return Iterator(_iter, _vec_iter, _ptr_on_vec);
}

Editor::Iterator Editor::Iterator::Decr(std::list<Shape*>* list)
{
    if (_vec_iter-- != _ptr_on_vec->begin())
    {
        if ((*_vec_iter)->Type() == ShapeType::Composite)
        {
            auto composite = dynamic_cast<ICanContainShapes*>((*_vec_iter))->GetList();
            _vec_iter = --(composite.end());
            _ptr_on_vec = &(composite);
            _iter = *(--(composite.end()));
        }
        else {
            _iter = *_vec_iter;
        }
    }
    else
    {
        std::list<Shape*>::iterator temp_vec_iter = _vec_iter;
        temp_vec_iter = DecrHandle(temp_vec_iter, list);
        this->_iter = *temp_vec_iter;
        this->_ptr_on_vec = Editor::FindPtrOnVec(_iter, list);
        this->_vec_iter = std::find(_ptr_on_vec->begin(), _ptr_on_vec->end(), _iter);
    }
    this->id = _iter->Id();
    return Iterator(_iter, _vec_iter, _ptr_on_vec);
}

Editor::Iterator Editor::CreateCircle(float radius, float center_x, float center_y)
{
    storage.CopyForUndo(*this);
    storage.RemovePreviousVersion();
    Shape* new_circle;
    new_circle = ShapeFabric::NewCircle(radius, center_x, center_y);
    list.push_back(new_circle);
    auto temp = std::find(list.begin(), list.end(), new_circle);
    return Iterator(new_circle,temp,&list);
}

Editor::Iterator Editor::CreateRectangle(float width, float height, float center_x, float center_y)
{
    storage.CopyForUndo(*this);
    storage.RemovePreviousVersion();
    Shape* new_rectangle;
    new_rectangle = ShapeFabric::NewRectangle(width, height, center_x, center_y);
    list.push_back(new_rectangle);
    auto temp = std::find(list.begin(), list.end(), new_rectangle);
    return Iterator(new_rectangle, temp, &list);
}

Editor::Iterator Editor::CreateGroup(Iterator &first, Iterator &second)
{
    first.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);
    second.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);
    storage.CopyForUndo(*this);
    storage.RemovePreviousVersion();
    Shape* new_composite;
    new_composite = ShapeFabric::NewComposite(first._iter, second._iter);
    auto temp_new_composite = dynamic_cast<ICanContainShapes*>(new_composite);
    first._ptr_on_vec->push_back(new_composite);
    auto temp_iter = std::find(first._ptr_on_vec->begin(), first._ptr_on_vec->end(), new_composite);
    auto temp_ptr_on_vec = first._ptr_on_vec;
    second._ptr_on_vec->erase(second._vec_iter);
    first._ptr_on_vec->erase(first._vec_iter);
    auto lst = temp_new_composite->GetList();
    second._ptr_on_vec = &(lst);
    first._ptr_on_vec = &(lst);
    second._vec_iter = std::find(second._ptr_on_vec->begin(), second._ptr_on_vec->end(), second._iter);
    first._vec_iter = std::find(first._ptr_on_vec->begin(), first._ptr_on_vec->end(), first._iter);
    return Iterator(new_composite,temp_iter, temp_ptr_on_vec);
}

Editor::Iterator Editor::ChangeLocation(Iterator &element,Iterator &new_group)
{
    element.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);
    new_group.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);
    storage.CopyForUndo(*this);
    storage.RemovePreviousVersion();
    auto temp = FindCompositeIterator(&list, element._iter);
    if (temp == nullptr) {return ChangeLocation2(element, new_group);}
    auto lst = temp->GetList();
    lst.erase(element._vec_iter);
    auto temp_2 = dynamic_cast<ICanContainShapes*>(new_group._iter);
    auto lst_2 = temp_2->GetList();
    temp_2->add_figure(element._iter);
    element._vec_iter = std::find(lst_2.begin(), lst_2.end(), element._iter);
    element._ptr_on_vec = &lst_2;
    return element;
}

Editor::Iterator Editor::SetLocationOnRoot(Iterator& element)
{
    element.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);
    storage.CopyForUndo(*this);
    storage.RemovePreviousVersion();
    auto temp = FindCompositeIterator(&list, element._iter);
    if (temp == nullptr) { return element;}
    temp->GetList().erase(element._vec_iter);
    this->list.push_back(element._iter);
    element._vec_iter = std::find(this->list.begin(), this->list.end(), element._iter);
    element._ptr_on_vec = &(this->list);
    return element;
}

Editor::Iterator Editor::ChangeLocation(Iterator& element)
{
    element.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);
    storage.CopyForUndo(*this);
    storage.RemovePreviousVersion();
    auto temp = FindCompositeIterator(&list, element._iter);
    if (temp == nullptr) { return element; }
    temp->GetList().erase(element._vec_iter);
    this->list.push_back(element._iter);
    element._vec_iter = std::find(this->list.begin(), this->list.end(), element._iter);
    element._ptr_on_vec = &(this->list);
    return element;
}

Editor::Iterator Editor::FindMoreDeapthFigureHandler(float coord_x, float coord_y)
{
std::list<std::pair<Shape*, int>> enumaration;
int max_depth = -1;
int depth = 0;
Shape* find_figure = nullptr;
for(auto it: list)
    {
        if (coord_x < it->_maxX && coord_x >it->_minX && coord_y < it->_maxY && coord_y >it->_minY)
        {
            enumaration.push_back(std::pair<Shape*, int>(it, depth));
            if (it->_figurType == ShapeType::Composite)
            {
                    std::list<std::pair<Shape*, int>> temp = it->GetStoredShapedHandle(coord_x, coord_y,(depth+1));
                    enumaration.insert(enumaration.end(), temp.begin(), temp.end());
            }
        }
    }
for(auto it = enumaration.begin(); it != enumaration.end(); ++it)
    {
    if(it->second > max_depth)
        {
        find_figure = it->first;
        max_depth = it->second;
        }
    }
if (max_depth == -1)
    {
        std::cout << "no figure on this coord";
        std::list<Shape*>::iterator trash;
        return Iterator(nullptr, trash, nullptr);
    }
    else
    {
    auto temp = FindPtrOnVec(find_figure, &list);
    return Iterator(find_figure, std::find(temp->begin(), temp->end(), find_figure), temp);
    }
}

void Editor::PrintAllFigureInfo()
{

    Fabric(&(this->list),
        [](float x, float y, Shape* it)
        {
            {
            if (it->_figurType == ShapeType::Composite)
                {
                std::cout << "it's new composite" << std::endl;
                }
            else if(it->_figurType == ShapeType::Circle)
                {
                std::cout << "It's circle" << std::endl;
                }
            else
                {
                std::cout << "It's rectangle" << std::endl;
                }
            std::cout << "figure" << std::endl;
            std::cout << "max x = " << it->_maxX << std::endl;
            std::cout << "min x = " << it->_minX << std::endl;
            std::cout << "max y = " << it->_maxY << std::endl;
            std::cout << "min y = " << it->_minY << std::endl;
            }
        },
        0,
        0);
}

void Editor::MoveFigure(float x,float y, Iterator &it)
{
    it.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);
    storage.CopyForUndo(*this);
    storage.RemovePreviousVersion();
    it._iter->_maxX = it._iter->_maxX + x;
    it._iter->_minX = it._iter->_minX + x;
    it._iter->_maxY = it._iter->_maxY + y;
    it._iter->_minY = it._iter->_minY + y;
    if(it._iter->_figurType == ShapeType::Composite)
    {
        auto lst = dynamic_cast<ICanContainShapes*>(it._iter)->GetList();
        Fabric(&lst,
            [](float x, float y, Shape* it)
            {
                it->_maxX = it->_maxX + x;
                it->_minX = it->_minX + x;
                it->_maxY = it->_maxY + y;
                it->_minY = it->_minY + y;
            },
            x,
            y);
    }
    auto temp_list = &list;
    Shape* temp;
    ICanContainShapes* tempComposit;
    Shape* iterator;
    iterator = it._iter;
    while(1)
    {
        tempComposit = FindCompositeIterator(temp_list,iterator);
        Shape* temp = dynamic_cast<Shape*>(tempComposit);
        if(temp == nullptr)
        {
            break;
        }
        else
        {
            if (x > 0 && it._iter->_maxX + x > temp->_maxX) { temp->_maxX = it._iter->_maxX + x; }
            if (x < 0 && it._iter->_minX + x < temp->_minX) { temp->_minX = it._iter->_minX + x; }
            if (y > 0 && it._iter->_maxY + y > temp->_maxY) { temp->_maxY = it._iter->_maxY + y; }
            if (y < 0 && it._iter->_minY + y < temp->_minY) { temp->_minY = it._iter->_minY + y; }
        }
        iterator = temp;
    }
}

void Editor::RemoveFigure(Iterator &figur)
{
    figur.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);
    storage.CopyForUndo(*this);
    storage.RemovePreviousVersion();
    auto composite_with_deleting_fig = FindCompositeIterator(&list, figur._iter);
    if (composite_with_deleting_fig == nullptr)
    {
        this->list.erase(figur._vec_iter);
    }
    else
    {
        composite_with_deleting_fig->GetList().erase(figur._vec_iter);
    }
    if(figur._iter->_figurType == ShapeType::Composite)
    {
        auto lst = dynamic_cast<ICanContainShapes*>(figur._iter)->GetList();
        Fabric(&lst, [](float x, float y, Shape* it) {
            delete it;
            }, 0, 0);

    }
    delete figur._iter;
}

void Editor::FigureInfo(Iterator figur)
{
    figur.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);;
    std::cout << std::endl;
    std::cout << "/////////////";
    if (figur._iter != nullptr)
    {
        if (figur._iter->_figurType == ShapeType::Composite)
        {
            std::cout << "it's new composite" << std::endl;
        }
        else if(figur._iter->_figurType == ShapeType::Circle)
        {
            std::cout << "it's circle" << std::endl;
        }
        else
        {
            std::cout << "it's rectangle" << std::endl;
        }
        std::cout << "figure" << std::endl;
        std::cout << "max x = " << figur._iter->_maxX << std::endl;
        std::cout << "min x = " << figur._iter->_minX << std::endl;
        std::cout << "max y = " << figur._iter->_maxY << std::endl;
        std::cout << "min y = " << figur._iter->_minY << std::endl;
    }
    std::cout << "/////////////";
    std::cout << std::endl;
}

Editor::Iterator Editor::CopyFigur(Iterator &figur)
{
    figur.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);
    storage.CopyForUndo(*this);
    storage.RemovePreviousVersion();
    auto temp = FindCompositeIterator(&(this->list), figur._iter);
    Shape* new_composite = FigureHandle(figur._iter, 0);
    std::list<Shape*>::iterator temp_vec_iter;
    if(temp == nullptr)
    {
        list.push_back(new_composite);
        temp_vec_iter = std::find(this->list.begin(), this->list.end(), new_composite);
    }
    else
    {
        temp->add_figure(new_composite);
        temp_vec_iter = std::find(temp->GetList().begin(), temp->GetList().end(), new_composite);
    }
    return Iterator(new_composite, temp_vec_iter, FindPtrOnVec(new_composite, &list));
}

Editor::Iterator Editor::InctIter(Iterator &iter)
{
    iter.UpdateIterator(_flgChngedMemoryPlaceRdcor, &list);
    return iter.Incr(&(this->list));
}

void Editor::Dispose()
{
    Fabric(&(list), [](float x, float y, Shape* it) {
        delete it;
        }, 0, 0);
    this->list.clear();
}

void Editor::Undo()
{
    std::list<Shape*>* temp = storage.take_undo_version(*this);
    this->Dispose();
    this->list = *temp;
    _flgChngedMemoryPlaceRdcor = 1;
}

void Editor::Redo()
{
    std::list<Shape*>* temp = storage.GetProviousVersion();
    storage.CopyForUndo(*this);
    this->Dispose();
    this->list = *temp;
    _flgChngedMemoryPlaceRdcor = 1;
}

Editor::~Editor()
{
    Fabric(&(list), [](float x, float y, Shape* it) {
        delete it;
        }, 0, 0);
    this->list.clear();
}

void Editor::Fabric(std::list<Shape*>* list, void(*func)(float x,float y,Shape* it), float x, float y)
{
    for(std::list<Shape*>::iterator it = list->begin(); it!=list->end(); ++it)
    {
        if((*it)->_figurType == ShapeType::Composite)
        {
            auto composite_dynamic_cast = dynamic_cast<ICanContainShapes*>(*it);
            auto lst = composite_dynamic_cast->GetList();
            Fabric(&lst, func, x, y);
        }
        func(x, y, *it);
    }
}

Editor::Editor(Editor& original)
{
    for (auto it: original.list)
    {
        Shape* new_figure = FigureHandle(it, 1);
        this->list.push_back(new_figure);
    }
}

std::list<Shape*> Editor::CopyGList(Editor& original)
{
    std::list<Shape*> list_return;
    for (auto it: original.list)
    {
        Shape* new_figure = FigureHandle(it, 1);
        list_return.push_back(new_figure);
    }
    return list_return;
}

std::list<Shape*>* Editor::FindPtrOnVec(Shape* find, std::list<Shape*>* maybe_finded_ptr)
{
    for (std::list<Shape*>::iterator it = maybe_finded_ptr->begin(); it != maybe_finded_ptr->end(); ++it)
    {
        if (find == *it) { return maybe_finded_ptr; }
        else
        {
            if ((*it)->_figurType == ShapeType::Composite)
            {
                auto lst = dynamic_cast<ICanContainShapes*>(*it)->GetList();
                auto temp = FindPtrOnVec(find, &(lst));
                if (temp != nullptr) { return temp; }
                else { continue; }
            }
        }
    }
    return nullptr;
}

ICanContainShapes* Editor::FindCompositeIterator(std::list<Shape*>* list, Shape* find)
{
    for(auto it = list->begin();it!= list->end(); ++it)
    {
        if((*it)->_figurType == ShapeType::Composite)
            {
                auto composite = dynamic_cast<ICanContainShapes*>(*it);
                if (FindFigureInComposite(composite, find)) { return composite; }
                else
                {
                    auto lst = composite->GetList();
                    ICanContainShapes* temp = FindCompositeIterator(&(lst), find);
                    if (temp == nullptr) { continue; }else{ return temp;}
                }
            }
    }
    return nullptr;
}
bool Editor::FindFigureInComposite(ICanContainShapes* composite, Shape* figur)
{
    auto lst = composite->GetList();
    for(auto it: lst)
    {
        if (it == figur) { return true; }
    }
    return false;
}

Editor::Iterator Editor::ChangeLocation2(Iterator &element, Iterator &new_group)
{
    element.UpdateIterator(_flgChngedMemoryPlaceRdcor, &(this->list));
    list.erase(element._vec_iter);
    auto temp_2 = dynamic_cast<ICanContainShapes*>(new_group._iter);
    temp_2->add_figure(element._iter);
    auto lst = temp_2->GetList();
    element._vec_iter = std::find(lst.begin(), lst.end(), element._iter);
    element._ptr_on_vec = &lst;
    return element;
}

Shape* Editor::FigureHandle(Shape* it, int mode) {
    Shape* new_figure;
    if(it->_figurType == ShapeType::Composite)
    {
        new_figure = ShapeFabric::NewComposite(it, mode);
        auto temp_new_composite = dynamic_cast<ICanContainShapes*>(new_figure);
        CopyMethHandler(temp_new_composite, dynamic_cast<ICanContainShapes*>(it), mode);
    }
    else if(it->_figurType == ShapeType::Circle)
    {
        new_figure = ShapeFabric::NewCircle(it, mode);
    }
    else
    {
        new_figure = ShapeFabric::NewRectangle(it, mode);
    }
    return new_figure;
}

void Editor::CopyMethHandler(ICanContainShapes* copy, ICanContainShapes* original, int mode)
{
    for(auto it: original->GetList())
    {
        Shape* new_figure = FigureHandle(it, mode);
        copy->GetList().push_back(new_figure);
    }
}

void Editor::Buffer::CopyForUndo(Editor& copied)
{
    std::list<Shape*>* copy = new std::list<Shape*>(CopyGList(copied));
    copy_undo.push_back(copy);
}

std::list<Shape*>* Editor::Buffer::take_undo_version(Editor& now_version)
{
    std::list<Shape*>* take = copy_undo.back();
    CopyForRedo(now_version);
    copy_undo.pop_back();
    return take;
}

void Editor::Buffer::CopyForRedo(Editor& copied)
{
    std::list<Shape*>* copy = new std::list<Shape*>(CopyGList(copied));
    copy_redo.push_back(copy);
}

void Editor::Buffer::Dispose(std::list<Shape*> list_del)
{
   Editor::Fabric(&(list_del), [](float x, float y, Shape* it)
                  {
                        delete it;
                  }, 0, 0);
    list_del.clear();
}

std::list<Shape*>* Editor::Buffer::GetProviousVersion()
{
    std::list<Shape*>* take = copy_redo.back();
    copy_redo.pop_back();
    return take;

}

void Editor::Buffer::RemovePreviousVersion()
{
    for(auto it = copy_redo.begin();it!=copy_redo.end();++it)
        {
            Dispose(**it);
        }
    copy_redo.clear();
}
