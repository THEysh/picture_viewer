//
// Created by top on 2023-04-23.
//
#include <QLabel>
#include <QGuiApplication>
#include <QtGui/QClipboard>
#include <QFileInfo>
#include "My_Photo_Graphics.h"
#include "Item_Interface.h"
#include "qdebug.h"
#include <memory>
#include <QtConcurrent/QtConcurrent>
#include "QMouseEvent"
#include "QMimeData"
#include "My_Qtreewidget.h"
#include "QtMath"

My_Photo_Graphics::My_Photo_Graphics(QWidget *parent):
        // 定义初始化
        QGraphicsView(parent),
        in_tree(nullptr),
        item_queue(1),
        mutex(),
        arload_image(nullptr),
        image_timer(new QTimer),
        scene(new QGraphicsScene()){

    or_background.load(":ui/images/pic_b/wallhaven-nkqrgd.png");
    this->setScene(scene);
    setRenderHint(QPainter::Antialiasing, true);
    // 以鼠标位置的view缩放
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setInteractive(true);
    // 禁用滚轮
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 允许文件的拖动
    setAcceptDrops(true);
    // 设置为拖拽
    this->setDragMode(static_cast<DragMode>(QGraphicsView::ScrollHandDrag | QGraphicsView::RubberBandDrag));
    connect_loadphoto();
    //初始化背景等等
    show_image_item();
}

void My_Photo_Graphics::wheelEvent(QWheelEvent *event) {
    QGraphicsView::wheelEvent(event);
    if (!item_queue.empty()){
        item_queue.at(item_queue_idx)->wheelEvent(event,this);
    } else{
        qDebug()<<"My_Photo_Graphics::wheelEvent";
        return;
    }
}

void My_Photo_Graphics::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    qDebug()<<"My_Photo_Graphics::resizeEvent:->view siez"<<this->size();
    // 更新背景的尺寸
    QRect new_rect(QPoint(0, 0), this->size());
    scene->setSceneRect(new_rect);
    background = or_background.scaled(this->size(), Qt::IgnoreAspectRatio);
    this->setBackgroundBrush(QBrush(background));

    if (item_queue.size()>1){
        qDebug()<<"size大于1是对比模式，不更新图片";
        return;
    }
    // 更新graphics_Item_unique尺寸
    if (!item_queue.empty()){
        item_queue.at(item_queue_idx)->resizeEvent(event,this, scene);
    }
    else{
        qDebug()<<"My_Photo_Graphics::resizeEvent bug!!";
    }
}

void My_Photo_Graphics::graphics_load_image(const QString &path, const QStringList &imageTypes,QTreeWidgetItem* item) {
    if (!is_comparison){
        item_queue.clearn_item_keep_one();
    }
    // 进行判断，是加入什么类型的图片
    QFileInfo fileInfo(path);
    // 创建智能指针
    std::shared_ptr<Item_Interface> temp_unique;
    if (fileInfo.suffix().compare("svg", Qt::CaseInsensitive) == 0){
        qDebug() << "The file is an SVG ,load ...";
        //这是一个使用C++11提供的智能指针模板函数std::make_unique()来创建一个指向C_SvgItem对象的unique_ptr，
        // 其中path是作为构造函数参数传递给C_SvgItem的。std::make_unique()可以用于创建可以管理其生命周期的堆分配
        //对象的std::unique_ptr。与直接使用 new 创建对象相比，使用std::make_unique()可以更加安全和方便.
        temp_unique = std::make_unique<C_SvgItem>(path,item);
        // 加入相关信息
        item_queue.enqueue(temp_unique);
    }
    else if ((fileInfo.suffix().compare("gif", Qt::CaseInsensitive) == 0)){
        qDebug() << "The file is an gif,load ...";
        temp_unique = std::make_unique<C_GifItem>(path,item);
        item_queue.enqueue(temp_unique);
    }
    else if (imageTypes.contains(fileInfo.suffix(), Qt::CaseInsensitive)) {
        qDebug() << "The file is ipg,png...,load ...";
        temp_unique = std::make_unique<C_QPixmapItem>(path,imageTypes,item);
        item_queue.enqueue(temp_unique);
    }
    else {
        qDebug() << "Unsupported image format: " << path;
        return;
    }
    if (!item_queue.empty()){
        qDebug()<<"item_queue_idx size"<<item_queue_idx;
        item_queue_idx = item_queue.size() - 1;
    } else{
        item_queue_idx = 0;
    }
    qDebug()<<"size_item_queue"<<item_queue.size();
    show_image_item();
}

void My_Photo_Graphics::show_image_item() {
    /*初始化背景
    QGraphicsView 组件的背景和场景是两个不同的概念，因此不同的代码片段可能会对它们的大小进行不同的调整。
    前面两行代码设置场景大小与视图大小一致，即将场景矩形设置为 (0, 0, width, height)，其中 width 和 height 分别为视图的宽和高。
    这样可以确保场景的大小与视图相同，从而使所有的图形项都能够完整地显示在视图中。如果不进行这个设置，场景的大小可能会被设置为默认值，
    这可能导致某些图形项被裁剪或者部分显示。*/
    //scene->clear();
    if ((scene!= nullptr)&&(!item_queue.empty())){
        item_queue.at(item_queue_idx)->show_photo(this,scene);
        this->show();
    }
}

My_Photo_Graphics::~My_Photo_Graphics() {
    delete scene;
    delete in_tree;
    qDebug() << "this is : ~My_Photo_Graphics";
}

void My_Photo_Graphics::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);
    QAction *copy_element = menu.addAction("复制");
    QAction *right_rotate = menu.addAction("右旋转90°");
    QAction *left_rotate = menu.addAction("左旋转90°");
    if(item_queue.empty()){
        return;
    }
    //上面的被qt管理，不会内存泄漏
    connect(right_rotate, &QAction::triggered, [this]() {// 旋转图片，例如90度
        item_queue.at(item_queue_idx)->phot_rotate(true,this);
    });
    connect(left_rotate, &QAction::triggered, [this]() {// 旋转图片，例如90度
        item_queue.at(item_queue_idx)->phot_rotate(false,this);
    });
    connect(copy_element,&QAction::triggered, [this]() {

        QString path = item_queue.at(item_queue_idx)->photo_path;
        qDebug()<<"copy_element,"<<path;
        QUrl url = QUrl::fromLocalFile(path);
        // 将 QUrl 对象添加到 QList<QUrl> 中
        QList<QUrl> copyFiles;
        copyFiles.push_back(url);
        // 创建一个 QMimeData 对象，并将 QList<QUrl> 对象设置为它的数据
        auto *mimeData = new QMimeData;
        mimeData->setUrls(copyFiles);
        // 将 QMimeData 对象设置到剪贴板中
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setMimeData(mimeData);
        // 输出成功提示信息
        qDebug() << "The file has been copied to the clipboard.";

    });
    menu.exec(event->globalPos());
}

void My_Photo_Graphics::keyPressEvent(QKeyEvent *event)
{
    // 如果用户按下了 Ctrl+C 键，则执行复制操作
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C) {
        // 执行复制操作
        QString copy_path = item_queue.at(item_queue_idx)->photo_path;
        qDebug()<<"COPYPATH"<<copy_path;
        QUrl url = QUrl::fromLocalFile(copy_path);
        // 将 QUrl 对象添加到 QList<QUrl> 中
        QList<QUrl> copyFiles;
        copyFiles.push_back(url);
        // 创建一个 QMimeData 对象，并将 QList<QUrl> 对象设置为它的数据
        auto *mimeData = new QMimeData;
        mimeData->setUrls(copyFiles);
        // 将 QMimeData 对象设置到剪贴板中
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setMimeData(mimeData);
        // 输出成功提示信息
        qDebug() << "The file has been copied to the clipboard.";
    }

}

void My_Photo_Graphics::dragEnterEvent(QDragEnterEvent *event) {
    QGraphicsView::dragEnterEvent(event);
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
    qDebug()<<"dragEnterEvent";
}

void My_Photo_Graphics::dragMoveEvent(QDragMoveEvent *event) {
    QGraphicsView::dragMoveEvent(event);
    event->acceptProposedAction();
}

void My_Photo_Graphics::dropEvent(QDropEvent *event) {
    QGraphicsView::dropEvent(event);
    //3个drag事件实现了文件的拖动
    // 这里是从图片里拖动
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urls = mimeData->urls();
        for (const QUrl &url : urls) {
            QString filePath = url.toLocalFile();
            qDebug() << "Dropped file path: " << filePath;
            graphics_load_image(filePath,in_tree->imageTypes, nullptr);
            break;
        }
        event->acceptProposedAction();
    } else {
        event->ignore();
    }

    // 这里是左边的节点拖动至view
    if (event->mimeData()->hasFormat("application/x-qtreewidget-values")) {
        QByteArray encodedData = event->mimeData()->data("application/x-qtreewidget-values");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);
        if (!stream.atEnd()) { // 检查数据流是否结束
            while (!stream.atEnd()) {
                QString filetype, filename, path;
                stream >> filetype >> filename >> path;
                qDebug() << "stream1" << filetype;
                qDebug() << "stream2" << filename;
                qDebug() << "stream3" << path;
                if (filetype== "files"){
                    // 在视角加一张图片，开启对比模式。结束后关闭
                    QTreeWidgetItem* act_item = in_tree->hash_item.value(path); // 获取激活的树节点
                    item_queue.max_len = item_queue.max_len + 1;
                    is_comparison = true;
                    graphics_load_image(path, in_tree->imageTypes,act_item);
                    is_comparison = false;
                } else{
                    break;
                }
            }
            event->acceptProposedAction();
        }
    }
    qDebug()<<"dropEvent";
}

void My_Photo_Graphics::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    // 将鼠标事件转换为场景坐标系的位置
    const QPointF scenePos = mapToScene(event->pos());
    // 获取该位置所对应的图形项
    auto *item = scene->itemAt(scenePos, transform());
    if (item) {
        // 可以使用 setZValue() 函数来设置项目的 Z 坐标值，从而控制项目的绘制顺序。Z 坐标值越大的项目会被绘制在 Z 坐标值小的项目的上面。
        // 首先使用 foreach 循环遍历当前场景中所有的项目，并计算它们 Z 坐标值的最大值。然后，我们将需要移动到最顶层的项目的 Z 坐标值设置为 maxZValue + 1
        qDebug()<<"uuid1"<<item->data(0).toString();
        qDebug()<<item->data(0).toString();
        int max_z = 0;
        int old_idx = item_queue_idx;
        for (int i=0; i<item_queue.size(); i++){
            max_z = qMax(max_z,int(item->zValue()));
            auto uuid1 = item->data(0).toString();
            auto uuid2 = item_queue.at(i)->uuidSymbol.toString();
            bool isEqual = (uuid1==uuid2);
            if (isEqual){
                item_queue_idx = i;
                qDebug()<<"-------------------item_queue_idx size"<<item_queue.at(i).get();
                qDebug()<<"-------------------item_queue_idx size"<<item_queue.at(i)->photo_tree_item;
                // 鼠标点击后，下面是让树节点高亮显示
                if(item_queue.at(i)->photo_tree_item!= nullptr){
                    in_tree->active_item = item_queue.at(i)->photo_tree_item;
                }

            }
        }
        if (old_idx!=item_queue_idx){
            // 只有索引改变了，才更新最大值！！
            item_queue.max_z_val = item_queue.max_z_val+1;
            item->setZValue(item_queue.max_z_val);
        }
        qDebug()<<"-------"<<item->zValue();

    } else {
        // 点击了背景
        qDebug() << "Clicked on background.";
    }
    // 传递鼠标事件以继续处理。
    QGraphicsView::mousePressEvent(event);
}

void My_Photo_Graphics::connect_loadphoto() {

}
