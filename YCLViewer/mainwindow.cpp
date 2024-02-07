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
    QTreeWidgetItem* pRootItem = new QTreeWidgetItem(ui->treeWidget);
    pRootItem->setText(0,"[TAG] eType: " + QString::number(pRootNode->eType));

    for (const auto child : pRootNode->children)
        appendNodeToView(child, pRootItem);
}

void
MainWindow::appendNodeToView(const StTag* pSourceTag, QTreeWidgetItem* pParentItem){

    /* Populate items to tree */
    QTreeWidgetItem* pTreeItem = new QTreeWidgetItem(pParentItem);
    QString sNodeType = QString::fromStdString(yclutils::GetNodeName(pSourceTag->eType));
    pTreeItem->setText(0,"[TAG - " + QString::number(pSourceTag->eType) + "] - " + sNodeType);

    for (const auto child : pSourceTag->children)
        appendNodeToView(child, pTreeItem);

}











