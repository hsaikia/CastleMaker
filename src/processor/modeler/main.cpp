#include <cstdio>
#include <map>
#include "model_processor.h"
#include "objprocessor.h"

std::string DATA_DIR = "./models/";
std::string HOME = ".";
std::map <std::string, Material > material_map;
std::vector<std::string> errors;

int main(int argc, char *argv[]){
	if(argc < 3){
		printf("Usage : %s <rule file> <output file without extension>\n", argv[0]);
		return 0;
	}
	Model m;
	readMtl("default.mtl", true);
	parseFile(argv[1], m);
	if(errors.size() > 0){
	    printf("The following errors were found in the Rule file! Exiting!\n");
	    for(int i = 0; i < errors.size(); i++){
		printf("%s\n", errors[i].c_str());
	    }
	    exit(1);
	}
	printf("Total Vertices in the model : %d\n", (int)(m.vertices.size()));
	printf("Total Faces in the model : %d\n", (int)(m.faces.size()));
	writeObj(argv[2], m);
    
}
