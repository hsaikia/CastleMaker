#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <cstdio>
#include <cmath>
#include <utility>
#include "util.h"

//Faces are defined in the same way as they are defined in obj files, i.e. with the indexes of the vertices in the vertices vector, note that they are 0-indexed.


struct Face {
    Face(){
	//scale_textures = false;
	material = "default";
    }
    //bool scale_textures;
    std::string material;
    std::vector<int> vertex_indices;
    std::vector<int> texture_indices;
};

struct Model {
    Model(){
	is_textured = false;
    }
    std::vector<Point> vertices;
    std::vector<Point> tex_coords;		//will use only u and v
    std::vector<Face> faces; // pair of material and face
    bool is_textured;
};

struct Material {
    Point Ka; // Point used as Color
    Point Kd;
    std::string map_Ka;
    std::string map_Kd;
};

extern void cleanModel(Model &model);
extern void scaleTextures(Model &model);
extern void getVolume(float length, float breadth, float height, float x_elevation, float y_elevation,  Model& vol);
extern void getCone(float radius, float height, int div, Model& cone);
extern void getParabolicCone(float radius, float height, int hdiv, int vdiv, Model& parCone);
extern void getCylinder(float bottom_rad, float top_rad, float height, int div, Model& cyl);
extern void getRoundedDoor(float height, float breadth, float thickness, int divisions, Model& door);
extern void translate(Model& model, float tx, float ty, float tz);
extern void scale(Model& model, float sx, float sy, float sz);
extern void getBBox(Model model, BBox& bbox);
extern void alignAndFitToOrigin(Model& model, bool scaleZ);
extern void combine(Model&, Model);
#endif
