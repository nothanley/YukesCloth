#include <QDialog>
#include <YukesCloth>
#pragma once

//class CSimObj;
//class StTag;

namespace Ui {
    class NodeSelectWindow;
}

class NodeSelectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NodeSelectWindow(CSimObj* pSimObject, StTag* pTargetTag, QWidget *parent = nullptr);
    ~NodeSelectWindow();

Q_SIGNALS:
    void interfaceClose();
    void addNodeToItem(StTag* newTag);

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();

private:
    void SetupListWidget();

    Ui::NodeSelectWindow *ui;
    CSimObj* pSimObj = nullptr;
    StTag* m_pParentTag = nullptr;
    std::vector<StTag*> m_tagPalette;
};

