#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "util.h"
#include "model.h"
#include <vector>
#include <algorithm> //required for min and max

/**
--------------------------
Rule properties
--------------------------
RSPLIT_Z : First argument is the number of repetitions. The next arguments (equal to the number of successors) are the weights for each respective successor.

*/
enum Rule {
	TRANSLATE,
	TRANSLATE_LOT,
	ROTATE,
	SCALE,
	EXTRUDE, 
	COMP_SPLIT,
	UNIFORM_SUBDIVIDE, // Repeats the subdivision N times, syntax : uniform_subdiv(N, frac1, frac2... ){Part1, Part2, ...}
	SUBDIVIDE,	    //along vertical
	REPEAT_SUBDIVIDE,   //along vertical
	SPLIT,		    //along horizontal
	UNIFORM_SPLIT,	    //along horizontal
	REPEAT_SPLIT,	    //along_horizontal
	LOT_SPLIT, 
	LOT_SPLIT_SIMPLE, 
	LOT_SPLIT_RAND, 
	INSERT, 
	CIRC, //Lot
	RECT, //Lot
	CENTER, //Lot
	EDGES, //Lot
	CORNERS, //Lot
	CYLINDER,
	ROUND_DOOR,
	PAR_CONE_ROOF,
	VOLUME,
	CONE_ROOF,
	ADD_MATERIAL,
	ADD_TEXTURE
}; 

enum NodeType {
	LOT,
	CIRCULAR_CORNER_LOT,
	EDGE_LOT,
	NORMAL_LOT_CHILD,
	NORMAL_CHILD	
};


class Successor {
	public:
	Rule rule;
	std::vector<float> args[2];
	std::vector<std::string> names;
};

class Component {
	public:
	Model geometry;
	std::vector<Successor> children;
	std::vector<float> weights;
};

class DynamicComp {
	public:
	std::string handle;
	Model geometry;
};

class TransformationNode {
	public:
	TransformationNode() {
		matrix[0][0] = 1; matrix[0][1] = 0; matrix[0][2] = 0; matrix[0][3] = 0; 
		matrix[1][0] = 0; matrix[1][1] = 1; matrix[1][2] = 0; matrix[1][3] = 0; 
		matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1; matrix[2][3] = 0; 
		matrix[3][0] = 0; matrix[3][1] = 0; matrix[3][2] = 0; matrix[3][3] = 1; 
		type = NORMAL_CHILD;
		painted = false;
		scale_x = 1.0f;
		scale_y = 1.0f;
		tex_off_u = 0.0f;
		tex_off_v = 0.0f;
		tex_len_u = 1.0f;
		tex_len_v = 1.0f;
		material = "default";
	}
	std::string handle;
	float matrix[4][4];
	float scale_x;
	float scale_y;	
	float tex_off_u;
	float tex_off_v;
	float tex_len_u;
	float tex_len_v;
	NodeType type;
	bool painted;
	std::string material;
	std::vector<TransformationNode> children;	
};

extern void getNewGlobalTransformationMatrix(TransformationNode global, TransformationNode& local);
extern void applyTransformation(Model &model, TransformationNode T);
#endif
