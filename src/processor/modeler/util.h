#ifndef UTIL_H_
#define UTIL_H_

#define _PI 3.14159265
#define _FLT_MAX 1.0e37
#define EPS 0.0000001
#include <string>
#include <vector>

typedef struct Point{
	float x;
	float y;
	float z;
	Point(){
	}
	Point(float _x, float _y, float _z){
		x = _x; y = _y; z = _z;
	}
} Point;

typedef struct BBox {
	Point min;
	Point max;
} BBox;

void rem_ws(std::string& line);
void trim(std::string& line);
void split(std::string line, std::vector<char> &operators, std::vector<std::string> &tokens);
void split_del(std::string str, std::vector<std::string> &toks, char delimiter);

#endif
