#include "model_processor.h"
#include <algorithm>
extern FILE* yyin;
extern int yyparse();
extern std::map<std::string, float> attr_map;
extern std::map<std::string, std::vector<Component> > comp_map;
extern std::map<std::string, Component> lot_map;
extern std::map <std::string, Material > material_map;
extern std::vector<std::string> errors;
std::vector<TransformationNode> lot_nodes;
std::map<std::string, Model> leaf_map;
int line_number;

void compute_children(std::vector<TransformationNode> &children, Successor _s) {
    //Randomizations for random intervals
    for(int arg = 0; arg < _s.args[0].size(); arg++) {
        _s.args[0][arg] = (_s.args[1][arg] - _s.args[0][arg]) * rand() / RAND_MAX + _s.args[0][arg];
    }
    if(_s.rule == ADD_MATERIAL){
        TransformationNode node;
	node.painted = true;
	node.material = _s.names[0];
	node.handle = _s.names[1];
	Material mat;
	mat.Ka = Point(_s.args[0][0], _s.args[0][1], _s.args[0][2]);	
	mat.Kd = Point(_s.args[0][0], _s.args[0][1], _s.args[0][2]);	
	material_map[_s.names[0]] = mat;
	children.push_back(node);
    } else if (_s.rule == ADD_TEXTURE) {
        TransformationNode node;
	node.painted = true;
	node.material = _s.names[0];
	node.handle = _s.names[2];
	node.scale_x = _s.args[0][0];
	node.scale_y = _s.args[0][1];
	Material mat;
	mat.Ka = Point(1, 1, 1);
	mat.Kd = Point(1, 1, 1);
	mat.map_Ka = _s.names[1];
	mat.map_Kd = _s.names[1];
	material_map[_s.names[0]] = mat;
	children.push_back(node);
    } else if(_s.rule == TRANSLATE) {
        TransformationNode node;
        node.handle = _s.names[0];
        node.matrix[0][3] = _s.args[0][0];
        node.matrix[1][3] = _s.args[0][1];
        node.matrix[2][3] = _s.args[0][2];
        children.push_back(node);
    } else if(_s.rule == ROTATE) {
        TransformationNode node;
        node.handle = _s.names[0];
	int rotations = (int)_s.args[0][0] % 4;
        node.matrix[0][0] =  cos(rotations * M_PI / 2);
        node.matrix[0][1] = -sin(rotations * M_PI / 2);
        node.matrix[1][0] = sin(rotations * M_PI / 2);
        node.matrix[1][1] = cos(rotations * M_PI / 2);
        children.push_back(node);
    } else if(_s.rule == TRANSLATE_LOT) {
        TransformationNode node;
        node.handle = _s.names[0];
        node.matrix[0][3] = _s.args[0][0];
        node.matrix[1][3] = _s.args[0][1];
        node.matrix[2][3] = _s.args[0][2];
        node.type = LOT;
        children.push_back(node);
    } else if(_s.rule == SCALE) {
        TransformationNode node;
        node.handle = _s.names[0];
        node.matrix[0][0] = _s.args[0][0];
        node.matrix[1][1] = _s.args[0][1];
        node.matrix[2][2] = _s.args[0][2];
        children.push_back(node);
    } else if(_s.rule == EXTRUDE) {
        TransformationNode node;
        node.handle = _s.names[0];
        node.matrix[2][2] = _s.args[0][0];
        children.push_back(node);
    } else if (_s.rule == LOT_SPLIT_SIMPLE) {
        TransformationNode l1, l2, l3, l4;
        l1.handle = _s.names[0 % _s.names.size()];
        l2.handle = _s.names[1 % _s.names.size()];
        l3.handle = _s.names[2 % _s.names.size()];
        l4.handle = _s.names[3 % _s.names.size()];
        float x_split = _s.args[0][0] - 0.5;
        float y_split = _s.args[0][1] - 0.5;
        l1.matrix[0][3] = (x_split - 0.5) / 2.0;
        l1.matrix[1][3] = (y_split - 0.5) / 2.0;
        l1.matrix[0][0] = _s.args[0][0];
        l1.matrix[1][1] = _s.args[0][1];
        l2.matrix[0][3] = (x_split + 0.5) / 2.0;
        l2.matrix[1][3] = (y_split - 0.5) / 2.0;
        l2.matrix[0][0] = 1.0 - _s.args[0][0];
        l2.matrix[1][1] = _s.args[0][1];
        l3.matrix[0][3] = (x_split + 0.5) / 2.0;
        l3.matrix[1][3] = (y_split + 0.5) / 2.0;
        l3.matrix[0][0] = 1.0 - _s.args[0][0];
        l3.matrix[1][1] = 1.0 - _s.args[0][1];
        l4.matrix[0][3] = (x_split - 0.5) / 2.0;
        l4.matrix[1][3] = (y_split + 0.5) / 2.0;
        l4.matrix[0][0] =  _s.args[0][0];
        l4.matrix[1][1] = 1.0 - _s.args[0][1];
        children.push_back(l1);
        children.push_back(l2);
        children.push_back(l3);
        children.push_back(l4);
    } else if (_s.rule == LOT_SPLIT){
	int xsplits = _s.args[0][0];
	int ysplits = _s.args[0][1];
	float xoff = 0.0f;
	int child_node = 0;
	if(_s.args[0].size() == 2){ //uniform split
	    float xinc = 1.0f/(xsplits + 1);
	    float yinc = 1.0f/(ysplits + 1);
	    for(int i = 0; i < xsplits + 1; i++){
		float yoff = 0.0f;
		for(int j = 0; j < ysplits + 1; j++){
		    TransformationNode t;
		    t.handle = _s.names[child_node % _s.names.size()];
		    t.matrix[0][3] = xoff + xinc/2 - 0.5;
		    t.matrix[1][3] = yoff + yinc/2 - 0.5;
		    t.matrix[0][0] = xinc;
		    t.matrix[1][1] = yinc;
		    yoff += yinc;
		    children.push_back(t);
		    child_node++;
		}
		xoff += xinc;
	    }
	} else {
	    for(int i = 0; i < xsplits; i++){
		float yoff = 0.0f;
		for(int j = 0; j < ysplits; j++){
		    TransformationNode t;
		    t.handle = _s.names[child_node % _s.names.size()];
		    t.matrix[0][3] = xoff + _s.args[0][2+i]/200 - 0.5;
		    t.matrix[1][3] = yoff + _s.args[0][2+xsplits+j]/200 - 0.5;
		    t.matrix[0][0] = _s.args[0][2+i]/100;
		    t.matrix[1][1] = _s.args[0][2 + xsplits + j]/100;
		    yoff += _s.args[0][2 + xsplits + j]/100;
		    children.push_back(t);
		    child_node++;
		}
		xoff += _s.args[0][2+i]/100;
	    }    
	}
	
    } else if (_s.rule == LOT_SPLIT_RAND) {
        int xsplits = _s.args[0][0];
        int ysplits = _s.args[0][1];
        std::vector<float> X;
        std::vector<float> Y;
        for(int i = 0; i < xsplits; i++) {
            X.push_back((float)rand()/RAND_MAX);
        }
        sort(X.begin(), X.end());
        X.push_back(1.0f);
        float x_off = 0.0f;
        float y_off = 0.0f;
        for(int i = 0; i <= xsplits; i++) {
            for(int j = 0; j < ysplits; j++) {
                Y.push_back((float)rand()/RAND_MAX);
            }
            sort(Y.begin(), Y.end());
            Y.push_back(1.0f);
            for(int j = 0; j <= ysplits; j++) {
                TransformationNode l;
                l.handle = _s.names[(i * xsplits + j)%_s.names.size()];
                l.matrix[0][3] = (x_off + X[i]) / 2.0f - 0.5f;
                l.matrix[1][3] = (y_off + Y[j]) / 2.0f - 0.5f;
                l.matrix[0][0] = X[i] - x_off;
                l.matrix[1][1] = Y[j] - y_off;
                // Temporary hack to avoid too stretchy lots
                if(l.matrix[0][0] < 2*l.matrix[1][1] && l.matrix[1][1] < 2*l.matrix[0][0])
                    children.push_back(l);
                y_off = Y[j];
            }
            x_off = X[i];
            y_off = 0.0f;
            Y.clear();
        }
    } else if (_s.rule == COMP_SPLIT) {
        int faces = _s.args[0][0];
        TransformationNode top;
        top.handle = _s.names[0];
        top.matrix[2][3] = 1.0;
        children.push_back(top);
        for(int i = 0; i < faces; i++) {
            TransformationNode face;
            face.handle = _s.names[i % (_s.names.size() - 1) + 1];
	    face.tex_off_u = (float)i/faces;
	    face.tex_len_u = 1.0f/faces;
            face.matrix[0][0] = cos(i * 2 * M_PI / faces) * tan(M_PI / faces);
            face.matrix[0][1] = -sin(i * 2 * M_PI / faces);
            face.matrix[1][0] = sin(i * 2 * M_PI / faces) * tan(M_PI / faces);
            face.matrix[1][1] = cos(i * 2 * M_PI / faces);
            face.matrix[1][3] = -0.5 * cos(i * 2 * M_PI / faces);
            face.matrix[0][3] = 0.5 * sin(i * 2 * M_PI / faces);
            children.push_back(face);
        }
    } else if (_s.rule == SUBDIVIDE) {
        float offset = 0.0f;
        for(int i = 0; i < _s.args[0].size(); i++) {
            TransformationNode part;
            part.handle = _s.names[i % _s.names.size()];
            part.matrix[2][3] = offset;
            part.matrix[2][2] = _s.args[0][i] / 100.0f;
	    part.tex_off_v = offset;
	    part.tex_len_v = _s.args[0][i] / 100.0f;	
            offset += _s.args[0][i] / 100.0f;
            children.push_back(part);
        }
    } else if (_s.rule == REPEAT_SUBDIVIDE) {
        int parts = _s.args[0][0];
        float offset = 0.0f;
        for(int i = 0; i < parts; i++) {
            for(int j = 0; j < _s.args[0].size() - 1; j++) {
                TransformationNode part;
                part.handle = _s.names[j % _s.names.size()];
                part.matrix[2][3] = offset;
                part.matrix[2][2] = (_s.args[0][j + 1] / parts) / 100.0f;
		part.tex_off_v = offset;
	        part.tex_len_v = (_s.args[0][j + 1] / parts) / 100.0f;	
                offset += (_s.args[0][j + 1] / parts)/100.0f;
                children.push_back(part);
            }
        }
    } else if (_s.rule == UNIFORM_SUBDIVIDE){
	int parts = _s.args[0][0];
	float offset = 0.0f;
	for(int i = 0; i < parts; i++){
	    TransformationNode part;
            part.handle = _s.names[i % _s.names.size()];
	    part.matrix[2][3] = offset;
	    part.matrix[2][2] = 1.0f / parts;
	    part.tex_off_v = offset;
	    part.tex_len_v = 1.0f/parts;	
	    offset += 1.0f / parts;
	    children.push_back(part);
	}
    } else if (_s.rule == SPLIT){
        float offset = -0.5f;	
	float tex_offset = 0.0f;
        for(int j = 0; j < _s.args[0].size(); j++) {
	    TransformationNode part;
            part.handle = _s.names[j % _s.names.size()];
            offset += _s.args[0][j] / 200.0f;
            part.matrix[0][3] = offset;
            part.matrix[0][0] = _s.args[0][j] / 100.0f;
	    part.tex_off_u = tex_offset;
	    part.tex_len_u = _s.args[0][j] / 100.0f;
	    tex_offset += _s.args[0][j] / 100.0f;	
            offset += _s.args[0][j] /200.0f;
            children.push_back(part);
	}	
    } else if (_s.rule == UNIFORM_SPLIT){
	int parts = _s.args[0][0];
        float offset = -0.5f;
	float tex_offset = 0.0f;
        for(int i = 0; i < parts; i++) {
            TransformationNode part;
            part.handle = _s.names[i % _s.names.size()];
            offset += 0.5f / parts;
            part.matrix[0][3] = offset;
            part.matrix[0][0] = 1.0f / parts;
	    part.tex_off_u = tex_offset;
	    part.tex_len_u = 1.0f/parts;
	    tex_offset += 1.0f/parts;	
            offset += 0.5f / parts;
            children.push_back(part);
        }
    } else if (_s.rule == REPEAT_SPLIT) {
        int parts = _s.args[0][0];
        float offset = -0.5f;
	float tex_offset = 0.0f;
        for(int i = 0; i < parts; i++) {
            for(int j = 0; j < _s.args[0].size() - 1; j++) {
                TransformationNode part;
                part.handle = _s.names[j % _s.names.size()];
                offset += (_s.args[0][j + 1] / parts)/200.0f;
                part.matrix[0][3] = offset;
                part.matrix[0][0] = (_s.args[0][j + 1] / parts) / 100.0f;
		part.tex_off_u = tex_offset;
	        part.tex_len_u = (_s.args[0][j + 1] / parts) / 100.0f;	
		tex_offset += (_s.args[0][j + 1] / parts) / 100.0f;
                offset += (_s.args[0][j + 1] / parts)/200.0f;
                children.push_back(part);
            }
        }
    } else if (_s.rule == CENTER) {
        TransformationNode center_lot;
        center_lot.handle = _s.names[0];
        center_lot.matrix[0][0] = _s.args[0][0];
        center_lot.matrix[1][1] = _s.args[0][1];
        center_lot.type = LOT;
        children.push_back(center_lot);
    } else if(_s.rule == CORNERS) { // for only rectangular lots
        for(int i = 0; i < 4; i++) {
            if(_s.args[0][2+i] < 1)
                continue;
            TransformationNode corner_lot;
            corner_lot.handle = _s.names[i % _s.names.size()];
            corner_lot.matrix[0][0] = _s.args[0][0];
            corner_lot.matrix[1][1] = _s.args[0][1];
            if(i < 2)
                corner_lot.matrix[0][3] = 0.5;
            else
                corner_lot.matrix[0][3] = -0.5;
            if(i == 0 || i == 3)
                corner_lot.matrix[1][3] = -0.5;
            else
                corner_lot.matrix[1][3] = 0.5;
            corner_lot.type = CIRCULAR_CORNER_LOT;
            children.push_back(corner_lot);
        }
    } else if (_s.rule == EDGES) {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j <_s.args[0][2+i]; j++) {
                TransformationNode edge_lot;
                edge_lot.handle = _s.names[i % _s.names.size()];
                edge_lot.matrix[0][0] = _s.args[0][0];
                edge_lot.matrix[1][1] = _s.args[0][1];
                if(i == 0) {
                    edge_lot.matrix[0][3] = 0.5;
                    edge_lot.matrix[1][3] = (j + 1)*(1.0 / (_s.args[0][2 + i] + 1)) - 0.5;
                }
                if(i == 1) {
                    edge_lot.matrix[0][3] = (j + 1)*(1.0 / (_s.args[0][2 + i] + 1)) - 0.5;
                    edge_lot.matrix[1][3] = 0.5;
                }
                if(i == 2) {
                    edge_lot.matrix[0][3] = -0.5;
                    edge_lot.matrix[1][3] = (j + 1)*(1.0 / (_s.args[0][2 + i] + 1)) - 0.5;
                }
                if(i == 3) {
                    edge_lot.matrix[0][3] = (j + 1)*(1.0 / (_s.args[0][2 + i] + 1)) - 0.5;
                    edge_lot.matrix[1][3] = -0.5;
                }
                edge_lot.type = EDGE_LOT;
                children.push_back(edge_lot);
            }
        }
    }
}

void recurse(Model &full, TransformationNode node) {
    std::vector<Component> comps = comp_map[node.handle];
    std::vector<TransformationNode> children;
    Model model;
    for(int i = 0; i < comps.size(); i++) {
        int rr = rand();
        float r = (float)rr / RAND_MAX;
        bool branch_found = false;
        for(int j = 0; j < comps[i].children.size(); j++) {
            if(r < comps[i].weights[j] || comps[i].weights[j] > 0.999999f) {
                Successor s = comps[i].children[j];
                if(s.rule == INSERT) {
		    if(leaf_map.find(s.names[0]) == leaf_map.end()){
			if(s.args[0][1] == 1) // keep materials local
			    readObj(s.names[0].c_str(), model, true);
			else // else make them global and changeable from parent rule files 
			    readObj(s.names[0].c_str(), model, false);
			leaf_map[s.names[0]] = model;
		    } else {
			model = leaf_map[s.names[0]];
		    }
		    if(s.args[0][2] == 1) // do NOT scale the Z
			alignAndFitToOrigin(model, false);
		    else
			alignAndFitToOrigin(model, true);
		    
		    if(s.args[0][0] == 1){	
			//set the model as pre-textured and ignore the inherited texture
			model.is_textured = true;
		    }
                } else if(s.rule == CYLINDER) {
		    getCylinder(s.args[0][0], s.args[0][1], s.args[0][2], s.args[0][3], model); //bottom-rad, top-rad, height, div
                } else if(s.rule == ROUND_DOOR) {
		    getRoundedDoor(s.args[0][0], s.args[0][1], s.args[0][2], s.args[0][3], model); //height, breadth, thickness, div
                } else if (s.rule == CONE_ROOF) {
		    getCone(s.args[0][0], s.args[0][1], s.args[0][2], model); //radius, height, divisions
                } else if (s.rule == PAR_CONE_ROOF) {
		    getParabolicCone(s.args[0][0], s.args[0][1], s.args[0][2], s.args[0][3], model); //radius, height, h-div, v-div
                } else if (s.rule == VOLUME) {
		    if(s.args[0][5] > 0){
		        getVolume(s.args[0][0], s.args[0][1], s.args[0][2], s.args[0][3], s.args[0][4], model); //length, breadth, height, x-elev, y-elev
		    }
		    if(s.names.size() > 0) {
                        //Have to assign a lot to the top of this structure
                        //to estimate : tz, sx, sy
                        TransformationNode top;
                        top.handle = s.names[0];
                        top.matrix[2][3] = s.args[0][2];
                        top.matrix[0][0] = (1 - s.args[0][3]) * s.args[0][0];
                        top.matrix[1][1] = (1 - s.args[0][4]) * s.args[0][1];
                        top.type = LOT;
                        if(top.matrix[0][0] > EPS && top.matrix[1][1] > EPS)
                            children.push_back(top);
		    }
                } else {
                    compute_children(children, s);
                }
                if(comps[i].weights[j] < 0.999999f) {
                    branch_found = true;
                    r = 1.0f;
                }
            } else {
                //printf("\t\t\t\t%s | r = %f | weight = %f rejected\n", node.handle.c_str(), r, comps[i].weights[j]);
                if(!branch_found) {
                    r -= comps[i].weights[j];
                }
            }
        }
    }	
    //printf("Applying transformation with (len v, off v) = %f %f\n", node.tex_len_v, node.tex_off_v);
    applyTransformation(model, node);
    for(int i = 0; i < children.size(); i++) {
        Model m;
        getNewGlobalTransformationMatrix(node, children[i]);
        if(children[i].type == CIRCULAR_CORNER_LOT) {
            float min_diag = std::min(children[i].matrix[0][0], children[i].matrix[1][1]);
            float min_off_diag = std::min(children[i].matrix[0][1], children[i].matrix[1][0]);
            children[i].matrix[0][0] = children[i].matrix[1][1] = min_diag;
            children[i].matrix[0][1] = children[i].matrix[1][0] = min_off_diag;
        }
        recurse(m, children[i]);
        if(children[i].type != NORMAL_CHILD) {
            BBox S;
            getBBox(m, S);
            float min_dim = std::min(S.max.z - S.min.z, std::min(S.max.y - S.min.y, S.max.x - S.min.x));
            float max_dim = std::max(S.max.z - S.min.z, std::max(S.max.y - S.min.y, S.max.x - S.min.x));
            if(max_dim < 50 * min_dim)
                combine(model, m);
        } else {
            combine(model, m);
        }
    }
    combine(full, model);
}

void process(Model &final) {
    /*
    * First the lots are processed. There are some parent lots, in the lot_map, all other lots are obtained by transformations.
    * Every child obtained by a transformation, carries only information about the transformation matrix. Geometry is only processed at the leaf nodes and is carried upwards.
    */

    //Lots
    for(std::map<std::string, Component>::iterator it = lot_map.begin(); it != lot_map.end(); it++) {
        for(int i = 0; i < it->second.children[0].args[0].size(); i++) {
            it->second.children[0].args[0][i] =
                rand() * (it->second.children[0].args[1][i] - it->second.children[0].args[0][i]) / RAND_MAX + it->second.children[0].args[0][i];
	    //printf("%s | %d | %f\n", it->first.c_str(), i, it->second.children[0].args[0][i]);
        }
        TransformationNode parent_lot;
        parent_lot.handle = it->first;
        if(it->second.children[0].rule == CIRC) {
            parent_lot.matrix[0][3] = it->second.children[0].args[0][0];
            parent_lot.matrix[1][3] = it->second.children[0].args[0][1];
            parent_lot.matrix[0][0] = it->second.children[0].args[0][2];
            parent_lot.matrix[1][1] = it->second.children[0].args[0][2];
        }
        else if(it->second.children[0].rule == RECT) {
            parent_lot.matrix[0][3] = (it->second.children[0].args[0][0] + it->second.children[0].args[0][2])/2.0;
            parent_lot.matrix[1][3] = (it->second.children[0].args[0][1] + it->second.children[0].args[0][3])/2.0;
            parent_lot.matrix[0][0] = it->second.children[0].args[0][2] - it->second.children[0].args[0][0];
            parent_lot.matrix[1][1] = it->second.children[0].args[0][3] - it->second.children[0].args[0][1];
        }
        parent_lot.type = LOT;
        lot_nodes.push_back(parent_lot);
    }
    while(lot_nodes.size() > 0) {
        Model m;
        recurse(m, lot_nodes[0]);
        combine(final, m);
        lot_nodes.erase(lot_nodes.begin());
    }
    //scaleTextures(final);
}

void parseFile(const char *filename, Model &model) {
    line_number = 1;
    yyin = fopen(filename, "r");
    do {
        yyparse();
    } while (!feof(yyin));
/*
    //debug print statements start
        for(std::map<std::string, float>::iterator it = attr_map.begin(); it!=attr_map.end(); it++) {
            printf("%s --> %f\n", it->first.c_str(), it->second);
        }
        for(std::map<std::string, std::vector<Component> >::iterator it = comp_map.begin(); it != comp_map.end(); it++) {
            printf("\n--------------\nName : %s\n", it->first.c_str());
            for(int k = 0; k < it->second.size(); k++) {
                for(int i = 0 ; i < it->second[k].children.size(); i++) {
                    printf("\nRule : %d\nWeight : %f\nSuccessor : ", it->second[k].children[i].rule, it->second[k].weights[i]);
                    for(int j = 0; j < it->second[k].children[i].names.size(); j++)
                        printf(" %s |", it->second[k].children[i].names[j].c_str());
                    printf("\nArgs : ");
                    for(int j = 0; j < it->second[k].children[i].args[0].size(); j++)
                        printf(" [%f, %f] |", it->second[k].children[i].args[0][j], it->second[k].children[i].args[1][j]);
                }
            }
        }
        printf("\n----------END-----------\n");
    //debug print statements end
*/
    if(errors.size() == 0){
	struct timeval new_time;
	gettimeofday(&new_time,NULL);
	srand((new_time.tv_sec * 1000) + (new_time.tv_usec / 1000));
        process(model);
    }
    attr_map.clear();
    lot_map.clear();
    comp_map.clear();
    lot_nodes.clear();
    leaf_map.clear();
}
