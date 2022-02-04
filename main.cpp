#include <iostream>
#include <unordered_map>
#include "ArguementHandler/ArgHandle.hpp"
#include "SimpleCppTextFileHandler/file.hpp"
#include "platformHelper.hpp"
#include "ParseSAMP.hpp"
#include "PreprocessSAMP.hpp"
#include "ApplyRules.hpp"

void help(){
	std::cout<<"\tSimplestAutoMake v1.02\n\nHelp:  SAM [Makefile] {Options}\nOptions:\n\t[-h]: show this help page\n\t[-p {platform...}]: specify to use a specific platform file\n\t[-l]: list the rules that will be applied\n\t[-n]: don't generate a Makefile\n\n\tclean:  deletes Makefile if it exists."<<std::endl;
}

void platformHelp(){
	std::cout<<"Unable to determine default platform.  please use the -p flag to define a platform.  Valid platforms:\n\tWindows/Win\n\tLinux\n\tMacos/OSX\n\tFreeBSD/BSD."<<std::endl;
}

int main(int argc, char** argv){
	ArguementHandler args(argc, argv);
	if(args.hasArg("-h")){
		help();
		return(0);
	}

	if(args.hasArg("")){
		if(args.findArgs("")[0].value == "clean"){
			if(fileExists("Makefile")){
				deleteFile("Makefile");
				return(0);
			}
		}
	}
	
	std::string SAMFile = "";
	if(fileExists("Make.sam")){
		SAMFile = "Make.sam";
	}
	if(fileExists("Make.SAM")){
		SAMFile = "Make.SAM";
	}
	if(fileExists("Makefile.sam")){
		SAMFile = "Makefile.sam";
	}
	if(fileExists("Makefile.SAM")){
		SAMFile = "Makefile.SAM";
	}
	if(SAMFile == ""){
		std::cout<<"Error: Did not find SAM file."<<std::endl;
		return(1);
	}

	std::vector<std::string> Inputs;

	int platform = getPlatform();
	if(args.hasArg("-p")){
		auto pFArg = args.findArgs("-p");
		for(int i=0;i<pFArg.size();i++){
			std::string pArg = pFArg[i].value;
			int platformNum = getPlatform(pArg);
			if(platformNum != -1){
				platform = platformNum;
			}else{
				Inputs.push_back(pArg);
			}
		}
		if(pFArg.size() == 0){
			platformHelp();
			return(1);
		}
		if(pFArg.size() == 1 && pFArg[0].value == ""){
			platformHelp();
			return(1);
		}
	}else{
		if(platform == -1){
			platformHelp();
			return(1);
		}
	}

	if(platform != -1){
		Inputs.push_back(getPlatformName(platform));
	}
	
	if(Inputs.size() == 0){
		platformHelp();
		return(1);
	}

	std::vector<std::string> Files;
	bool SAMsFTF = false;

	for(int i=0;i<Inputs.size();i++){
		std::string inputFile = "";
		if(fileExists(Inputs[i])){
			inputFile = Inputs[i];
		}
		if(fileExists(Inputs[i] + ".samp")){
			inputFile = Inputs[i] + ".samp";
		}
		if(fileExists(Inputs[i] + ".SAMP")){
			inputFile = Inputs[i] + ".SAMP";
		}
		if(inputFile != ""){
			Files.push_back(inputFile);
		}else{
			std::cout<<"Unable to find SAMPs: "<<Inputs[i]<<std::endl;
			SAMsFTF = true;
		}
	}

	if(SAMsFTF){
		return(1);
	}

	std::vector<std::string> FilesSorted;
	for(int i=0;i<Files.size();i++){
		if(getPlatform(Files[i]) != -1){
			FilesSorted.push_back(Files[i]);
			Files.erase(Files.begin() + i);
			break;
		}
	}
	if(FilesSorted.size() == 0){//never encountered a platform
		platformHelp();
		return(-1);
	}
	for(int i=0;i<Files.size();i++){
		FilesSorted.push_back(Files[i]);
	}

	std::cout<<"Reading SAMPs: ";
	for(int i=0;i<FilesSorted.size();i++){
		std::cout<<FilesSorted[i];
		if(i+1<FilesSorted.size()){
			std::cout<<", ";
		}
	}
	std::cout<<std::endl;

	std::unordered_map<std::string, std::string> Rules;

	for(int i=0;i<FilesSorted.size();i++){
		std::vector<std::string> SAMP = preprocessSAMP(FilesSorted[i]);
		parseSAMP(Rules, SAMP);
	}

	if(args.hasArg("-l")){
		std::cout<<"Rules:"<<std::endl;
		for(const auto& it : Rules){
			std::cout<<it.first<<", "<<it.second<<std::endl;
		}
	}

	std::string SAMFileContent = readFile(SAMFile);
	std::string SAMOutput = applyRules(Rules, SAMFileContent);
	if(!args.hasArg("-n")){
		writeFile("Makefile", SAMOutput);
	}
	return(0);
}
