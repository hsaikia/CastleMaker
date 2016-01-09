#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"
#include "model_processor.h"
#include "objprocessor.h"
#include "syntaxhighlighter.h"
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QFile>


extern Model m;
extern bool update_model;
extern std::map <std::string, Material > material_map;
extern std::vector<std::string> errors;
float model_size;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    highlighter = new SyntaxHighlighter(ui->textEdit->document());
    err = new QDialog;
    err->setModal(true);
    err->setWindowTitle("There were errors in the rule file!");
    layout = new QHBoxLayout;
    text = new QLabel;
    layout->addWidget(text);
    err->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Quit_triggered()
{
        this->close();
}

void MainWindow::on_pushButton_released()
{
    QFile file("rule.file");
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        out.flush();
        file.close();
    }
    Model new_m;
    material_map.clear();
    readMtl("default.mtl", true);
    errors.clear();
    parseFile("rule.file", new_m);
    //printf("Errors : %ld\n", errors.size());
    if(errors.size() > 0){
        new_m.faces.clear();
        std::string all;
        for(size_t i = 0; i < errors.size(); i++){
            all += errors[i];
            all += "\n";
        }
        QString str = QString::fromStdString(all);
        text->setText(str);
        err->show();
    }

    BBox scope;
    getBBox(new_m, scope);
    translate(new_m, -(scope.max.x + scope.min.x)/2.0, -(scope.max.x + scope.min.x)/2.0, -(scope.max.x + scope.min.x)/2.0);
    model_size = std::max(std::max(scope.max.y - scope.min.y, scope.max.x - scope.min.x), scope.max.z - scope.min.z);
    //scale(new_m, 1.0/model_size, 1.0/model_size, 1.0/model_size);
    //scaleTextures(new_m);
    sortFacesByMaterial(new_m);
    m = new_m;
    update_model = true;
}
