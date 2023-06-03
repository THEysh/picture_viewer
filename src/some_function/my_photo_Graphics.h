//
// Created by top on 2023-04-23.
//

#ifndef MY_PHOTO_GRAPHICS_H
#define MY_PHOTO_GRAPHICS_H
#include "QObject"
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include "QMenu"
#include "Pic_Thread.h"
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <memory>
#include <QThreadPool>
#include <QTimer>

class My_Qtreewidget;
class Item_Interface;

class My_Photo_Graphics : public QGraphicsView {
Q_OBJECT
public:
    explicit My_Photo_Graphics(QWidget *parent);
    ~My_Photo_Graphics() override;
    QGraphicsScene *scene = nullptr;
    void graphics_load_image(const QString &path, const QStringList &type_img,QTreeWidgetItem* item);
    My_Qtreewidget *in_tree;
protected:
    QPixmap or_background;
    QPixmap background;
    void show_image_item();
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void connect_loadphoto();
    Item_Interface_Queue item_queue; //存放智能指针的队列（所有图片的指向）
    int item_queue_idx = 0;
    bool is_comparison = false; //对比模式
    // 创建并启动后台线程
    QTimer image_timer;
    QThread thread;
    QMutex mutex;
    std::shared_ptr<Load_Image_Intf> arload_image;
};

#endif // MY_PHOTO_GRAPHICS_H


