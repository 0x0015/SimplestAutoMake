#include "PreprocessSAMP.hpp"
#include "SimpleCppTextFileHandler/file.hpp"
#include <filesystem>

std::vector<std::string> SplitString(std::string str,std::string delimeter);//just need one def, not whole header

std::vector<std::string> readFiles;
std::filesystem::path cwd;

std::string getQuotesValue(std::string& base, int initialI = 0){
	std::string output;
	bool fp = false;
	for(int i=initialI;i<base.length();i++){
		if(fp){
			if(base[i] == '\"'){
				return(output);
			}else{
				output += base[i];
			}
		}else{
			if(base[i] == '\"'){
				fp = true;
			}
		}
	}
	return("");
}

std::vector<std::string> readSAMP(std::string& filename){
	for(int i=0;i<readFiles.size();i++){
		if(readFiles[i] == filename){
			std::cout<<"Preprocessor Error:  recursive include of "<<filename<<std::endl;
			return(std::vector<std::string>());
		}
	}
	readFiles.push_back(filename);
	if(!fileExists((cwd/filename).string())){
		std::cout<<"Error: file doesn't exists: "<<filename<<std::endl;
		return(std::vector<std::string>());
	}
	std::string file = readFile((cwd/filename).string());
	if(file == ""){
		std::cout<<"Error: file contents was empty: "<<filename<<std::endl;
		return(std::vector<std::string>());
	}
	std::vector<std::string> lines = SplitString(file, "\n");
	int linesTrue = 0;
	std::vector<std::string> output;
	for(int i=0;i<lines.size();i++){
		if(lines[i].length() > 0 && lines[i][0] == '#'){
			if(lines[i].length() > 10 && lines[i].substr(1, 7) == "include"){
				std::string quotes = getQuotesValue(lines[i], 9);
				if(quotes == ""){
					std::cout<<"Preprocessor Error:  unable to parse include statement: "<<lines[i]<<" ("<<filename<<"["<<linesTrue<<"])"<<std::endl;
				}else{
					if(quotes.find("/") != std::string::npos){
						std::filesystem::path newFilePos = cwd/quotes;
						if(newFilePos.has_filename() && newFilePos.has_parent_path()){
							std::string newFilename = newFilePos.filename().string();
							cwd = newFilePos.parent_path();
							std::vector<std::string> newLines = readSAMP(newFilename);
							for(int j=0;j<newLines.size();j++){
								output.push_back(newLines[j]);
							}
						}else{
							std::cout<<"Preprocessor Error:  \""<<quotes<<"\" is not a valid path"<<" ("<<filename<<"["<<linesTrue<<"])"<<std::endl;

						}
						
					}else{
						std::vector<std::string> newLines = readSAMP(quotes);
						for(int j=0;j<newLines.size();j++){
							output.push_back(newLines[j]);
						}
					}
				}
			}else{
				std::cout<<"Preprocessor Error:  unknown preprocessor flag: "<<lines[i]<<" ("<<filename<<"["<<linesTrue<<"])"<<std::endl;

			}
		}else{
			output.push_back(lines[i]);
		}
		linesTrue++;
	}
	return(output);
}

std::vector<std::string> preprocessSAMP(std::string& filename){
	readFiles.clear();
	cwd = std::filesystem::current_path();
	std::vector<std::string> read = readSAMP(filename);
	return(read);
}

