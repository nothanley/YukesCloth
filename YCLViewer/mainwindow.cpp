#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "defwidgetitem.h"
#include "nodeselectwindow.h"
#include <YukesCloth>
#include <Cloth/SimMesh.h>
#pragma once

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("YCL Viewer v0.9.10");
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
                                                         tr("Yukes ClothSim Container (*.ycl)") );
    if (m_sYclFilePath == "") return;
    OpenYukesClothFile(m_sYclFilePath);
}

void
GetNodeCount(const uint32_t& target, StTag* pNode, uint32_t& totalNodes ){
    if (pNode->eType == target)
        totalNodes++;

    for (auto& child : pNode->children){
        GetNodeCount(target, child, totalNodes);
    }
}

void
MainWindow::UpdateStatLabels(){

    uint32_t numMeshes = 0;
    uint32_t numLines = 0;
    uint32_t numCols = 0;

    StTag* pRootNode = m_pYclFile->m_pClothSimObj->m_pStHead;
    GetNodeCount(enTagType_SimMesh, pRootNode, numMeshes);
    GetNodeCount(enTagType_SimLine, pRootNode, numLines);
    GetNodeCount(enTagType_Capsule_Standard, pRootNode, numCols);
    GetNodeCount(enTagType_Capsule_Tapered, pRootNode, numCols);


    ui->nodeLabel->setText(QString::number(pRootNode->children.size()) + " node(s)");
    ui->pathLabel->setText(m_sYclFilePath.split("/").last());
    ui->meshTotalLabel->setText("SimMesh Total: " + QString::number(numMeshes));
    ui->lineTotalLabel->setText("SimLine Total: " + QString::number(numLines));
    ui->colTotalLabel->setText("Collision Total: " + QString::number(numCols));
}

void
MainWindow::OpenYukesClothFile(const QString& filePath){

    m_pYclFile = new CClothContainer(filePath.toStdString().c_str());
    StTag* pRootNode = m_pYclFile->m_pClothSimObj->m_pStHead;
    PopulateTreeWidget(pRootNode);
    UpdateStatLabels();
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
        text += " (" + QString::fromStdString(pSourceTag->pSimMesh->sModelName)
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

void
AppendTableItem(QTableWidget* pTableWidget, const QString& text, int colIdx = 0){
    int numRows = pTableWidget->rowCount();
    pTableWidget->setRowCount(numRows+1);
    pTableWidget->setItem(numRows,colIdx,new QTableWidgetItem(text));
}

void
AppendTableItem(QTableWidget* pTableWidget, const uint32_t& value, int colIdx = 0){
    int numRows = pTableWidget->rowCount();
    pTableWidget->setRowCount(numRows+1);
    pTableWidget->setItem(numRows,colIdx,new QTableWidgetItem(QString::number(value)));
}

void
ClearTable(QTableWidget* pTable){
    pTable->clear();
    pTable->setColumnCount(0);
    pTable->setRowCount(0);
}

void removeElementAtIndex(std::vector<StTag*>& vec, size_t index) {
    if (index < vec.size()) { // Ensure the index is valid
        vec.erase(vec.begin() + index); // Remove the element at the specified index
    }
}

int
GetParentChildNodeIndex(StTag* pSourceTag){
    if (!pSourceTag->pParent) return -1;
    std::vector<StTag*> pTags = pSourceTag->pParent->children;
    for (int i = 0; i < pTags.size(); i++)
    {
        StTag* node = pTags.at(i);

        if (node->eType         == pSourceTag->eType &&
            node->sSize         == pSourceTag->sSize &&
            node->sTagName      == pSourceTag->sTagName &&
            node->streamPointer == pSourceTag->streamPointer)
        {
            return i;
        }
    }
    return -1;
}
void
MainWindow::PopulateTableWidget(StTag* pNode){
    ClearTable(ui->tableWidget);
    QStringList headers = {"Tag Type", "Tag Size","Tag Address","Children"};
    ui->tableWidget->setColumnCount(1);

    AppendTableItem(ui->tableWidget, pNode->eType);
    AppendTableItem(ui->tableWidget, pNode->sSize);
    AppendTableItem(ui->tableWidget, pNode->streamPointer);
    AppendTableItem(ui->tableWidget, pNode->children.size());

    ui->tableWidget->verticalHeader()->setVisible(true);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->setVerticalHeaderLabels(headers);

    /* Display mesh data */
    if (pNode->pSimMesh){
        StSimMesh* pSimMesh = pNode->pSimMesh;
        QStringList meshHeaders = {"Mesh Name","Mesh Vertex Count","Sim Vertex Count","Constraints", "Is Sim Line", "Nodes"};

        AppendTableItem(ui->tableWidget, QString::fromStdString(pSimMesh->sObjName));
        AppendTableItem(ui->tableWidget, pSimMesh->sObjVerts.size());
        AppendTableItem(ui->tableWidget, pSimMesh->sSimVtxCount);
        AppendTableItem(ui->tableWidget, pSimMesh->constraints.size());
        AppendTableItem(ui->tableWidget, pSimMesh->bIsSimLine);
        AppendTableItem(ui->tableWidget, pSimMesh->nodePalette.size());

        ui->tableWidget->setVerticalHeaderLabels(headers+meshHeaders);
    }
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    DefWidgetItem* pTreeItem = (DefWidgetItem*)item;
    StTag* pSourceTag = pTreeItem->getItemTag();

    ui->ItemAddress->clear();
    ui->ItemName->clear();
    ui->ItemName->setText( yclutils::GetNodeName(pSourceTag->eType).c_str() );
    ui->ItemAddress->setText( "0x"+QString::number(pSourceTag->streamPointer-0xC,16).toUpper() );
    PopulateTableWidget(pSourceTag);
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


void MainWindow::on_actionExit_triggered()
{
    QApplication::exit(0);
}


void MainWindow::on_SaveButton_clicked()
{
    if (!this->m_pYclFile){
        QMessageBox::warning(this,"File Error", "Please load .ycl to save contents.");
        return; }

    if (m_pYclFile->m_pClothSimObj->m_pStHead->children.size() == 0){
        QMessageBox::warning(this,"File Error", "YCL is missing a tag hierarchy.");
        return;}

    QString savePath = QFileDialog::getSaveFileName(this, tr("Save .ycl file as"),
                                                         this->m_sYclFilePath,
                                                         tr("Yukes ClothSim Container (*.ycl)") );
    if (savePath == "") return;
    if (!savePath.endsWith(".ycl"))
        savePath += ".ycl";

    CClothSave::SaveToDisk(savePath.toStdString().c_str(), m_pYclFile->m_pClothSimObj);
    QMessageBox::warning(this,"Save Complete", "File saved to: " + savePath);
}


void MainWindow::on_actionSave_File_triggered()
{
    on_SaveButton_clicked();
}

void
MainWindow::ClearSelectionWindow()
{
    this->m_pSelectionWindow = nullptr;
    qDebug() << "Window Closed!";
}

void AddStringToVector(std::vector<std::string>& vec,
                          const std::string& target) {
    for (const auto& str : vec)
        if (str == target) return;

    vec.push_back(target);
}

int
FindNodeIndex(const SimNode& sTarget, std::vector<SimNode> nodeTable)
{
    for (int i = 0; i < nodeTable.size(); i++) {
        SimNode nodeItem = nodeTable.at(i);

        if (sTarget.name == nodeItem.name &&
            sTarget.vecf == nodeItem.vecf) {
            return i;
        }
    }

    return -1;
}

void
MainWindow::AddNodeToTreeTag(StTag* newTag){
    qDebug() << "\nAdding node: " << newTag->sTagName.c_str() << " to root";
    CSimObj* pSimObj = m_pYclFile->m_pClothSimObj;
    if (!newTag->pSimMesh) return;

    /* Append to node palette if doesn't exists */
    StSimMesh* pSimMesh = newTag->pSimMesh;

    for (auto& node : pSimMesh->skin.nodePalette)
    {
        int nodeIdx = FindNodeIndex(node,pSimObj->m_NodeTable);
        if (nodeIdx == -1)
            pSimObj->m_NodeTable.push_back(node);
    }

    /* Append mesh names to string table */
    AddStringToVector(pSimObj->m_sStringTable,newTag->pSimMesh->sModelName);
    AddStringToVector(pSimObj->m_sStringTable,newTag->pSimMesh->sObjName);

    /* Add node to parent children */
    std::vector<StTag*>& children = pSimObj->m_pStHead->children;
    children.insert(children.begin(), newTag);

    /* Reload Hierarchy */
    PopulateTreeWidget(m_pYclFile->m_pClothSimObj->m_pStHead);
    UpdateStatLabels();
}

void MainWindow::on_addnodebutton_clicked()
{
    if (this->m_pSelectionWindow || !m_pYclFile) return;
    StTag* pUserTag = m_pYclFile->m_pClothSimObj->m_pStHead;
    if (!pUserTag) return;

    /* Get source file path */
    QString sourceFile = QFileDialog::getOpenFileName(this, tr("Open .ycl file"),
                                                         this->m_sExplorerPath,
                                                         tr("Yukes ClothSim Container (*.ycl)") );
    if (sourceFile == "") return;

    CClothContainer sourceYCL(sourceFile.toStdString().c_str());
    this->m_pSelectionWindow = new NodeSelectWindow(sourceYCL.m_pClothSimObj,pUserTag,this);
    m_pSelectionWindow->show();

    /* Connect close signals in this & child dialog */
    QObject::connect(m_pSelectionWindow, &NodeSelectWindow::interfaceClose,
                        this, &MainWindow::ClearSelectionWindow);

    /*  Connect signals in this & child dialog */
    QObject::connect(m_pSelectionWindow, &NodeSelectWindow::addNodeToItem,
                        this, &MainWindow::AddNodeToTreeTag);
}


void MainWindow::on_removenodebutton_clicked()
{
    QTreeWidgetItem* pTreeItem = ui->treeWidget->currentItem();
    DefWidgetItem* pDefItem = (DefWidgetItem*)pTreeItem;

    /* Evaluate parent index */
    if (!pDefItem) return;
    StTag* pSourceTag = pDefItem->getItemTag();
    int nodeIdx = GetParentChildNodeIndex(pSourceTag);

    /* Remove Element */
    if (nodeIdx == -1) return;
    removeElementAtIndex(pSourceTag->pParent->children,nodeIdx);
    delete pSourceTag; /* todo: delete all children */

    /* Reload Hierarchy */
    PopulateTreeWidget(m_pYclFile->m_pClothSimObj->m_pStHead);
    UpdateStatLabels();
}


















