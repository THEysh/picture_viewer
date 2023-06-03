//
// Created by top on 2023-05-24.
//

#ifndef PICTURE_VIEWER_PIC_THREAD_H
#define PICTURE_VIEWER_PIC_THREAD_H
#include <QThread>
#include <QGraphicsView>
#include <QMutex>
#include <QQueue>
#include <QRunnable>
#include "memory"
#include "qdebug.h"
class  Item_Interface;

// =====================================================================
class Load_Image_Intf : public QObject{
Q_OBJECT
public:
    explicit Load_Image_Intf(QMutex &mutex);
    virtual void thredrun(){}
    signals:
    //这是信号，不是函数
    void end_of_loading(Item_Interface *unique);

protected:
    QMutex &mutex;
    int id_ = 0;

};
// =====================================================================

class Load_Pixmap : public Load_Image_Intf {
public:
    const QString path;
    const QStringList imageTypes;
    explicit Load_Pixmap(QMutex &mutex,const QString &path, const QStringList &imageTypes);
    void thredrun() override;
};

// =====================================================================

class Item_Interface_Queue:public QObject {
Q_OBJECT
public:
    explicit Item_Interface_Queue(int max_len=2);
public:
    int max_z_val=0; //永远保存最大的y值
    int max_len;
    QQueue< std::shared_ptr<Item_Interface> > item_data; // 存储数据的队列
    // 入队函数
    void enqueue(const std::shared_ptr<Item_Interface>& data);
    //清空到只剩下1个
    void clearn_item_keep_one();
    // 判断队列是否为空
    bool empty() const;
    // 获取队列中元素数量
    size_t size() const;
    std::shared_ptr<Item_Interface> at(int idx);
};

// =====================================================================

class Gif_Rect_Sig : public QObject
{
Q_OBJECT
public:
    Gif_Rect_Sig(const QRect &rect);
signals:
    //信号不用实例化
    void sizeChanged(const QRect &rect);

public slots:
    void checkRect(const QRect &rect);

public:
    void set_gif_rect(const QRect &rect);
    QRect get_gif_rect();
private:
    QRect gif_rect;
};

#endif //PICTURE_VIEWER_PIC_THREAD_H
