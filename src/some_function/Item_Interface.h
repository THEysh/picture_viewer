

#ifndef ITEM_INTERFACE_H
#define ITEM_INTERFACE_H

#include <QLabel>
#include <QTimer>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QUuid>
#include "QObject"
#include "my_photo_Graphics.h"
#include "memory"
#include "QGraphicsProxyWidget"


class Gif_Rect_Sig;
// 把不同类型显示的图片抽样出来
class Item_Interface : public QObject{
Q_OBJECT
public: //公共接口
    //唯一标识符号
    QUuid uuidSymbol = nullptr;
    QString photo_path = nullptr; //路径
    QTreeWidgetItem* photo_tree_item;
    explicit Item_Interface(const QString &interface_load_path = nullptr, QTreeWidgetItem* interface_load_item= nullptr);
    ~Item_Interface() override;
    virtual void click_element();
    virtual void show_photo(QGraphicsView *view, QGraphicsScene *scene);
    virtual void wheelEvent(QWheelEvent *event, QGraphicsView *view);
    virtual void resizeEvent(QResizeEvent *event, QGraphicsView *view, QGraphicsScene *scene);
    virtual void phot_rotate(bool is_right, QGraphicsView *view);
    virtual void set_z_val(int num);
protected:
    double roller_factor = 1.05;
    QPointF pr_mou;
    virtual void position_calculation(QGraphicsView *view);
};

class C_QPixmapItem : public Item_Interface{
Q_OBJECT
public:
    explicit C_QPixmapItem(const QString &path, const QStringList &imageTypes, QTreeWidgetItem* item);
    ~C_QPixmapItem() override;
    void click_element() override;
    void show_photo(QGraphicsView *view, QGraphicsScene *scene); //传入当前的场景，更新图片大小
    void set_z_val(int num) override;

protected:
    void wheelEvent(QWheelEvent *event,QGraphicsView *view) ;
    void resizeEvent(QResizeEvent *event, QGraphicsView *view, QGraphicsScene *scene) override;
    void phot_rotate(bool is_right, QGraphicsView *view) override;
    void position_calculation(QGraphicsView *view);

private:
    QPixmap or_activated_photo_pixmap = QPixmap(); //原始图片
    std::unique_ptr<QPixmap> photo_pixmap_unique;
    std::unique_ptr<QGraphicsPixmapItem> graphics_pixmapItem_unique;
    QRectF pixmap_rect;
};

class C_SvgItem : public Item_Interface{
Q_OBJECT
public:
    explicit C_SvgItem(const QString &path,QTreeWidgetItem* item);
    ~C_SvgItem() override;
    void click_element() override;
    void show_photo(QGraphicsView *view, QGraphicsScene *scene); //传入当前的场景，更新图片大小
    void set_z_val(int num) override;

protected:
    void wheelEvent(QWheelEvent *event,QGraphicsView *view) override;
    void resizeEvent(QResizeEvent *event, QGraphicsView *view, QGraphicsScene *scene) override;
    void phot_rotate(bool is_right, QGraphicsView *view) override;
    void position_calculation(QGraphicsView *view) override;
private:
    QRectF svg_Rect;
    std::unique_ptr<QSvgRenderer> svgrender_unique = nullptr;
    std::unique_ptr<QGraphicsSvgItem> graphics_svgItem_unique = nullptr;

};

class C_GifItem : public Item_Interface{
Q_OBJECT
public:
    C_GifItem(const QString &path, QTreeWidgetItem* item);
    ~C_GifItem() override;
    void click_element() override;
    void show_photo(QGraphicsView *view, QGraphicsScene *scene); //传入当前的场景，更新图片大小
    void position_calculation(QGraphicsView *view) override;
    void set_z_val(int num) override;
protected:
    void wheelEvent(QWheelEvent *event,QGraphicsView *view) override;
    void resizeEvent(QResizeEvent *event, QGraphicsView *view, QGraphicsScene *scene) override;
    void phot_rotate(bool is_right, QGraphicsView *view) override;

private:
    void _connect(QGraphicsView *view);
    std::unique_ptr<QMovie> au_movie;
    std::unique_ptr<QPixmap> gif_pixmap;
    QPixmap or_pixmap;
    std::unique_ptr<QGraphicsPixmapItem> graphics_gifItem_unique;
    std::unique_ptr<Gif_Rect_Sig> rect_sig;
    QTimer timer;


};
#endif // ITEM_INTERFACE_H

