#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "defwidgetitem.h"
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

    m_pYclFile = new CClothContainer(filePath.toStdString().c_str());
    StTag* pRootNode = m_pYclFile->m_pClothSimObj->m_pStHead;
    PopulateTreeWidget(pRootNode);
}


void MainWindow::on_actionOpen_ycl_file_triggered()
{
    on_OpenFile_clicked();
}

void
MainWindow::PopulateTreeWidget(StTag* pRootNode){
    ui->treeWidget->clear();

    for (const auto child : pRootNode->children)
        appendNodeToView(child, nullptr);

    if (isExpanded)
    {
        ui->treeWidget->expandAll();
    }
    else{
        ui->treeWidget->collapseAll();
    }

}


void
SetNodeText(const StTag* pSourceTag, DefWidgetItem* pTreeItem){
    /* Format Text */
    QString sNodeType = QString::fromStdString(yclutils::GetNodeName(pSourceTag->eType));
    pTreeItem->setText(0,"[TAG 0x" + QString::number(pSourceTag->eType,16) + "] - " + sNodeType);

    if (pSourceTag->eType == /* SIMMESH */ 0x5 || pSourceTag->eType == 0x1B){
        QString text = pTreeItem->text(0);
        text += " (" + QString::fromStdString(pSourceTag->pSimMesh->name)
                + " : "+ QString::fromStdString(pSourceTag->pSimMesh->sObjName) + ")";
        pTreeItem->setText(0,text);
    }
    else if(pSourceTag->eType == /* STRING */ 0x18){
        QString text = pTreeItem->text(0);
        text += " (" + QString::fromStdString(pSourceTag->sTagName) + ")";
        pTreeItem->setText(0,text);
    }
}

void
MainWindow::appendNodeToView(StTag* pSourceTag, DefWidgetItem* pParentItem){

    /* Populate items to tree */
    DefWidgetItem* pTreeItem = (pParentItem) ? new DefWidgetItem(pParentItem) : new DefWidgetItem(ui->treeWidget);
    pTreeItem->setItemTag(pSourceTag);

    /* Format Text */
    SetNodeText(pSourceTag,pTreeItem);

    for (const auto child : pSourceTag->children)
        appendNodeToView(child, pTreeItem);

}

QString intToHexQString(int value) {
    // Convert the integer to a hexadecimal string
    QString hexString = QString::number(value, 16).toUpper();

    // Ensure that the string has exactly two characters by padding with a leading zero if necessary
    if (hexString.length() < 2)
        hexString.prepend('0');

    return hexString;
}


void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    DefWidgetItem* pTreeItem = (DefWidgetItem*)item;
    StTag* pSourceTag = pTreeItem->getItemTag();

    ui->textEdit->clear();
    ui->ItemAddress->clear();
    ui->ItemName->clear();

#ifdef DEBUG_DISP_BINARY
    QString sDataHexString;

    for (int i = 0; i < pSourceTag->data.size(); i++){
        auto byte = pSourceTag->data.at(i);

        if (i % 16 == 0)
            sDataHexString += "\n";

        sDataHexString += " " + intToHexQString(byte);
    }

    ui->textEdit->setText(sDataHexString);
#endif

    ui->ItemName->setText( yclutils::GetNodeName(pSourceTag->eType).c_str() );
    ui->ItemAddress->setText( "0x"+QString::number(pSourceTag->streamPointer-0xC,16).toUpper() );
}




void MainWindow::on_actionExpand_Collapse_triggered()
{
    if (isExpanded){
        ui->treeWidget->collapseAll();
    }
    else{
        ui->treeWidget->expandAll();
    }

    isExpanded = !isExpanded;
}










