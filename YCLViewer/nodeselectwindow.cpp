#include "nodeselectwindow.h"
#include "ui_nodeselectwindow.h"
#include <YukesCloth>

NodeSelectWindow::NodeSelectWindow(CSimObj *pSimObject, StTag* pTargetTag, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NodeSelectWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Add New Node");
    this->pSimObj = pSimObject;
    this->m_pParentTag = pTargetTag;

    this->setAttribute(Qt::WA_DeleteOnClose);
    NodeSelectWindow::SetupListWidget();
}

NodeSelectWindow::~NodeSelectWindow()
{
    emit interfaceClose();
    delete pSimObj;
    delete ui;
}


QString
GetNodeName(const StTag* pSourceTag){
    QString nodeText = QString::fromStdString(yclutils::GetNodeName(pSourceTag->eType));
    nodeText = "[TAG 0x" + QString::number(pSourceTag->eType,16) + "] - " + nodeText;

    if (pSourceTag->eType == /* SIMMESH */ 0x5 || pSourceTag->eType == 0x1B){
        nodeText += " (" + QString::fromStdString(pSourceTag->pSimMesh->sModelName)
                + " : "+ QString::fromStdString(pSourceTag->pSimMesh->sObjName) + ")";
    }
    else if(pSourceTag->eType == /* STRING */ 0x18){
        nodeText += " (" + QString::fromStdString(pSourceTag->sTagName) + ")";
    }
    return nodeText;
}


void
NodeSelectWindow::SetupListWidget(){
    ui->listWidget->clear();
    m_tagPalette = pSimObj->m_pStHead->children;

    for (int i = 0; i < m_tagPalette.size(); i++)
    {
        StTag* pTag = m_tagPalette[i];
        QString listStr = QString::number(i) + ": " + GetNodeName(pTag);
        ui->listWidget->addItem(listStr);
    }
}

void NodeSelectWindow::on_buttonBox_rejected()
{
    this->close();
}


void NodeSelectWindow::on_buttonBox_accepted()
{
    QListWidgetItem* listItem = ui->listWidget->currentItem();
    int nodeIdx = listItem->text().split(":").first().toInt();

    StTag* sourceTag = m_tagPalette.at(nodeIdx);
    emit addNodeToItem(sourceTag);

    /* Clear MEM!! */
    this->close();
}



