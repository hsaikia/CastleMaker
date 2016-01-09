#ifndef OBJPROCESSOR_H_
#define OBJPROCESSOR_H_

#include "util.h"
#include "model.h"
#include <fstream>
#include <cstdlib>
#include <string>

void sortFacesByMaterial(Model& model);
void readObj(const char* filename, Model& model, bool areMaterialsLocal);
void writeObj(const char* filename, Model model);
void readMtl(std::string filename, bool areMaterialsLocal);

#endif
