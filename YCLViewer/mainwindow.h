#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>
#include <YukesCloth>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DefWidgetItem;
class QTreeWidgetItem;
class CClothContainer;
class NodeSelectWindow;

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
    void on_actionExit_triggered();
    void on_SaveButton_clicked();
    void on_actionSave_File_triggered();
    void on_addnodebutton_clicked();
    void on_removenodebutton_clicked();
    void ClearSelectionWindow();
    void AddNodeToTreeTag(StTag* newTag);

private:
    Ui::MainWindow *ui;
    void OpenYukesClothFile(const QString& filePath);
    void appendNodeToView(StTag *pSourceTag, DefWidgetItem* pParentItem);
    void UpdateStatLabels();
    void PopulateTreeWidget(StTag* pRootNode);
    void PopulateTableWidget(StTag* pNode);

private:
    QString m_sYclFilePath;
    CClothContainer* m_pYclFile = nullptr;
    NodeSelectWindow* m_pSelectionWindow = nullptr;
    QString m_sExplorerPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // Cache Explorer Directory
    bool isExpanded = false;
};
#endif // MAINWINDOW_H
