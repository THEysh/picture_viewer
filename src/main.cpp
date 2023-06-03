#include <QString>
#include <iostream>
#include <QResource>
#include <QApplication>
#include <QMainWindow>
#include <QDirIterator>
#include "qdebug.h"
#include "ui/ui_file/Qtree_ui.h"
#include "function.h"

void Resource_Registration(){
    QString currentPath = QDir::currentPath();
    // 输出路径
    qDebug() << "Current path: " << currentPath;
    QString path_rcc = QString(currentPath + "/my_qrc.rcc"); //获取项目的.rcc路径
    qDebug() << "path_rcc: " << path_rcc;

    if (QResource::registerResource(path_rcc)) { //获取项目的资源文件
        qDebug() << "Resource file registered successfully!";
    } else {
        qWarning() << "Failed to register resource file，try find currentPath ";
    }

}

bool Generate_qrc(){

    QString Project_path = PROJECT_ROOT_DIR ; // 项目路径
    QString resourcePath = Project_path + "/src/ui/images/";
    QString qrcFileName = Project_path + "/src/my_qrc.qrc";// 生成的qrc文件名

    QFile qrcFile(qrcFileName);
    if(!qrcFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"Failed to create "<<qrcFileName<<" !";
        return false;
    }

    QTextStream out(&qrcFile);
    out << "<RCC>\n";
    out << "<qresource prefix=\"/\">\n";

    QDirIterator it(resourcePath, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        if (!it.fileInfo().isDir())
        {
            QString filePath = it.filePath();
            QString relativePath = filePath.right(filePath.length() - resourcePath.length());
            out << "<file>ui/images/" << relativePath << "</file>\n";
        }
    }
    out << "</qresource>\n";
    out << "</RCC>\n";

    qrcFile.close();
    return true;
}

int main(int argc, char *argv[])
{
    system("chcp 65001"); //用于解决中文乱码
    QApplication app(argc, argv);
    // 获取命令行参数
    QStringList args = app.arguments();
    // QApplication::arguments() 用于获取命令行参数。你可以在应用程序启动时获取命令行参数并判断其中是否有文件名或路径，
    // 如果有，则说明用户可能是通过拖放文件到应用程序图标来启动的，此时你可以获取到这个文件的绝对路径，然后在程序中进行处理。
    QString openpath = "";
    if (args.count()>1){
        // 获取需要打开的目录
        openpath = args.at(1);
    }
    // 注册资源文件（包括图片的rcc文件，这是由qrc转换出来的）,返回路径,已经在cmake添加了add_executable(.qrc)的路径就可以不注册
    Resource_Registration();
    Generate_qrc(); // 生成一个.qrc的脚本
    auto *QWidget = new QMainWindow();
    Ui_Qtree_Class_UI my_ui{};
    Inherit_UI my_class(my_ui,QWidget,openpath);
    QWidget->show();

    return QApplication::exec();
}