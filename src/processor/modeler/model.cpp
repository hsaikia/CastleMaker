#include "model.h"

float dist(Point a, Point b){
    return (float)sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}
// removes duplicate vertices, converts non-triangular faces to triangular faces, add normals
void cleanModel(Model &model) {
    //duplicates
    //see whether the distance between any two vertices is less than a certain threshold
    for(int i = 0; i < model.vertices.size(); i++) {
        for(int j = i + 1; j < model.vertices.size(); j++) {
            float dx = model.vertices[i].x - model.vertices[j].x;
            float dy = model.vertices[i].y - model.vertices[j].y;
            float dz = model.vertices[i].z - model.vertices[j].z;
            if(dx*dx + dy*dy + dz*dz < 0.01f) {
                //i and j are duplicates
                //update faces
                for(int k = 0; k < model.faces.size(); k++) {
                    for(int l = 0; l < model.faces[k].vertex_indices.size(); l++) {
                        if(model.faces[k].vertex_indices[l] == j)
                            model.faces[k].vertex_indices[l] = i;
                    }
                }
            }
        }
    }
    //TODO : To comment out later!!!
    /*

    	for(int k = 0; k < model.faces.size(); k++){
        if(model.faces[k].vertex_indices.size() > 3){
                    for(int l = 0; l < model.faces[k].vertex_indices.size() - 2; l++){
    		std::vector<int> face;
    		face.push_back(model.faces[k][0]);
    		face.push_back(model.faces[k][l + 1]);
    		face.push_back(model.faces[k][l + 2]);
    		model.faces.push_back(face);
    	}
    	model.faces.erase(model.faces.begin() + k);
    	k--;
    }
    	}
    	for(int k = 0; k < model.faces.size(); k++){
    if(model.faces[k][0] == model.faces[k][1] || model.faces[k][2] == model.faces[k][1] || model.faces[k][0] == model.faces[k][2]){
    	model.faces.erase(model.faces.begin() + k);
    	k--;
    }
    }*/
}
void getBBox(Model model, BBox& bbox) {
    int num_vertices = model.vertices.size();
    bbox.min = Point(_FLT_MAX, _FLT_MAX, _FLT_MAX);
    bbox.max = Point(-_FLT_MAX, -_FLT_MAX, -_FLT_MAX);
    for(int i=0; i<num_vertices; i++) {
        bbox.min.x = std::min(bbox.min.x, model.vertices[i].x);
        bbox.min.y = std::min(bbox.min.y, model.vertices[i].y);
        bbox.min.z = std::min(bbox.min.z, model.vertices[i].z);
        bbox.max.x = std::max(bbox.max.x, model.vertices[i].x);
        bbox.max.y = std::max(bbox.max.y, model.vertices[i].y);
        bbox.max.z = std::max(bbox.max.z, model.vertices[i].z);
    }
}

/*
x_elevation = linear interpolation between the center and the corresponding vertex gives the upper x co-ordinate
y_elevation = similar as x_elevation only in the y-direction
*/

void getVolume(float length, float breadth, float height, float x_elevation, float y_elevation, Model& vol) {
    vol.vertices.push_back(Point(-length/2, -breadth/2, 0));
    vol.vertices.push_back(Point(length/2, -breadth/2, 0));
    vol.vertices.push_back(Point(length/2, breadth/2, 0));
    vol.vertices.push_back(Point(-length/2, breadth/2, 0));

    vol.vertices.push_back(Point(-(1.0 - x_elevation)*length/2, -(1.0 - y_elevation)*breadth/2, height));
    vol.vertices.push_back(Point((1.0 - x_elevation)*length/2, -(1.0 - y_elevation)*breadth/2, height));
    vol.vertices.push_back(Point((1.0 - x_elevation)*length/2, (1.0 - y_elevation)*breadth/2, height));
    vol.vertices.push_back(Point(-(1.0 - x_elevation)*length/2, (1.0 - y_elevation)*breadth/2, height));

    vol.tex_coords.push_back(Point(0, 0, 0));
    vol.tex_coords.push_back(Point(1, 0, 0));
    vol.tex_coords.push_back(Point(1, 1, 0));
    vol.tex_coords.push_back(Point(0, 1, 0));
    vol.tex_coords.push_back(Point(0, 0, 0));
    vol.tex_coords.push_back(Point(1, 0, 0));
    vol.tex_coords.push_back(Point(1, 1, 0));
    vol.tex_coords.push_back(Point(0, 1, 0));
    vol.tex_coords.push_back(Point(0, 0, 0));
    vol.tex_coords.push_back(Point(1, 0, 0));
    vol.tex_coords.push_back(Point(1, 1, 0));
    vol.tex_coords.push_back(Point(0, 1, 0));
    vol.tex_coords.push_back(Point(0, 0, 0));
    vol.tex_coords.push_back(Point(1, 0, 0));
    vol.tex_coords.push_back(Point(1, 1, 0));
    vol.tex_coords.push_back(Point(0, 1, 0));
    vol.tex_coords.push_back(Point(0, 0, 0));
    vol.tex_coords.push_back(Point(1, 0, 0));
    vol.tex_coords.push_back(Point(1, 1, 0));
    vol.tex_coords.push_back(Point(0, 1, 0));

    Face face1, face2, face3, face4, face5;
    face1.vertex_indices.push_back(0); face1.vertex_indices.push_back(1);
    face1.vertex_indices.push_back(5); face1.vertex_indices.push_back(4);

    face1.texture_indices.push_back(0); face1.texture_indices.push_back(1);
    face1.texture_indices.push_back(2); face1.texture_indices.push_back(3);

    vol.faces.push_back(face1);

    face2.vertex_indices.push_back(1); face2.vertex_indices.push_back(2);
    face2.vertex_indices.push_back(6); face2.vertex_indices.push_back(5);

    face2.texture_indices.push_back(4); face2.texture_indices.push_back(5);
    face2.texture_indices.push_back(6); face2.texture_indices.push_back(7);

    vol.faces.push_back(face2);

    face3.vertex_indices.push_back(2); face3.vertex_indices.push_back(3);
    face3.vertex_indices.push_back(7); face3.vertex_indices.push_back(6);

    face3.texture_indices.push_back(8); face3.texture_indices.push_back(9);
    face3.texture_indices.push_back(10); face3.texture_indices.push_back(11);
    
    vol.faces.push_back(face3);

    face4.vertex_indices.push_back(3); face4.vertex_indices.push_back(0);
    face4.vertex_indices.push_back(4); face4.vertex_indices.push_back(7);

    face4.texture_indices.push_back(12); face4.texture_indices.push_back(13);
    face4.texture_indices.push_back(14); face4.texture_indices.push_back(15);

    vol.faces.push_back(face4);

    face5.vertex_indices.push_back(4); face5.vertex_indices.push_back(5);
    face5.vertex_indices.push_back(6); face5.vertex_indices.push_back(7);

    face5.texture_indices.push_back(16); face5.texture_indices.push_back(17);
    face5.texture_indices.push_back(18); face5.texture_indices.push_back(19);

    vol.faces.push_back(face5);
}


//default center of the base is the origin
void getCone(float radius, float height, int div, Model& cone) {
    float angle_increment = 2*M_PI/div;
    for(int i = 0; i < div; i++) {
        float x1 = cos(i * angle_increment);
        float y1 = sin(i * angle_increment);
        cone.vertices.push_back(Point(radius * x1, radius * y1, 0));
    }
    cone.vertices.push_back(Point(0, 0, height));
    int tex_cos = 0;
    for(int i = 0; i < div; i++) {
        Face face;
        face.vertex_indices.push_back(i);
        face.vertex_indices.push_back((i+1)%div);
        face.vertex_indices.push_back(div);
	cone.tex_coords.push_back(Point(0, 0, 0));
	cone.tex_coords.push_back(Point(1, 0, 0));
	cone.tex_coords.push_back(Point(0.5, 1, 0));
	face.texture_indices.push_back(tex_cos);
	face.texture_indices.push_back(tex_cos + 1);
	face.texture_indices.push_back(tex_cos + 2);
	tex_cos += 3;
        cone.faces.push_back(face);
    }
}

//should not be further subdivided due to obvious reasons!
void getParabolicCone(float radius, float height, int hdiv, int vdiv, Model& parCone) {
    float angle_increment = 2 * M_PI / hdiv;
    float height_increment = height/vdiv;
    for(int i = 0; i < hdiv; i++) {
        for(int j = 0; j < vdiv; j++) {
            float rad = radius * (1 - sqrt(j * (height_increment/height) )) ;
            float x1 = cos(i * angle_increment);
            float y1 = sin(i * angle_increment);
            parCone.vertices.push_back(Point(rad * x1, rad * y1, j * height_increment));
        }
    }
    parCone.vertices.push_back(Point(0, 0, height));
    int tex_cos = 0;
    for(int i = 0; i < hdiv; i++) {
        for(int j = 0; j < vdiv - 1; j++) {
            Face face;
	    face.material = "default";
            face.vertex_indices.push_back(i*vdiv + j);
            face.vertex_indices.push_back((i*vdiv + vdiv)%(hdiv * vdiv) + j);
            face.vertex_indices.push_back((i*vdiv + vdiv)%(hdiv * vdiv) + j + 1);
            face.vertex_indices.push_back(i*vdiv + j + 1);
	    parCone.tex_coords.push_back(Point(0, 0, 0));
	    parCone.tex_coords.push_back(Point(1, 0, 0));
	    parCone.tex_coords.push_back(Point(1, 1, 0));
	    parCone.tex_coords.push_back(Point(0, 1, 0));
	    face.texture_indices.push_back(tex_cos);
	    face.texture_indices.push_back(tex_cos + 1);
	    face.texture_indices.push_back(tex_cos + 2);
	    face.texture_indices.push_back(tex_cos + 3);
	    tex_cos += 4;
            parCone.faces.push_back(face);
        }
        Face top_point;
	top_point.material = "default";
        top_point.vertex_indices.push_back(i*vdiv + vdiv - 1);
        top_point.vertex_indices.push_back((i*vdiv + vdiv)%(hdiv * vdiv) + vdiv - 1);
        top_point.vertex_indices.push_back(hdiv * vdiv);
	parCone.tex_coords.push_back(Point(0, 0, 0));
	parCone.tex_coords.push_back(Point(1, 0, 0));
	parCone.tex_coords.push_back(Point(0.5, 1, 0));
	top_point.texture_indices.push_back(tex_cos);
	top_point.texture_indices.push_back(tex_cos + 1);
	top_point.texture_indices.push_back(tex_cos + 2);
	tex_cos += 3;
        parCone.faces.push_back(top_point);
    }
}

//default center of the base is the origin
void getCylinder(float bottom_rad, float top_rad, float height, int div, Model& cyl) {
    float angle_increment = 2*M_PI/div;
    for(int i = 0; i < div; i++) {
        float x1 = bottom_rad * cos(i * angle_increment);
        float y1 = bottom_rad * sin(i * angle_increment);
        cyl.vertices.push_back(Point(x1, y1, 0));
    }
    for(int i = 0; i < div; i++) {
        float x2 = top_rad * cos(i * angle_increment);
        float y2 = top_rad * sin(i * angle_increment);
        cyl.vertices.push_back(Point(x2, y2, height));
    }
    int tex_cos = 0;
    float tx = 0.0f;
    float ix = 1.0 / div;
    for(int i = 0; i < div; i++) {
        Face face;
        face.vertex_indices.push_back(i);
        face.vertex_indices.push_back((i + 1)%div);
        face.vertex_indices.push_back((i + 1)%div + div);
        face.vertex_indices.push_back(i + div);
	cyl.tex_coords.push_back(Point(tx, 0, 0));
	cyl.tex_coords.push_back(Point(tx + ix, 0, 0));
	cyl.tex_coords.push_back(Point(tx + ix, 1, 0));
	cyl.tex_coords.push_back(Point(tx, 1, 0));
	tx = tx + ix;
	face.texture_indices.push_back(tex_cos);
	face.texture_indices.push_back(tex_cos + 1);
	face.texture_indices.push_back(tex_cos + 2);
	face.texture_indices.push_back(tex_cos + 3);
	tex_cos += 4;
        cyl.faces.push_back(face);
    }
    /*
    Face top;
    for(int i = 0; i < div; i++) {
        top.vertex_indices.push_back(i + div);
    }
    cyl.faces.push_back(top);
    */

}

void getRoundedDoor(float height, float breadth, float thickness, int divisions, Model& door){
    
    //The bottom rectangular part
    door.vertices.push_back(Point(-breadth/2, 0, 0));
    door.vertices.push_back(Point(breadth/2, 0, 0));
    door.vertices.push_back(Point(breadth/2, 0, height));
    door.vertices.push_back(Point(-breadth/2, 0, height));
    door.vertices.push_back(Point(-breadth/2, thickness, 0));
    door.vertices.push_back(Point(breadth/2, thickness, 0));
    door.vertices.push_back(Point(breadth/2, thickness, height));
    door.vertices.push_back(Point(-breadth/2, thickness, height));

    door.tex_coords.push_back(Point(0, 0, 0));
    door.tex_coords.push_back(Point(1, 0, 0));
    door.tex_coords.push_back(Point(1, height/breadth, 0));
    door.tex_coords.push_back(Point(0, height/breadth, 0));
   
    Face face;
    face.vertex_indices.push_back(4); 
    face.vertex_indices.push_back(5); 
    face.vertex_indices.push_back(6); 
    face.vertex_indices.push_back(7);
    face.texture_indices.push_back(0); 
    face.texture_indices.push_back(1);
    face.texture_indices.push_back(2);
    face.texture_indices.push_back(3);
    door.faces.push_back(face);

    //Side faces
    Face face1;
    face1.vertex_indices.push_back(4); 
    face1.vertex_indices.push_back(0); 
    face1.vertex_indices.push_back(3); 
    face1.vertex_indices.push_back(7);
    door.faces.push_back(face1);
    
    Face face3;
    face3.vertex_indices.push_back(1); 
    face3.vertex_indices.push_back(5); 
    face3.vertex_indices.push_back(6); 
    face3.vertex_indices.push_back(2);
    door.faces.push_back(face3);

    Face face4;
    face4.vertex_indices.push_back(4); 
    face4.vertex_indices.push_back(0); 
    face4.vertex_indices.push_back(1); 
    face4.vertex_indices.push_back(5);
    door.faces.push_back(face4);

    //Now the circular part


    float angle = 0.0f;
    float angle_inc = M_PI / divisions;
    float r = breadth / 2;
    //center point 
    door.vertices.push_back(Point(0, thickness, height));
    door.tex_coords.push_back(Point(0.5, height/breadth, 0));
    door.tex_coords.push_back(Point(1, height/breadth, 0));
    for(int i = 0; i < divisions - 1; i++){
	angle += angle_inc;
	door.vertices.push_back(Point(r * cos(angle), 0, height + r * sin(angle)));
	door.vertices.push_back(Point(r * cos(angle), thickness, height + r * sin(angle)));  
	door.tex_coords.push_back(Point(0.5 + 0.5 * cos(angle), (height + r * sin(angle))/breadth, 0));
	door.tex_coords.push_back(Point(0.5, height/breadth, 0));
	door.tex_coords.push_back(Point(0.5 + 0.5 * cos(angle), (height + r * sin(angle))/breadth, 0));
    }
    door.tex_coords.push_back(Point(0, height/breadth, 0));

    Face t1;
    t1.vertex_indices.push_back(8);
    t1.vertex_indices.push_back(6);
    t1.vertex_indices.push_back(10);
    t1.texture_indices.push_back(4);
    t1.texture_indices.push_back(5);
    t1.texture_indices.push_back(6);
    door.faces.push_back(t1);
    
    Face t1side;
    t1side.vertex_indices.push_back(2);
    t1side.vertex_indices.push_back(6);
    t1side.vertex_indices.push_back(10);
    t1side.vertex_indices.push_back(9);
    door.faces.push_back(t1side);
    
    for(int i = 1; i < divisions - 1; i++){
    Face t;
    t.vertex_indices.push_back(8);
    t.vertex_indices.push_back(8 + 2*i);
    t.vertex_indices.push_back(10 + 2*i);
    t.texture_indices.push_back(4 + 3*i);
    t.texture_indices.push_back(5 + 3*i);
    t.texture_indices.push_back(6 + 3*i);
    door.faces.push_back(t);
    
    Face tside;
    tside.vertex_indices.push_back(7 + 2*i);
    tside.vertex_indices.push_back(8 + 2*i);
    tside.vertex_indices.push_back(10 + 2*i);
    tside.vertex_indices.push_back(9 + 2*i);
    door.faces.push_back(tside);
    }
    Face tl;
    tl.vertex_indices.push_back(8);
    tl.vertex_indices.push_back(6 + 2 * divisions);
    tl.vertex_indices.push_back(7);
    tl.texture_indices.push_back(1 + 3*divisions);
    tl.texture_indices.push_back(2 + 3*divisions);
    tl.texture_indices.push_back(3 + 3*divisions);
    door.faces.push_back(tl);
    
    Face tlside;
    tlside.vertex_indices.push_back(7);
    tlside.vertex_indices.push_back(3);
    tlside.vertex_indices.push_back(5 + 2*divisions);
    tlside.vertex_indices.push_back(6 + 2*divisions);
    door.faces.push_back(tlside);
}

//translate
void translate(Model& model, float tx, float ty, float tz){
	for(int i=0; i<model.vertices.size(); i++){
		model.vertices[i].x += tx;
		model.vertices[i].y += ty;
		model.vertices[i].z += tz;
	}
}

//scales a model
void scale(Model& model, float sx, float sy, float sz) {
    for(int i=0; i<model.vertices.size(); i++) {
        if(sx > EPS) model.vertices[i].x *= sx;
        if(sy > EPS) model.vertices[i].y *= sy;
        if(sz > EPS) model.vertices[i].z *= sz;
    }
}

void alignAndFitToOrigin(Model& model, bool scaleZ) {
    BBox scope;
    getBBox(model, scope);
    /*
    float factor = std::max(std::max(scope.max.x - scope.min.x, scope.max.y - scope.min.y), scope.max.z - scope.min.z);
    scale(model, 1.0/factor, 1.0/factor, 1.0/factor);
*/
    bool is_thin = false;
    if(10 * (scope.max.y - scope.min.y) <= scope.max.x - scope.min.x){
        float factor = std::max(scope.max.x - scope.min.x, scope.max.y - scope.min.y);
	if(scaleZ)
	    scale(model, 1.0/factor, 1.0/factor, 1.0/(scope.max.z - scope.min.z));
	else
	    scale(model, 1.0/factor, 1.0/factor, 1.0);
	    
	is_thin = true;
    } else {
	if(scaleZ)
	    scale(model, 1.0/(scope.max.x - scope.min.x), 1.0/(scope.max.y - scope.min.y), 1.0/(scope.max.z - scope.min.z));
	else
	    scale(model, 1.0/(scope.max.x - scope.min.x), 1.0/(scope.max.y - scope.min.y), 1.0);
    }
    getBBox(model, scope);
    for(int i = 0; i < model.vertices.size(); i++) {
        model.vertices[i].x -= (scope.max.x + scope.min.x)/2.0;
	if(!is_thin)
	    model.vertices[i].y -= (scope.min.y + scope.max.y)/2.0;
	else
	    model.vertices[i].y -= scope.min.y;
        model.vertices[i].z -= scope.min.z;
    }

}

//part is to be combined to final
void combine(Model& final, Model part) {
    int num_vertices_old = final.vertices.size();
    int num_tex_coords_old = final.tex_coords.size();
    final.vertices.insert(final.vertices.end(), part.vertices.begin(), part.vertices.end());
    final.tex_coords.insert(final.tex_coords.end(), part.tex_coords.begin(), part.tex_coords.end());
    for(int i = 0; i < part.faces.size(); i++) {
        Face f = part.faces[i];
        for(int j = 0; j < f.vertex_indices.size(); j++) {
            f.vertex_indices[j] += num_vertices_old;
        }
        for(int j = 0; j < f.texture_indices.size(); j++) {
            f.texture_indices[j] += num_tex_coords_old;
        }
        final.faces.push_back(f);
    }
}
/* Will avoid the issue of random scaling and will scale textures according to the size of the polygon */

/*
void scaleTextures(Model &model){
    for(int i = 0; i < model.faces.size(); i++){
	if(!model.faces[i].scale_textures){
	    continue;
	}
	int v1 = model.faces[i].vertex_indices[0];
	int v2 = model.faces[i].vertex_indices[1];
	int v3 = model.faces[i].vertex_indices[2];
	float scale_x = dist(model.vertices[v1], model.vertices[v2]);
	float scale_y = dist(model.vertices[v3], model.vertices[v2]);
	for(int j = 0; j < model.faces[i].texture_indices.size(); j++){
	    model.tex_coords[model.faces[i].texture_indices[j]].x *= scale_x;
	    model.tex_coords[model.faces[i].texture_indices[j]].y *= scale_y;
	}
    }
}
*/

