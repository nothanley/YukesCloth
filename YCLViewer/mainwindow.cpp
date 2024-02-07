#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <YukesCloth>
#pragma once

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::on_OpenFile_clicked()
{
    this->m_sYclFilePath = QFileDialog::getOpenFileName(this, tr("Open .ycl file"),
                                                         this->m_sExplorerPath,
                                                         tr("Animation Definitions (*.ycl)") );
    if (m_sYclFilePath == "") return;
    OpenYukesClothFile(m_sYclFilePath);
}

void
MainWindow::OpenYukesClothFile(const QString& filePath){

    m_pYclFile = new ClothContainer(filePath.toStdString().c_str());
    StTag* pRootNode = m_pYclFile->m_pClothSimObj->m_pStHead;
    PopulateTreeWidget(pRootNode);
}


void
MainWindow::PopulateTreeWidget(StTag* pRootNode){
    ui->treeWidget->clear();

    /* Populate items to tree */
    QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
    pRootItem->setText(0,"[TAG] eType: " + QString::number(pRootNode->eType));

    for (const auto child : pRootNode->children)
        appendNodeToView(child, nullptr);
}

void
MainWindow::appendNodeToView(const StTag* pSourceTag, QTreeWidgetItem* pParentItem){

    /* Populate items to tree */
    QTreeWidgetItem* pTreeItem = (pParentItem) ? new QTreeWidgetItem(pParentItem) : new QTreeWidgetItem(ui->treeWidget);
    QString sNodeType = QString::fromStdString(yclutils::GetNodeName(pSourceTag->eType));
    pTreeItem->setText(0,"[TAG 0x" + QString::number(pSourceTag->eType,16) + "] - " + sNodeType);

    if (pSourceTag->eType == /* SIMMESH */ 0x5){
        QString text = pTreeItem->text(0);
        text += " (" + QString::fromStdString(pSourceTag->pSimMesh->modelName)
              + " : "+ QString::fromStdString(pSourceTag->pSimMesh->sObjName) + ")";
        pTreeItem->setText(0,text);
    }
    else if(pSourceTag->eType == /* STRING */ 0x18){
        QString text = pTreeItem->text(0);
        text += " (" + QString::fromStdString(pSourceTag->sTagName) + ")";
        pTreeItem->setText(0,text);
    }


    for (const auto child : pSourceTag->children)
        appendNodeToView(child, pTreeItem);

}

void MainWindow::on_actionOpen_ycl_file_triggered()
{
    on_OpenFile_clicked();
}

