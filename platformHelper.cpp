#include "platformHelper.hpp"

int getPlatform(){//0=linux, 1=windows, 2=osx, 3=bsd
	#ifdef _WIN32
	return 1;
    #elif _WIN64
	return 1;
    #elif __APPLE__ || __MACH__
	return 2;
    #elif __linux__
	return 0;
    #elif __FreeBSD__
	return 3;
    #endif
	return(-1);
}

int getPlatform(std::string& pArg){
	if(pArg == "Windows"){
		return 1;
	}else if(pArg == "windows"){
		return 1;
	}else if(pArg == "Win"){
		return 1;
	}else if(pArg == "win"){
		return 1;
	}else if(pArg == "Linux"){
		return 0;
	}else if(pArg == "linux"){
		return 0;
	}else if(pArg == "Macos"){
		return 2;
	}else if(pArg == "macos"){
		return 2;
	}else if(pArg == "MacOS"){
		return 2;
	}else if(pArg == "MacOs"){
		return 2;
	}else if(pArg == "OSX"){
		return 2;
	}else if(pArg == "osx"){
		return 2;
	}else if(pArg == "FreeBSD"){
		return 3;
	}else if(pArg == "BSD"){
		return 3;
	}else if(pArg == "bsd"){
		return 3;
	}else{
		if(pArg.length() >= 5 && pArg.substr(0, 5) == "Linux"){
			return 0;
		}else if(pArg.length() >= 7 && pArg.substr(0, 7) == "Windows"){
			return 1;
		}else if(pArg.length() >= 5 && pArg.substr(0, 5) == "Macos"){
			return 2;
		}else if(pArg.length() >= 7 && pArg.substr(0, 7) == "FreeBSD"){
			return 3;
		}else{
			return -1;
		}
	}
}

std::string getPlatformName(int platform){
	switch(platform){
		case 0:
			return("Linux");
		case 1:
			return("Windows");
		case 2:
			return("Macos");
		case 3:
			return("FreeBSD");
		default:
			return("");
	}
	return("");
}
