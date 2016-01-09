#include "glviewer.h"
#include <GL/glut.h>

extern Model m;
extern bool update_model;
extern std::map <std::string, Material > material_map;
std::map<std::string, int> texture_map;
extern std::string DATA_DIR;

float zoom = -1.0f;
float rot = 0.0f;
float vertical_pan = 0.0f;
extern float model_size;

GLViewer::GLViewer(QWidget *parent) :
    QGLWidget(parent)
{
     xRot = 0;
     yRot = 0;
     zRot = 0;
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);
}

 QSize GLViewer::minimumSizeHint() const
 {
     return QSize(50, 50);
 }

 QSize GLViewer::sizeHint() const
 {
     return QSize(400, 400);
 }

 static void qNormalizeAngle(int &angle)
 {
     while (angle < 0)
         angle += 360 * 16;
     while (angle > 360 * 16)
         angle -= 360 * 16;
 }

 void GLViewer::setXRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != xRot) {
         xRot = angle;
         updateGL();
     }
 }

 void GLViewer::setYRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != yRot) {
         yRot = angle;
         updateGL();
     }
 }

 void GLViewer::setZRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != zRot) {
         zRot = angle;
         updateGL();
     }
 }

void GLViewer::initializeGL(){
    glClearColor(0.2, 0.2, 0.2, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void GLViewer::paintGL(){
    rot += 0.5;
    if(update_model){
        update_model = false;
	zoom = -1.0f;
	rot = 0.0f;
	//vertical_pan = 0.0f;
	loadGLTextures();
        buildlists();
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, -zoom * model_size, -zoom * model_size / 2.0 ,         0, 0, 0.0,           0, 0, 1);
    glRotatef(rot, 0, 0, 1);
    //glRotatef(-xRot / 16.0, 1.0, 0.0, 0.0);
    //glRotatef(-yRot / 16.0, 0.0, 1.0, 0.0);
    //glRotatef(-zRot / 16.0, 0.0, 0.0, 1.0);
    glTranslatef(0, 0, vertical_pan * model_size);
    glCallList(model_list);
}

void GLViewer::wheelEvent(QWheelEvent *event){
     zoom += (float)event->delta() / 4000;
     event->accept();
}

//Left click : pan up
//Right click : pan down
void GLViewer::mouseReleaseEvent(QMouseEvent *event){
    /*
    if(event->button() == Qt::LeftButton){
	vertical_pan += 0.01;
    } else if(event->button() == Qt::RightButton){
	vertical_pan -= 0.01;
    }
    */
}

 void GLViewer::mousePressEvent(QMouseEvent *event)
 {
     lastPos = event->pos();
    if(event->button() == Qt::LeftButton){
	vertical_pan += 0.02;
    } else if(event->button() == Qt::RightButton){
	vertical_pan -= 0.02;
    }
 }

 void GLViewer::mouseMoveEvent(QMouseEvent *event)
 {
     int dx = event->x() - lastPos.x();
     int dy = event->y() - lastPos.y();

     if (event->buttons() & Qt::LeftButton) {
         setXRotation(xRot + 8 * dy);
         setYRotation(yRot + 8 * dx);
     } else if (event->buttons() & Qt::RightButton) {
         setXRotation(xRot + 8 * dy);
         setZRotation(zRot + 8 * dx);
     }
     lastPos = event->pos();
 }

void GLViewer::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (double)w/h, 0.01, 1000);
}

void GLViewer::loadGLTextures(){
    //printf("Deleting %ld textures\n", texture.size());
    glDeleteTextures(texture.size(), &texture[0]);
    texture.clear();
    texture_map.clear();
    texture.resize(material_map.size());
    //printf("Reserved space for %ld textures | materials %ld\n", texture.size(), material_map.size());
    int tex = 0;
    for(std::map <std::string, Material>::iterator it = material_map.begin(); it != material_map.end(); it++){
        if(it->second.map_Kd != ""){
            QImage t;
            QImage b;
            QString filename = QString(DATA_DIR.c_str()) + QString(it->second.map_Kd.c_str());
            if ( !b.load(filename) )
            {
                printf("Texture couldn't be loaded. Proceeding without textures..");
            }
            t = GLViewer::convertToGLFormat( b );
            //printf("Texture id to bind : %d\n", tex);
            glGenTextures( 1, &texture[tex] );
            glBindTexture( GL_TEXTURE_2D, texture[tex] );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            glTexImage2D( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );

        }
        texture_map[it->first] = tex;
        tex++;
    }
}

void GLViewer::buildlists()
{
    model_list = glGenLists(1);
    glNewList(model_list, GL_COMPILE);

    for(unsigned int i = 0; i < m.faces.size(); i++){
	    Material mat = material_map[m.faces[i].material];
            //printf("Material in buildLists : %s\n", m.faces[i].material.c_str());
            if(mat.map_Kd != ""){
                //printf("Texture Found! Material in buildLists : %s\n", m.faces[i].material.c_str());
                glBindTexture(GL_TEXTURE_2D, texture[texture_map[m.faces[i].material]]);
	    } else {
                glBindTexture(GL_TEXTURE_2D, 0);
	    }
            glBegin(GL_POLYGON);
            float a1 = m.vertices[m.faces[i].vertex_indices[1]].x - m.vertices[m.faces[i].vertex_indices[0]].x;
            float a2 = m.vertices[m.faces[i].vertex_indices[1]].y - m.vertices[m.faces[i].vertex_indices[0]].y;
            float a3 = m.vertices[m.faces[i].vertex_indices[1]].z - m.vertices[m.faces[i].vertex_indices[0]].z;
            float b1 = m.vertices[m.faces[i].vertex_indices[2]].x - m.vertices[m.faces[i].vertex_indices[0]].x;
            float b2 = m.vertices[m.faces[i].vertex_indices[2]].y - m.vertices[m.faces[i].vertex_indices[0]].y;
            float b3 = m.vertices[m.faces[i].vertex_indices[2]].z - m.vertices[m.faces[i].vertex_indices[0]].z;
            float denom = sqrt((a2*b3 - b2*a3)*(a2*b3 - b2*a3) + (a3*b1 - a1*b3)*(a3*b1 - a1*b3) + (a1*b2 - a2*b1)*(a1*b2 - a2*b1));
            glNormal3f((a2*b3 - b2*a3)/denom, (a3*b1 - a1*b3)/denom, (a1*b2 - a2*b1)/denom);
            for(unsigned int j = 0; j < m.faces[i].vertex_indices.size(); j++){
		    glColor3f(mat.Kd.x, mat.Kd.y, mat.Kd.z);
		    //printf("Color : %f %f %f\n", mat.Kd.x, mat.Kd.y, mat.Kd.z);
		    if(m.faces[i].texture_indices.size() > 0)
                    glTexCoord2f(m.tex_coords[m.faces[i].texture_indices[j]].x,
                                  m.tex_coords[m.faces[i].texture_indices[j]].y);
                    glVertex3f(	m.vertices[m.faces[i].vertex_indices[j]].x,
				m.vertices[m.faces[i].vertex_indices[j]].y, 
				m.vertices[m.faces[i].vertex_indices[j]].z);
            }
            glEnd();
    }

    glEndList();
}

