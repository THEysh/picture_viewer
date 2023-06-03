//
// some_function.h
//
#ifndef QT_EXAM_FUNCTION_H
#define QT_EXAM_FUNCTION_H


#include <QWidget>
#include <QMainWindow>
#include <QSplitter>
#include "ui/ui_file/Qtree_ui.h"

using namespace  std;

class Inherit_UI : public QWidget{
Q_OBJECT
signals:

public slots:
public:
    Inherit_UI(Ui_Qtree_Class_UI &ui_f, QMainWindow *Win,const QString &load_path):info(load_path){
        this->Win = Win;
        ui_f.setupUi(Win);
        this->ui_f = ui_f;
        photo_g = ui_f.graphicsView;//这样是实现，ui_f.photo_label 和 这个类中的photo_label共用一个地址
        qtree_widget = ui_f.treeWidget_1;
        qtree_widget->connect_photo(photo_g); //初始化My_Photo_Graphics和My_Qtreewidget之间的连接
        connect_all();
        Splitter();
        detect_user_directory();
    }
private:
    QMainWindow *Win;
    Ui_Qtree_Class_UI ui_f;
    My_Qtreewidget *qtree_widget = nullptr;
    My_Photo_Graphics *photo_g = nullptr;
    QFileInfo info;
private:
    void connect_all(){
        Win->setWindowTitle("image");
        QIcon icon(":ui/images/pic_2d/images-solid.svg");
        Win->setWindowIcon(icon);
    }
    //设置窗口之间的拖动
    void Splitter(){
        // 将树形控件和 photo_g 作为子控件添加到 QSplitter 中
        QSplitter* splitter = new QSplitter(Qt::Horizontal);
        splitter->addWidget(ui_f.verticalWidget);
        splitter->addWidget(photo_g);
        //设置分隔条的宽度
        splitter->setHandleWidth(1);
        // 使左侧 QWidget 的大小占比为 1，右侧 QWidget 的大小占比为 3
        splitter->setStretchFactor(1, 3);
        // 添加
        ui_f.horizontalLayout_1->addWidget(splitter);

    }

    void detect_user_directory(){
        // 检测这个路径是不是图片或者一个目录。是一个目录就打开这个目录的节点，是一个图片就打开图片，并定位到目录的节点
        if (info.isDir()){
            qDebug()<<"info.isDir()"<<info.filePath();
            qtree_widget->ProjectDir = info.filePath();
            qtree_widget->_updata_all_Qtree_dir();
        }
        else if (info.isFile()){
            qDebug()<<"info.isFile()"<<info.filePath();
            // 获取文件所在的目录
            QDir dir = info.dir();
            qDebug() << "File-dir：" << dir.path();
            qtree_widget->ProjectDir = dir.path();
            qtree_widget->_updata_all_Qtree_dir();
            auto tempitem = qtree_widget->hash_item.value(info.filePath());
            while (tempitem == nullptr)
            {
                QEventLoop loop;
                QTimer::singleShot(100, &loop, SLOT(quit())); // 100ms 后退出事件循环
                loop.exec();
                tempitem = qtree_widget->hash_item.value(info.filePath());
            }
            qDebug()<<tempitem;
            qtree_widget->on_itemClicked(tempitem);
        } else{
            qDebug()<<"none"<<info.filePath();
        }
    }
};


#endif //QT_EXAM_FUNCTION_H