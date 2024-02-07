#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class QTreeWidgetItem;
class StTag;
class ClothContainer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_OpenFile_clicked();

    void on_actionOpen_ycl_file_triggered();

private:
    Ui::MainWindow *ui;
    void OpenYukesClothFile(const QString& filePath);
    void appendNodeToView(const StTag* pSourceTag, QTreeWidgetItem* pParentItem);
    void PopulateTreeWidget(StTag* pRootNode);

    QString m_sYclFilePath;
    ClothContainer* m_pYclFile = nullptr;
    QString m_sExplorerPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // Cache Explorer Directory

};
#endif // MAINWINDOW_H
