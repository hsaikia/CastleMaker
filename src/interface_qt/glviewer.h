#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QGLWidget>
#include <QTimer>
#include <QWheelEvent>
#include <QMouseEvent>
#include "model.h"
#include "objprocessor.h"

class GLViewer : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLViewer(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:
     int xRot;
     int yRot;
     int zRot;
     QPoint lastPos;
     QTimer timer;
     GLuint model_list;
     GLuint temp_list;
     std::vector <GLuint> texture;
     void buildlists();
     void loadGLTextures();
     void wheelEvent(QWheelEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void setXRotation(int angle);
     void setYRotation(int angle);
     void setZRotation(int angle);
};

#endif // GLVIEWER_H
