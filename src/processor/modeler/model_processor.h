#ifndef MODEL_PROCESSOR_H_
#define MODEL_PROCESSOR_H_

#include "model.h"
#include "util.h"
#include "component.h"
#include "objprocessor.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>
#include <vector>
#include <queue>
#include <ctime>
#include "sys/time.h"

void parseFile(const char *filename, Model& model);

#endif
