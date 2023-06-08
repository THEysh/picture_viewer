//
// Created by top on 2023-05-17.
//

#include <QFileInfo>
#include <memory>
#include <QMovie>
#include "my_photo_Graphics.h"
#include "Item_Interface.h"
#include "qDebug.h"
#include "Pic_Thread.h"
#include "QMouseEvent"
#include "QPointF"
#include <QVector>

Item_Interface::Item_Interface(const QString &interface_load_path, QTreeWidgetItem* interface_load_item){
    photo_path = interface_load_path;
    photo_tree_item = interface_load_item;
    uuidSymbol = QUuid::createUuid();
}

Item_Interface::~Item_Interface() {

}

void Item_Interface::click_element() {

}

void Item_Interface::show_photo(QGraphicsView *view, QGraphicsScene *scene) {

}

void Item_Interface::wheelEvent(QWheelEvent *event,QGraphicsView *view) {

}

void Item_Interface::resizeEvent(QResizeEvent *event, QGraphicsView *view, QGraphicsScene *scene) {

}

void Item_Interface::phot_rotate(bool is_right,QGraphicsView *view) {

}

void Item_Interface::position_calculation(QGraphicsView *view) {

}

void Item_Interface::set_z_val(int num) {
}

//====================================================================================================

C_QPixmapItem::C_QPixmapItem(const QString &path, const QStringList &imageTypes,QTreeWidgetItem* item):
        Item_Interface(path,item) //初始化父类
{
    or_activated_photo_pixmap.load(path, imageTypes.join(',').toUtf8().constData());
    photo_pixmap_unique = std::make_unique<QPixmap>(or_activated_photo_pixmap);
    if (!or_activated_photo_pixmap.isNull()) {
        qDebug() << "Image loaded successfully.";
        // 创建一个指向 QGraphicsPixmapItem 对象的指针，并将其传递给 C_QPixmapItem 类的构造函数
        graphics_pixmapItem_unique = std::make_unique<QGraphicsPixmapItem>();
        // 创建唯一标识符号
        graphics_pixmapItem_unique->setData(0, uuidSymbol);
        // 给tree的节点做标识符

        graphics_pixmapItem_unique->setPixmap(or_activated_photo_pixmap);
        pixmap_rect = graphics_pixmapItem_unique->boundingRect();
        // 设置为拖拽
        graphics_pixmapItem_unique->setFlags(QGraphicsItem::ItemIsMovable);
    }
    else {
        qDebug() << "Failed to load image from" << path;
    }
}

C_QPixmapItem::~C_QPixmapItem() {
}

void C_QPixmapItem::click_element() {
    Item_Interface::click_element();
}

void C_QPixmapItem::show_photo(QGraphicsView *view, QGraphicsScene *scene) {
    Item_Interface::show_photo(view,scene);
    // 位置更新
    position_calculation(view);
    scene->addItem(graphics_pixmapItem_unique.get());

}

void C_QPixmapItem::position_calculation(QGraphicsView *view) {
    // 位置更新
    int p_width = or_activated_photo_pixmap.width();
    int p_height = or_activated_photo_pixmap.height();
    if ((view->width())>p_width && (view->height())>p_height){
        // 视角大于图片的高, 保持默认
    }else{
        // 自适应缩放
        photo_pixmap_unique = std::make_unique<QPixmap>(or_activated_photo_pixmap.scaled(view->width() ,view->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
    graphics_pixmapItem_unique->setPixmap(*photo_pixmap_unique);
    graphics_pixmapItem_unique->setScale(1);
    pixmap_rect = graphics_pixmapItem_unique->pixmap().rect();
    QPointF center = view ->viewport()->rect().center() - pixmap_rect.center();
    qDebug()<<"view center:"<<view ->viewport()->rect().center()<<"pixmap_rect center"<<pixmap_rect.center();
    qDebug()<<"view rect:"<<view ->rect()<<"pixmap_rect"<<pixmap_rect;
    graphics_pixmapItem_unique->setPos(center);
}

void C_QPixmapItem::wheelEvent(QWheelEvent *event,QGraphicsView *view) {
    Item_Interface::wheelEvent(event,view);
    if (graphics_pixmapItem_unique== nullptr){
        qDebug()<<"C_SvgItem::show_photo bug";
        return;
    }
//    int old_wid = photo_pixmap_unique->width();
//    int old_height = photo_pixmap_unique->height();
//    int new_wid; int new_height;
//
//    if (event->delta() > 0){
//        new_wid = old_wid *roller_factor*roller_factor;
//        new_height = old_height *roller_factor*roller_factor;
//    }else{
//        new_wid = old_wid;
//        new_height = old_height;
//    }
//    if ((new_wid < or_activated_photo_pixmap.width())&&(new_height < or_activated_photo_pixmap.height())){
//        photo_pixmap_unique = std::make_unique<QPixmap>(or_activated_photo_pixmap.scaled(new_wid,new_height,Qt::KeepAspectRatio));
//        graphics_pixmapItem_unique->setPixmap(*photo_pixmap_unique);
//    }
//    else{
//        photo_pixmap_unique = std::make_unique<QPixmap>(or_activated_photo_pixmap);
//        graphics_pixmapItem_unique->setPixmap(*photo_pixmap_unique);
//    }
    // 设置视角为中心
    graphics_pixmapItem_unique->setTransformOriginPoint(photo_pixmap_unique->rect().center());
    if (event->delta() > 0) {
        // 放大，只对pixmapItem场景进行缩放
        graphics_pixmapItem_unique->setScale(graphics_pixmapItem_unique->scale() * roller_factor);
    } else {
        // 缩放
        graphics_pixmapItem_unique->setScale(graphics_pixmapItem_unique->scale() / roller_factor);
    }


}

void C_QPixmapItem::resizeEvent(QResizeEvent *event, QGraphicsView *view, QGraphicsScene *scene) {
    Item_Interface::resizeEvent(event, view, scene);
    position_calculation(view); //位置更新
}

void C_QPixmapItem::phot_rotate(bool is_right,QGraphicsView *view){
    Item_Interface::phot_rotate(is_right,view);
    if (graphics_pixmapItem_unique== nullptr){
        qDebug()<<"C_SvgItem::show_photo bug";
        return;
    }
    QTransform transform;
    if ((graphics_pixmapItem_unique!= nullptr)&&is_right){
        graphics_pixmapItem_unique->setTransform(transform.rotate(90), true); //svg做旋转操作
        // 计算rect信息，并更新
        pixmap_rect = transform.mapRect(pixmap_rect).toRect();
    }
    else if((graphics_pixmapItem_unique!= nullptr)&&!is_right){
        graphics_pixmapItem_unique->setTransform(transform.rotate(-90), true); //svg做旋转操作
        // 计算rect信息，并更新
        pixmap_rect = transform.mapRect(pixmap_rect).toRect();
    } else{ return;}
    position_calculation(view); //位置更新
}

void C_QPixmapItem::set_z_val(int num) {
    Item_Interface::set_z_val(num);
    graphics_pixmapItem_unique->setZValue(num);
}

//====================================================================================================

C_SvgItem::C_SvgItem(const QString &path,QTreeWidgetItem* item):
        Item_Interface(path,item),
        svgrender_unique(new QSvgRenderer()),
        graphics_svgItem_unique(new QGraphicsSvgItem())
{
    bool loadResult = svgrender_unique ->load(path); // 加载 SVG 文件

    if (loadResult){
        // 设置唯一标识符号
        graphics_svgItem_unique->setData(0,uuidSymbol);
        graphics_svgItem_unique->setSharedRenderer(svgrender_unique.get());
        graphics_svgItem_unique->setScale(1);
        svg_Rect = graphics_svgItem_unique->boundingRect(); //用于计算位置信息
        // 设置为拖拽
        graphics_svgItem_unique->setFlags(QGraphicsItem::ItemIsMovable);
    }
    else{
        qDebug()<<"QSvgRenderer load fail";
    }
}

C_SvgItem::~C_SvgItem() {

}

void C_SvgItem::click_element() {
    Item_Interface::click_element();
}

void C_SvgItem::show_photo(QGraphicsView *view, QGraphicsScene *scene) {
    Item_Interface::show_photo(view, scene);
    if (graphics_svgItem_unique== nullptr){
        qDebug()<<"C_SvgItem::show_photo bug";
        return;
    }
    // 计算尺寸，让svgItem保持中心位置,SVG图不窗口自适应计算了，只计算中心位置
    position_calculation(view);
    scene->addItem(graphics_svgItem_unique.get()); //将svg条目添加到场景中
}

void C_SvgItem::position_calculation(QGraphicsView *view) {
    if (graphics_svgItem_unique == nullptr){
        qDebug()<<"C_SvgItem::show_photo bug";
        return;
    }
    QPointF center = view ->viewport()->rect().center() - svg_Rect.center();
    graphics_svgItem_unique->setScale(1);
    qDebug()<<"svgItem center:"<<center;
    graphics_svgItem_unique->setPos(center);
}

void C_SvgItem::wheelEvent(QWheelEvent *event,QGraphicsView *view) {
    Item_Interface::wheelEvent(event,view);
    if (graphics_svgItem_unique == nullptr) {
        qDebug() << "C_SvgItem::show_photo bug";
        return;
    }
//    QPointF temp(0,0);
//    QPointF df(0,0);
//    if (pr_mou!=temp){
//        df = event->posF()-pr_mou;
//    }
//    auto p = graphics_svgItem_unique->mapFromScene(event->posF());
//    auto p2 = graphics_svgItem_unique->mapFromScene(QPointF(0,0));
//    qDebug()<<"event->posF()"<< event->posF();
//    qDebug()<<"df"<< df;
//    qDebug()<<"p1"<< p*graphics_svgItem_unique->scale();
//    qDebug()<<"p2"<< -p2*graphics_svgItem_unique->scale();
//    qDebug()<<"p3"<< event->posF()+p2*graphics_svgItem_unique->scale();
//    graphics_svgItem_unique->setScale(1);
    //graphics_svgItem_unique->setTransformOriginPoint(graphics_svgItem_unique->boundingRect().center());
    // 获取鼠标在视图中的位置
    graphics_svgItem_unique->setTransformOriginPoint(graphics_svgItem_unique->boundingRect().center());
    if (event->delta() > 0) {
        graphics_svgItem_unique->setScale(graphics_svgItem_unique->scale()*roller_factor);
        qDebug() << "graphics_svgItem_unique->scale();" << graphics_svgItem_unique->scale();
    } else {
        graphics_svgItem_unique->setScale(graphics_svgItem_unique->scale()/roller_factor);
        qDebug() << "graphics_svgItem_unique->scale();" << graphics_svgItem_unique->scale();
    }
    event->accept();
}

void C_SvgItem::resizeEvent(QResizeEvent *event, QGraphicsView *view, QGraphicsScene *scene) {
    Item_Interface::resizeEvent(event, view, scene);
    if (graphics_svgItem_unique== nullptr){
        qDebug()<<"C_SvgItem::show_photo bug";
        return;
    }
    position_calculation(view);
}

void C_SvgItem::phot_rotate(bool is_right, QGraphicsView *view) {
    Item_Interface::phot_rotate(is_right,view);
    if (graphics_svgItem_unique== nullptr){
        qDebug()<<"C_SvgItem::show_photo bug";
        return;
    }
    QTransform transform;
    if ((graphics_svgItem_unique != nullptr)&&is_right) { // 检查指针是否为 nullptr

        graphics_svgItem_unique->setTransform(transform.rotate(90), true); //svg做旋转操作
        // 计算rect信息，并更新
        svg_Rect = transform.mapRect(svg_Rect).toRect();

    } else if ((graphics_svgItem_unique != nullptr)&&!is_right){
        graphics_svgItem_unique->setTransform(transform.rotate(-90), true);
        // 计算rect信息，并更新
        svg_Rect = transform.mapRect(svg_Rect).toRect();

    } else{ return;}
    position_calculation(view);
}

void C_SvgItem::set_z_val(int num) {
    Item_Interface::set_z_val(num);
    graphics_svgItem_unique->setZValue(num);
}

//====================================================================================================


C_GifItem::C_GifItem(const QString &path, QTreeWidgetItem* item):
        Item_Interface(path,item)
{
        au_movie = std::make_unique<QMovie>(path);
        au_movie->start();
        gif_pixmap = std::make_unique<QPixmap>(QPixmap::fromImage(au_movie->currentImage()));
        graphics_gifItem_unique = std::make_unique<QGraphicsPixmapItem>();
        //创建唯一标识符号
        graphics_gifItem_unique->setData(0,uuidSymbol);
        graphics_gifItem_unique->setPixmap(*gif_pixmap);
        // 设置为拖拽 ,要在不为nullpter设置拖拽
        graphics_gifItem_unique->setFlags(QGraphicsItem::ItemIsMovable);
        // 计算rect 设置rect变化的信号,
        rect_sig = std::make_unique<Gif_Rect_Sig>(graphics_gifItem_unique->boundingRect().toRect());
}

C_GifItem::~C_GifItem(){
    // 释放内存, 因为智能指针，就是不写，内存也会被释放
}

void C_GifItem::_connect(QGraphicsView *view) {
    if (au_movie == nullptr){
        qDebug()<<"C_GifItem::_connect(),bug";
        return;
    }
    // 开始更新画面
    QObject::connect(&timer, &QTimer::timeout, [this,view](){
        or_pixmap = au_movie->currentPixmap();
        QRect or_rect = or_pixmap.rect();
        if((view->width()>or_rect.width()) && (view->height()>or_rect.height())){
            gif_pixmap = std::make_unique<QPixmap>(or_pixmap);
        } else{
            gif_pixmap = std::make_unique<QPixmap>(or_pixmap.scaled(view->width(),view->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        }
        // 画面更新
        graphics_gifItem_unique->setPixmap(*gif_pixmap);
        // 检查尺寸是否改变, 采用信号判断尺寸是否改变，改变就更新。只有这样写才能同步实现view的拖拽
        rect_sig->checkRect(graphics_gifItem_unique->pixmap().rect());
//        qDebug()<<"view:"<<view->rect();
//        qDebug()<<"graphics_gifItem_unique:"<<graphics_gifItem_unique->boundingRect();
    });
    // 检查尺寸是否变化，变化就更新位置:
    QObject::connect(rect_sig.get(),&Gif_Rect_Sig::sizeChanged,[this,view](const QRect &rect){
        position_calculation(view);
        qDebug()<<"this:connect(rect_sig.get().....,view rect:"<<view->rect();
        qDebug()<<"this:connect(rect_sig.get().....,graphics_gifItem_unique->boundingRect():"<<graphics_gifItem_unique->boundingRect();
    });
    //使用 "this" 关键字引入它的作用域,
    QObject::connect(au_movie.get(), &QMovie::frameChanged, [this](int frameIndex){
        QSize temp_size = au_movie->currentPixmap().size();
//        qDebug() << "Current Frame Index: " << frameIndex;
//        qDebug() << "Current Frame size: " << temp_size;
    });
    connect(au_movie.get(), &QMovie::resized, [](const QSize& size){
        qDebug() << "Animation Resized to " << size;
    });
    connect(au_movie.get(), &QMovie::started, [](){
        qDebug() << "Animation started, Animation started " ;
    });
}

void C_GifItem::show_photo(QGraphicsView *view, QGraphicsScene *scene) {
    Item_Interface::show_photo(view, scene);
    // 更新画面 在写在初始化的连接里
    // 将 QGraphicsPixmapItem 添加到 QGraphicsScene 中
    scene->addItem(graphics_gifItem_unique.get());
    // 连接信号
    _connect(view);
    // 定时器更新 QPixmap
    timer.start(33); // 33ms 即约等于一秒钟的 30 帧

    position_calculation(view);
    scene->addItem(graphics_gifItem_unique.get());

}

void C_GifItem::click_element() {
    Item_Interface::click_element();
}

void C_GifItem::wheelEvent(QWheelEvent *event,QGraphicsView *view) {
    Item_Interface::wheelEvent(event, view);
    if (graphics_gifItem_unique == nullptr){
        qDebug()<<"C_GifItem::wheelEvent bug";
    }
    graphics_gifItem_unique->setTransformOriginPoint(graphics_gifItem_unique->boundingRect().center());
    if (event->delta() > 0) {  // 缩放
        // 只对pixmapItem场景进行缩放
        graphics_gifItem_unique->setScale(graphics_gifItem_unique->scale() * roller_factor);
    } else {  // 放大
        graphics_gifItem_unique->setScale(graphics_gifItem_unique->scale() / roller_factor);
    }
}

void C_GifItem::resizeEvent(QResizeEvent *event, QGraphicsView *view, QGraphicsScene *scene) {
    Item_Interface::resizeEvent(event, view, scene);
    position_calculation(view);
}

void C_GifItem::phot_rotate(bool is_right, QGraphicsView *view) {
    Item_Interface::phot_rotate(is_right, view);
    if (graphics_gifItem_unique == nullptr){
        qDebug()<<"C_GifItem::wheelEvent bug";
    }
    QTransform transform;
    qDebug()<<"gif_rect1:"<<rect_sig->get_gif_rect();
    if ((graphics_gifItem_unique != nullptr)&&is_right) { // 检查指针是否为 nullptr
        graphics_gifItem_unique->setTransform(transform.rotate(90), true);
        // 计算rect信息，并更新
        QRect temp_r = transform.mapRect(rect_sig->get_gif_rect());
        rect_sig->set_gif_rect(temp_r);
        qDebug()<<"gif_rect2:"<<rect_sig->get_gif_rect();
    } else if ((graphics_gifItem_unique != nullptr)&&!is_right){
        // 计算rect信息，并更新
        graphics_gifItem_unique->setTransform(transform.rotate(-90), true);
        // 计算rect信息，并更新
        QRect temp_r = transform.mapRect(rect_sig->get_gif_rect());
        rect_sig->set_gif_rect(temp_r);
    } else{ return;}
    position_calculation(view);
}

void C_GifItem::position_calculation(QGraphicsView *view) {
    Item_Interface::position_calculation(view);
    if (graphics_gifItem_unique == nullptr){
        qDebug()<<"C_GifItem::wheelEvent bug";
    }

    graphics_gifItem_unique->setScale(1);
    QPointF center = (view ->viewport()->rect().center()) - (rect_sig->get_gif_rect().center());
    qDebug()<<"view center:"<<view ->viewport()->rect().center()<<"pixmap_rect center"<< rect_sig->get_gif_rect().center();
    qDebug()<<"view rect:"<<view ->rect()<<"pixmap_rect"<< rect_sig->get_gif_rect();
    qDebug()<<"center:"<<center;
    graphics_gifItem_unique->setPos(center);
}

void C_GifItem::set_z_val(int num) {
    Item_Interface::set_z_val(num);
    graphics_gifItem_unique->setZValue(num);
}


