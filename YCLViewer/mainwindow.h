#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DefWidgetItem;
class QTreeWidgetItem;
class StTag;
class CClothContainer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_OpenFile_clicked();

    void on_actionOpen_ycl_file_triggered();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_actionExpand_Collapse_triggered();

private:
    Ui::MainWindow *ui;
    void OpenYukesClothFile(const QString& filePath);
    void appendNodeToView(StTag *pSourceTag, DefWidgetItem* pParentItem);
    void PopulateTreeWidget(StTag* pRootNode);

    QString m_sYclFilePath;
    CClothContainer* m_pYclFile = nullptr;
    QString m_sExplorerPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // Cache Explorer Directory
    bool isExpanded = false;
};
#endif // MAINWINDOW_H
