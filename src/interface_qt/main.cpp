#include <QtGui/QApplication>
#include "mainwindow.h"
#include <GL/glut.h>
#include "model.h"
#include "objprocessor.h"

Model m;
std::map <std::string, Material > material_map;
std::vector<std::string> errors;
std::string DATA_DIR = "../../models/";
std::string HOME = "../../";
bool update_model;

int main(int argc, char *argv[])
{
    update_model = false;
    glutInit(&argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
