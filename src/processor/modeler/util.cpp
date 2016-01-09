#include "util.h"

//removes all occurences of white spaces in a string
void rem_ws(std::string& line){
	std::string ret = "";
	for(int i=0; i<line.length(); i++){
		if(line[i] != ' ')
			ret += line[i];
	}
	line = ret;
}

//removes forward and trailing spaces in a string
void trim(std::string& line){
	int start = 0;
	int end = line.length() - 1;
	while(true){
		if(line[start] != ' ')
			break;
		start++;
	}
	while(true){
		if(line[end] != ' ' && line[end] != '\r')
			break;
		end--;
	}
	if(start <= end)
	line = line.substr(start, end - start + 1);
}

//splits a string into two arrays of a)tokens and b)special characters (all whitespaces are removed before processing!!)
void split(std::string line, std::vector<char> &operators, std::vector<std::string> &tokens){
	rem_ws(line);
	std::string curr_token = "";
	for(int i=0; i<line.length(); i++){
		if(line[i] != '=' && line[i] != '$' && line[i] != ':' && line[i] != '(' && line[i] != ')' && line[i] != '|' && 
			line[i] != '{' && line[i] != '}' && line[i] != ';' && line[i] != '*' && line[i] != ','){
			curr_token += line[i];
		} else {
			operators.push_back(line[i]);
			tokens.push_back(curr_token);
			curr_token = "";	
		}
	}
}

//splits a string into tokens seperated by the given delimiter, doesn't return empty strings if delimiter is the 'space' character!
void split_del(std::string str, std::vector<std::string> &toks, char delimiter){
	int pos = 0; 
	while(str.length() > 0){
		trim(str);
		pos = str.find(delimiter);
		if(pos > 0){
			toks.push_back(str.substr(0, pos));
		} else if (pos == 0) {	
			if(delimiter != ' '){
				toks.push_back("");
			}
		} else {
			pos = str.length() - 1;
			toks.push_back(str);
		}
		str.erase(0, pos + 1);
	}
}


