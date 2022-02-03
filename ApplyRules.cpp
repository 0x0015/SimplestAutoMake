#include "ApplyRules.hpp"

std::string applyRules(std::unordered_map<std::string, std::string>& Rules, std::string& file){
	std::string output;
	for(int i=0;i<file.length();i++){
		bool foundRule = false;
		for(const auto& it : Rules){
			if(i+it.first.length() < file.length()){
				if(file.substr(i, it.first.length()) == it.first){
					output += it.second;
					i += it.first.length()-1;//because it will be incrimented in the for loop.
					foundRule = true;
					break;//we could keep searching to se if we find a better match, but for not it's not worth it.
				}
			}
		}
		if(!foundRule){
			output += file[i];
		}
	}
	return(output);
}
