#ifndef MY_QTREEWIDGET_H
#define MY_QTREEWIDGET_H
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>
#include <QFileSystemWatcher>
#include <QFuture>
#include "My_Photo_Graphics.h"

class My_Qtreewidget : public QTreeWidget {
Q_OBJECT
signals:
public:
    explicit My_Qtreewidget(QWidget *name);
    ~My_Qtreewidget() override;
    void connect_photo(My_Photo_Graphics* name);
    bool _is_type(const QString &name, const QStringList &strlist);
    QString ProjectDir;
    My_Photo_Graphics* my_photo;
    QTreeWidgetItem* active_item;
    QFileSystemWatcher* my_watcher;
    QStringList imageTypes;
    QMap<QString,QTreeWidgetItem*> hash_item;
    QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const override;
    void _updata_all_Qtree_dir();
    void on_itemClicked(QTreeWidgetItem *item);
private slots:
    void on_itemExpanded(QTreeWidgetItem *item);
    void on_itemCollapsed(QTreeWidgetItem *item);
    void on_itemDoubleClicked(QTreeWidgetItem *item);
    void on_fileChanged(const QString &filedPath);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private:
    const int MAX_NODE_COUNT;
    int nodeCount;
    QTreeWidgetItem* rootNode;
    QLineEdit *lineEdit = nullptr;

    QFuture<void> ar_future;
    void _updata_one_item(QTreeWidgetItem* item, const QString& path);
    void _dir_connect();
    void _add_a_layerDirs(QTreeWidgetItem *parentNode);
    void delete_roots(QTreeWidgetItem *parentNode);
    void keyPressEvent(QKeyEvent *event) override;
    void _updata_treewidgetItem(bool is_next);
    void contextMenuEvent(QContextMenuEvent *event) override;
    QString _rag(const QString &qString);


};

#endif // MY_QTREEWIDGET_H
