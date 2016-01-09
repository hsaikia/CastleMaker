#include "component.h"

void getNewGlobalTransformationMatrix(TransformationNode global, TransformationNode& local){
	TransformationNode global_new = local;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			global_new.matrix[i][j] = 0.0f;
			for(int k = 0; k < 4; k++){
				global_new.matrix[i][j] += (global.matrix[i][k] * local.matrix[k][j]);
			}
		}
	}
	local = global_new;
	local.tex_len_u *= global.tex_len_u;
	local.tex_len_v *= global.tex_len_v;
	local.tex_off_u = global.tex_off_u + local.tex_off_u * global.tex_len_u;
	local.tex_off_v = global.tex_off_v + local.tex_off_v * global.tex_len_v;
	if(global.painted && !local.painted){
	    local.painted = true;
	    local.material = global.material;
	    local.scale_x = global.scale_x;
	    local.scale_y = global.scale_y;
	}	
}

void applyTransformation(Model &model, TransformationNode T){
	//printf("vo %f vl %f\n", T.tex_off_v, T.tex_len_v);
	
	for(int i = 0; i < model.vertices.size(); i++){
                Point p;
                p.x = T.matrix[0][0] * model.vertices[i].x + 
			T.matrix[0][1] * model.vertices[i].y + 
			T.matrix[0][2] * model.vertices[i].z + 
			T.matrix[0][3];
                p.y = T.matrix[1][0] * model.vertices[i].x + 
			T.matrix[1][1] * model.vertices[i].y + 
			T.matrix[1][2] * model.vertices[i].z + 
			T.matrix[1][3];
                p.z = T.matrix[2][0] * model.vertices[i].x + 
			T.matrix[2][1] * model.vertices[i].y + 
			T.matrix[2][2] * model.vertices[i].z + 
			T.matrix[2][3];
                model.vertices[i] = p; 
	}
	if(T.painted && !model.is_textured){
	/*
	    for(int i = 0; i < model.tex_coords.size(); i++){
		model.tex_coords[i].x *= T.scale_x;
		model.tex_coords[i].y *= T.scale_y;
	    }
*/
	    for(int i = 0; i < model.faces.size(); i++){
		model.faces[i].material = T.material;
		//model.faces[i].scale_textures = true;
	    }
	    for(int i = 0; i < model.tex_coords.size(); i++){
		//printf("tex coords before : %f %f\n", model.tex_coords[i].x, model.tex_coords[i].y);
	        model.tex_coords[i].x = (model.tex_coords[i].x * T.tex_len_u + T.tex_off_u) * T.scale_x;
		model.tex_coords[i].y = (model.tex_coords[i].y * T.tex_len_v + T.tex_off_v) * T.scale_y;
		
	        //model.tex_coords[i].x = model.tex_coords[i].x > 1 ? model.tex_coords[i].x - (int)model.tex_coords[i].x : model.tex_coords[i].x;
	        //model.tex_coords[i].y = model.tex_coords[i].y > 1 ? model.tex_coords[i].y - (int)model.tex_coords[i].y : model.tex_coords[i].y;
	        //printf("tex coords after : %f %f\n", model.tex_coords[i].x, model.tex_coords[i].y);
	    }
	}
} 
