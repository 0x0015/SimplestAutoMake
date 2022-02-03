#include "ParseSAMP.hpp"
#include "SimpleCppTextFileHandler/file.hpp"
#include <algorithm>

std::vector<std::string> SplitString(std::string str,std::string delimeter){
    std::vector<std::string> splittedStrings = {};
    size_t pos = 0;

    while ((pos = str.find(delimeter)) != std::string::npos)
    {
        std::string token = str.substr(0, pos);
        if (token.length() > 0)
            splittedStrings.push_back(token);
        str.erase(0, pos + delimeter.length());
    }

    if (str.length() > 0)
        splittedStrings.push_back(str);
    return splittedStrings;
}

std::string & ltrim(std::string & str)
{
  auto it2 =  std::find_if( str.begin() , str.end() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
  str.erase( str.begin() , it2);
  return str;   
}

std::string & rtrim(std::string & str)
{
  auto it1 =  std::find_if( str.rbegin() , str.rend() , [](char ch){ return !std::isspace<char>(ch , std::locale::classic() ) ; } );
  str.erase( it1.base() , str.end() );
  return str;   
}
std::string trim(std::string& str){
	std::string s = str;
	return(ltrim(rtrim(s)));
}

void readSAMP(std::unordered_map<std::string, std::string>& Rules, std::string& filename){
	std::string file = readFile(filename);
	if(file == ""){
		std::cout<<"Unable to read file/contents was empty: "<<filename<<std::endl;
	}
	std::vector<std::string> lines = SplitString(file, "\n");
	for(int i=0;i<lines.size();i++){
		std::vector<std::string> ruleText = SplitString(lines[i], ":");
		uint8_t splitType = 0;//definitely there won't be more than 256 different split types, if not 2
		if(ruleText.size() != 2){
			ruleText = SplitString(lines[i], ":::");
			splitType = 0;
			if(ruleText.size() != 2){
				ruleText = SplitString(lines[i], ":+:");
				splitType = 1;
				if(ruleText.size() != 2){
					std::cout<<"Unable to seperate key and replacement from: "<<lines[i]<<std::endl;
					continue;
				}
			}
		}
		std::string key = trim(ruleText[0]);
		std::string rule = trim(ruleText[1]);
		switch(splitType){
			case 0:
				Rules["{" + key + "}"] = rule;
				break;
			case 1:
				Rules["{" + key + "}"].append(" " + rule);
				break;
		}
	}
}
