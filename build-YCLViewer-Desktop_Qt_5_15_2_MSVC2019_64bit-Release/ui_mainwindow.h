/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_ycl_file;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *OpenFile;
    QSpacerItem *horizontalSpacer;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEdit;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: rgb(20, 30, 40);\n"
"	color: rgb(255,255,255);\n"
"}\n"
"\n"
"QFrame{\n"
"	border: 2px solid white;\n"
"	border-radius: 2px;\n"
"	border-color: rgb(49, 68, 101);\n"
"	alternate-background-color: rgba(31, 46, 74,70);\n"
"}\n"
"\n"
"QScrollBar:vertical{\n"
"	width:  12px;\n"
"	background-color: rgb(31, 24, 38);\n"
"	margin: 0px 0px 0px 0px;\n"
"}\n"
"QScrollBar:handle{\n"
"	min-height: 45px;\n"
"	border-radius: 4px;\n"
"	background-color: rgb(40, 59, 93);\n"
"}\n"
"QScrollBar:arrow{\n"
"	border-radius: 4px;\n"
"	background-color: rgb(50, 69, 103);\n"
"}\n"
"QScrollBar:handle:hover{\n"
"	border-radius: 4px;\n"
"	background-color: rgb(60, 79, 113);\n"
"}\n"
"QScrollBar::sub-page:vertical {\n"
"    background-color: rgb(20, 31, 50)\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical {\n"
"    background-color: rgb(20, 31, 50)\n"
"}\n"
"\n"
"\n"
"QProgressBar{ \n"
"	color: rgba(255,255,255,200);\n"
"	border: solid grey;\n"
"	border-top-right-radius: 12px;\n"
"	border-top-left-radius: 12px;\n"
""
                        "	border-bottom-right-radius: 12px;\n"
"	border-bottom-left-radius: 12px;\n"
"	background-color: rgb(15, 25, 45);\n"
"}\n"
"QProgressBar::chunk {\n"
"	background-color:\n"
"	qlineargradient(\n"
"	spread:pad, x1:0, y1:1, x2:1, y2:0,\n"
"	stop:0 rgba(80, 120, 150, 100), \n"
"	stop:1 rgba(80, 120, 150, 225));\n"
"\n"
"	border-top-right-radius: 5px;\n"
"	border-top-left-radius: 5px;\n"
"	border-bottom-right-radius: 5px;\n"
"	border-bottom-left-radius: 5px;\n"
"}\n"
"\n"
"\n"
"QTreeView{\n"
"	border: 2px solid white;\n"
"	background-color: rgb(20, 31, 50);\n"
"	border-radius: 2px;\n"
"	border-color: rgb(49, 68, 101);\n"
"	alternate-background-color: rgba(31, 46, 74,70);\n"
"}\n"
"\n"
"QTreeView:item:hover{\n"
"	alternate-background-color: rgb(255, 255, 127);\n"
"	background-color: rgb(57, 85, 136);\n"
"}\n"
"\n"
"QTreeView:item:selected{\n"
"	alternate-background-color: rgb(255, 255, 127);\n"
"	background-color: rgb(87, 105, 166);\n"
"}\n"
"\n"
"QTreeView:item:pressed{\n"
"	background-color:rgba(255, 255, 255,255);\n"
""
                        "	alternate-background-color: rgba(255, 255, 255,255);\n"
"}\n"
"\n"
"QPushButton{\n"
"	color: rgb(255, 255, 255);\n"
"	background-color:rgb(72, 90, 125);\n"
"	border: 0px;\n"
"	border-radius: 2px;\n"
"}\n"
"QPushButton:hover{\n"
"	background-color:rgb(80, 99, 133);\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color:rgb(100, 120, 140);\n"
"}\n"
"\n"
"\n"
"\n"
""));
        actionOpen_ycl_file = new QAction(MainWindow);
        actionOpen_ycl_file->setObjectName(QString::fromUtf8("actionOpen_ycl_file"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setMinimumSize(QSize(0, 50));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        OpenFile = new QPushButton(frame_2);
        OpenFile->setObjectName(QString::fromUtf8("OpenFile"));
        OpenFile->setMinimumSize(QSize(98, 25));

        horizontalLayout_2->addWidget(OpenFile);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addWidget(frame_2);

        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeWidget = new QTreeWidget(frame);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setAlternatingRowColors(true);
        treeWidget->header()->setVisible(false);

        horizontalLayout->addWidget(treeWidget);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        textEdit = new QTextEdit(widget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setBold(false);
        font.setItalic(false);
        textEdit->setFont(font);
        textEdit->setLineWrapColumnOrWidth(0);
        textEdit->setReadOnly(true);

        verticalLayout_2->addWidget(textEdit);

        tableWidget = new QTableWidget(widget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout_2->addWidget(tableWidget);


        horizontalLayout->addWidget(widget);


        verticalLayout->addWidget(frame);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_ycl_file);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen_ycl_file->setText(QCoreApplication::translate("MainWindow", "Open .ycl file", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen_ycl_file->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        OpenFile->setText(QCoreApplication::translate("MainWindow", "Open File", nullptr));
        textEdit->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
