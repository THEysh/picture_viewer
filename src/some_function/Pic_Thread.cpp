//
// Created by top on 2023-05-24.
//

#include "Pic_Thread.h"
#include "Item_Interface.h"


// ==========================================================================

Item_Interface_Queue::Item_Interface_Queue(int load_max):
    max_len(load_max){
}

void Item_Interface_Queue::enqueue(const std::shared_ptr<Item_Interface>& data) {
    if (item_data.size() < max_len) {
        item_data.enqueue(data);
    } else{
        // 使用临时智能指针变量保存队首元素,用于删除，离开作用域，临时智能指针变量销毁，同时会自动释放管理的对象
        std::shared_ptr<Item_Interface> temp_ptr = item_data.dequeue();
        item_data.enqueue(data);
    }
    // 新加入的z轴值为它的之前的最大值加1
    max_z_val = max_z_val+1;
    data->set_z_val(max_z_val);
    qDebug()<<"size,Item_Interface_Queue::enqueue:"<<item_data.size();
}

void Item_Interface_Queue:: clearn_item_keep_one(){
    max_len = 1;
    while (item_data.size()>1){
        // 使用临时智能指针变量保存队首元素,用于删除，离开作用域，临时智能指针变量销毁，同时会自动释放管理的对象
        std::shared_ptr<Item_Interface> temp_ptr = item_data.dequeue();
    }
}

bool Item_Interface_Queue::empty() const {
    return item_data.isEmpty();
}

size_t Item_Interface_Queue::size() const {
    return item_data.size();
}

std::shared_ptr<Item_Interface> Item_Interface_Queue::at(int idx){
    if (idx>item_data.size()){
        return item_data.back();
    } else{
        return item_data.at(idx);
    }
}

// =====================================================================================

Gif_Rect_Sig::Gif_Rect_Sig(const QRect &rect){
    gif_rect = rect;
}

void Gif_Rect_Sig::checkRect(const QRect &rect){
    if ((rect.size().width() != gif_rect.size().width())||(rect.size().height() != gif_rect.size().height())) {
        gif_rect = rect;
        emit sizeChanged(rect);
    }
}

QRect Gif_Rect_Sig::get_gif_rect(){
    return gif_rect;
}

void Gif_Rect_Sig::set_gif_rect(const QRect &rect){
    gif_rect = rect;
}


//=================================================

Load_Image_Intf::Load_Image_Intf(QMutex &mutex) :mutex(mutex){}


//=============================================================================
Load_Pixmap::Load_Pixmap(QMutex &mutex, const QString &path, const QStringList &imageTypes) :
        Load_Image_Intf(mutex),
        path(path),
        imageTypes(imageTypes){}


void Load_Pixmap::thredrun() {
    qInfo() << "Task started!";
//        mutex.lock();
//        Item_Interface *temp_unique;
//        temp_unique = new C_QPixmapItem(path,imageTypes);
//        mutex.unlock();
//        emit end_of_loading(temp_unique);
    qInfo() << "Task finished!";
}
