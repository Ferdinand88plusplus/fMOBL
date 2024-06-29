#include "ffg.h"



enum PARSEMODES {
	BLOCKFIND,
	BLOCKREADING,
	STRINGREADING,
	COMMENTSKIP,
};

const std::string skipChars = " \t\n";
const std::string strChars = "\"'";
const std::string splitChars = "=@${}()";

enum FCOMMANDSIDS {
	PUSH_LEFT,
	PUSH_RIGHT,
	IMPORT,
	DELETE,
	IF,

	MAXFCOMMANDS
};
enum FCOMMUSEWITH {
	INT,
	STR,
	ALL,
	PROPARRAY,
	AFTER_READ,
	EXISTS,

	MAXFCOMMUSE,
};
const std::vector<std::string> fCommands = {
	"PUSH_LEFT",
	"PUSH_RIGHT",
	"IMPORT",
	"DELETE",
	"IF",
};
const std::vector<std::string> fExcludeComm = {
	"STOP",
};
const std::vector<std::string> fCommUseWith = {
	"INT",
	"STR",
	"ALL",
	"{",
	"AFTER_READ",
	"EXISTS",
};

const std::string intChars = "-0123456789";
const std::string mathOps = "/*+-";

enum MATHOPERATIONS {
	DIVIDE,
	MULTIPLY,
	PLUS,
	MINUS,

	MAXOPERATION,
};

void Ffg::parseMath(int from, int& to)
{
	
	for (int m = 0; m < MAXOPERATION; m++) {
		auto opPos = std::find_if(blocks.begin() + from, blocks.begin() + to, [m](std::string* block){
			return (block->size() == 1 && block->find( mathOps[m]) != std::string::npos);
			});
		if (opPos == blocks.begin() + to) {
			continue;
		}
		int offs = opPos - blocks.begin();

		/*
		if (*blocks[offs - 1] == "$") {
			for (int j = from; j > -1; j--) {
				if (*blocks[j] == *blocks[offs] && *blocks[j+1] == "=") {
					delete blocks[offs - 1];
					blocks.erase(blocks.begin() + (offs - 2), blocks.begin() + offs);
					*blocks[offs - 1] = *blocks[j + 2];
					to--;
				}
			}
		}
		if (*blocks[offs + 1] == "$") {
			for (int j = from; j > -1; j--) {
				if (*blocks[j] == *blocks[offs+2] && *blocks[j+1] == "=") {
					delete blocks[offs + 1];
					blocks.erase(blocks.begin() + (offs + 1), blocks.begin() + offs+2);
					*blocks[offs + 1] = *blocks[j + 2];
					to--;
				}
			}
		}*/
		
	

		int val1 = std::stoi(*blocks[offs - 1]);
		int val2 = std::stoi(*blocks[offs + 1]);

		switch (m) {
		case DIVIDE:
			val1 /= val2;
			break;
		case MULTIPLY:
			val1 *= val2;
			break;
		case PLUS:
			val1 += val2;
			break;
		case MINUS:
			val1 -= val2;
			break;
		}

		delete blocks[offs];
		delete blocks[offs + 1];
		blocks.erase(blocks.begin() + (offs ), blocks.begin() + (offs + 2));

		*blocks[offs - 1] = std::to_string(val1);

		to -= 2;
	}
}

#ifndef FASTFFG

void Ffg::prepCommands()
{
	for (int i = 0; i < blocks.size(); i++) {
		if (*blocks[i] == "@") {
			auto ffcomm = std::find(fCommands.begin(), fCommands.end(), *blocks[i + 1]);
			if (ffcomm == fCommands.end()) {
				auto ffexcl = std::find(fExcludeComm.begin(), fExcludeComm.end(), *blocks[i + 1]);
				if (ffexcl != fExcludeComm.end()) continue;
				std::cout << "FFG ERROR::Unknown Ffg command: " << *blocks[i + 1] << '\n';
				continue;
			}

			auto ffuse = std::find(fCommUseWith.begin(), fCommUseWith.end(), *blocks[i + 2]);

			if (ffuse == fCommUseWith.end()) {
				if (ffcomm - fCommands.begin() == IF) {
					ffuse = fCommands.begin();
				}
				std::cout << "FFG ERROR::Unknown data type to use Ffg command: "<<*blocks[i+2]<<'\n';
				continue;
			}

			int fId = ffcomm - fCommands.begin();
			int fType = ffuse - fCommUseWith.begin();
			std::vector<std::string> toFind;
			std::string fPut;
			Ffg* buffer = nullptr;
			if(fType == PROPARRAY){
				int j = i + 3;
				while (*blocks[j] != "}") {
					toFind.push_back(*blocks[j]);
					delete blocks[j];
					blocks.erase(blocks.begin() + j, blocks.begin() + j + 1);
				}
				delete blocks[j];
				blocks.erase(blocks.begin() + j, blocks.begin() + j + 1);
				fPut = *blocks[j];
			}
			else{
				fPut = *blocks[i + 3];
			}

			switch (fId) {
			case PUSH_LEFT:
				switch (fType) {
				case INT:
					for (int j = i+4; j < blocks.size(); j++)
					{

						std::string* block = blocks[j];
						if (*block == "@") {
							if (*blocks[j + 1] == "STOP" && *blocks[j + 2] == fPut) break;
						}
						char start = (*block)[0];
					
						if (isNumber(*block)) {
							std::string buffer = *block;
							//buffer.erase(0, 1);
							blocks[j] = new std::string;
							*blocks[j] += fPut;
							*blocks[j] += buffer;
						}
					}
					break;
				case STR:
					for (int j = i+4; j < blocks.size(); j++)
					{
					
						std::string* block = blocks[j];
						if (*block == "@") {
							if (*blocks[j + 1] == "STOP" && *blocks[j + 2] == fPut) break;
						}
						char start = (*block)[0];

						if (strChars.find(start) != std::string::npos ) {
							
							std::string buffer = *block;
							buffer.erase(0, 1);
							blocks[j] = new std::string;
							*blocks[j] += fPut;
							*blocks[j] += buffer;
						}
					}
					break;
				}
				break;
			case PUSH_RIGHT:
				switch (fType) {
				case INT:
					for (int j = i + 4; j < blocks.size(); j++)
					{

						std::string* block = blocks[j];
						if (*block == "@") {
							if (*blocks[j + 1] == "STOP" && *blocks[j + 2] == fPut) break;
						}
						char start = (*block)[0];

						if (isNumber(*block)) {
							std::string buffer = *block;
							//buffer.erase(0, 1);
							blocks[j] = new std::string;
							*blocks[j] += buffer;
							*blocks[j] += fPut;
						}
					}
					break;
				case STR:
					fPut.erase(0, 1);
					for (int j = i + 4; j < blocks.size(); j++)
					{

						std::string* block = blocks[j];
						if (*block == "@") {
							if (*blocks[j + 1] == "STOP" && blocks[j + 2]->substr(1, blocks[j+2]->size()-1) == fPut) break;
						}
						char start = (*block)[0];
						if (strChars.find(start) != std::string::npos) {

							std::string buffer = *block;
							blocks[j] = new std::string;
							*blocks[j] += buffer;
							*blocks[j] += fPut;
						}
						
					}
					break;
				}
				break;
			case IMPORT:
				buffer = new Ffg(fPut.substr(1, fPut.size() - 1).c_str());

				switch (fType) {
				case ALL:
					for (int j = 0; j < buffer->blocks.size(); j++) {
						std::string* blockbuf = new std::string;
						*blockbuf = *buffer->blocks[j];
						blocks.insert(blocks.begin() + j,blockbuf);
						i++;
					}
					delete buffer;
					break;
				case PROPARRAY:
					for (std::string fstr : toFind) {
						bool found = 0;
						for (int j = 0; j < buffer->blocks.size(); j++) {
							if (*buffer->blocks[j] == fstr && *buffer->blocks[j + 1] == "=") {
								for (int h = 0; h < 3; h++) {
									std::string* blockbuf = new std::string;
									*blockbuf = *buffer->blocks[j + h];
									blocks.insert(blocks.begin() + j + h, blockbuf);
									i++;
								}
								found = 1;
								break;
							}
						}
						if (!found) {
							std::cout << "FFG ERROR::Cant import property " << fstr << " from " << fPut << '\n';
							delete buffer;
							return;
						}
					}
					delete buffer;
					break;
				}
				break;
			case DELETE:
				switch (fType) {
				case AFTER_READ:
					delAfterRead = 1;
					break;
				}
				break;
			/*
			case IF:
				switch (fType) {
				case 0:
					break;
				case EXISTS:
					for (int j = i; j > -1; j--) {
						if (*blocks[j] == fPut && *blocks[j + 1] == "=") {

						}
					}
				}/*/
			}
			
		}
	}
}

void Ffg::prepMath()
{
	for (int i = 0; i < blocks.size(); i++) {
		std::string* block = blocks[i];

		if (*block == "{") {
			int end = -1;
			for (int j = i; j < blocks.size(); j++) {
				if (*blocks[j] == "}") {
					end = j;
					break;
				}
			}
			if (end == -1) {
				std::cout << "FFG ERROR::Cant find end of math operation! Block pos: " << i << '\n';
				return;
			}
			int brBeg = -1;
			int brEnd = -1;
			for (int j = i; j < end; j++) {
				if (*blocks[j] == "(") {
					brBeg = j;
					continue;
				}
				if (*blocks[j] == ")") {
					brEnd = j;
					int buf = brEnd;
					parseMath(brBeg, brEnd);
					delete blocks[brBeg];
					delete blocks[brEnd];
					blocks.erase(blocks.begin() + brBeg, blocks.begin() + brBeg + 1);
					blocks.erase(blocks.begin() + brEnd - 1, blocks.begin() + brEnd);

					int delta = buf - brEnd;
					end -= delta;

					brEnd = -1;
					brBeg = -1;
				}
			}
			end--;
			parseMath(i + 1, end);

			delete blocks[i];
			blocks.erase(blocks.begin() + i, blocks.begin() + i + 1);
		}
	}
}
// old prepare systems
#else

void Ffg::multiPrepare()
{

	fastDef();
	for (int i = 0; i < blocks.size(); i++) {
		std::string* block = blocks[i];

		if (*block == "@") {
			auto ffcomm = std::find(fCommands.begin(), fCommands.end(), *blocks[i + 1]);
			if (ffcomm == fCommands.end()) {
				auto ffexcl = std::find(fExcludeComm.begin(), fExcludeComm.end(), *blocks[i + 1]);
				if (ffexcl != fExcludeComm.end()) continue;
				std::cout << "FFG ERROR::Unknown Ffg command: " << *blocks[i + 1] << '\n';
				continue;
			}

			auto ffuse = std::find(fCommUseWith.begin(), fCommUseWith.end(), *blocks[i + 2]);

			if (ffuse == fCommUseWith.end()) {
				if (ffcomm - fCommands.begin() == IF) {
					ffuse = fCommands.begin();
				}
				std::cout << "FFG ERROR::Unknown data type to use Ffg command: " << *blocks[i + 2] << '\n';
				continue;
			}

			int fId = ffcomm - fCommands.begin();
			int fType = ffuse - fCommUseWith.begin();
			std::vector<std::string> toFind;
			std::string fPut;
			Ffg* buffer = nullptr;
			if (fType == PROPARRAY) {
				int j = i + 3;
				while (*blocks[j] != "}") {
					toFind.push_back(*blocks[j]);
					delete blocks[j];
					blocks.erase(blocks.begin() + j, blocks.begin() + j + 1);
				}
				delete blocks[j];
				blocks.erase(blocks.begin() + j, blocks.begin() + j + 1);
				fPut = *blocks[j];
			}
			else {
				fPut = *blocks[i + 3];
			}

			switch (fId) {
			case PUSH_LEFT:
				switch (fType) {
				case INT:
					for (int j = i + 4; j < blocks.size(); j++)
					{

						std::string* block = blocks[j];
						if (*block == "@") {
							if (*blocks[j + 1] == "STOP" && *blocks[j + 2] == fPut) break;
						}
						char start = (*block)[0];

						if (isNumber(*block)) {
							std::string buffer = *block;
							//buffer.erase(0, 1);
							blocks[j] = new std::string;
							*blocks[j] += fPut;
							*blocks[j] += buffer;
						}
					}
					break;
				case STR:
					for (int j = i + 4; j < blocks.size(); j++)
					{

						std::string* block = blocks[j];
						if (*block == "@") {
							if (*blocks[j + 1] == "STOP" && *blocks[j + 2] == fPut) break;
						}
						char start = (*block)[0];

						if (strChars.find(start) != std::string::npos) {

							std::string buffer = *block;
							buffer.erase(0, 1);
							blocks[j] = new std::string;
							*blocks[j] += fPut;
							*blocks[j] += buffer;
						}
					}
					break;
				}
				break;
			case PUSH_RIGHT:
				switch (fType) {
				case INT:
					for (int j = i + 4; j < blocks.size(); j++)
					{

						std::string* block = blocks[j];
						if (*block == "@") {
							if (*blocks[j + 1] == "STOP" && *blocks[j + 2] == fPut) break;
						}
						char start = (*block)[0];

						if (isNumber(*block)) {
							std::string buffer = *block;
							//buffer.erase(0, 1);
							blocks[j] = new std::string;
							*blocks[j] += buffer;
							*blocks[j] += fPut;
						}
					}
					break;
				case STR:
					fPut.erase(0, 1);
					for (int j = i + 4; j < blocks.size(); j++)
					{

						std::string* block = blocks[j];
						if (*block == "@") {
							if (*blocks[j + 1] == "STOP" && blocks[j + 2]->substr(1, blocks[j + 2]->size() - 1) == fPut) break;
						}
						char start = (*block)[0];
						if (strChars.find(start) != std::string::npos) {

							std::string buffer = *block;
							blocks[j] = new std::string;
							*blocks[j] += buffer;
							*blocks[j] += fPut;
						}

					}
					break;
				}
				break;
			case IMPORT:
				buffer = new Ffg(fPut.substr(1, fPut.size() - 1).c_str());
				i++;

				switch (fType) {
				case ALL:
					for (int j = 0; j < buffer->blocks.size(); j++) {
						std::string* blockbuf = new std::string;
						*blockbuf = *buffer->blocks[j];
						blocks.insert(blocks.begin() + j, blockbuf);
					}
					delete buffer;
					break;
				case PROPARRAY:
					for (std::string fstr : toFind) {
						bool found = 0;
						for (int j = 0; j < buffer->blocks.size(); j++) {
							if (*buffer->blocks[j] == fstr && *buffer->blocks[j + 1] == "=") {
								for (int h = 0; h < 3; h++) {
									std::string* blockbuf = new std::string;
									*blockbuf = *buffer->blocks[j + h];
									blocks.insert(blocks.begin() + j + h, blockbuf);
									i++;
								}
								found = 1;
								break;
							}
						}
						if (!found) {
							std::cout << "FFG ERROR::Cant import property " << fstr << " from " << fPut << '\n';
							delete buffer;
							return;
						}
					}
					delete buffer;
					break;
				}
				break;
			case DELETE:
				switch (fType) {
				case AFTER_READ:
					delAfterRead = 1;
					break;
				}
				break;
				/*
				case IF:
					switch (fType) {
					case 0:
						break;
					case EXISTS:
						for (int j = i; j > -1; j--) {
							if (*blocks[j] == fPut && *blocks[j + 1] == "=") {

							}
						}
					}/*/
			}

			continue;
		} // FFG commands
		if (*block == "{") {
			int end = -1;
			for (int j = i; j < blocks.size(); j++) {
				if (*blocks[j] == "}") {
					end = j;
					break;
				}
			}
			if (end == -1) {
				std::cout << "FFG ERROR::Cant find end of math operation! Block pos: " << i << '\n';
				return;
			}
			int brBeg = -1;
			int brEnd = -1;
			for (int j = i; j < end; j++) {
				if (*blocks[j] == "(") {
					brBeg = j;
					continue;
				}
				if (*blocks[j] == ")") {
					brEnd = j;
					int buf = brEnd;
					parseMath(brBeg, brEnd);
					delete blocks[brBeg];
					delete blocks[brEnd];
					blocks.erase(blocks.begin() + brBeg, blocks.begin() + brBeg + 1);
					blocks.erase(blocks.begin() + brEnd - 1, blocks.begin() + brEnd);

					int delta = buf - brEnd;
					end -= delta;

					brEnd = -1;
					brBeg = -1;
				}
			}
			end--;
			parseMath(i + 1, end);

			delete blocks[i];
			blocks.erase(blocks.begin() + i, blocks.begin() + i + 1);
			continue;
		} // FFG math parse
		if (*block == "[fname]") {
			*block = std::string('"'+fileName);
		} // does it needs a comment?
	}
}
#endif
void Ffg::fastDef()
{ 
	std::vector<std::string*> queue;
	for (int i = blocks.size()-1; i > -1; i--) {
		if (*blocks[i] == "$") {
			delete blocks[i];
			blocks.erase(blocks.begin() + i, blocks.begin() + i + 1);
			queue.push_back(blocks[i]);
			continue;
		}
		if (*blocks[i] == "=") {
			for (int j = 0; j < queue.size(); j++) {
				if (*blocks[i - 1] == *queue[j]) {

					*queue[j] = *blocks[i + 1];
					queue.erase(queue.begin() + j, queue.begin() + j + 1);
					j--;
				}
			}
		}
	}
	if (!queue.empty()) {
		std::cout << "STARTPRINT\n";
		for (std::string* b : blocks) std::cout << *b << '\n';
		std::cout << "ENDPRINT\n";
		for (std::string* q : queue) {
			std::cout << "FFG "<<fileName<<  " WARNING::Cant find define for property " << *q << '\n';
		}
	}
}


bool Ffg::isNumber(std::string block)
{
	for (char ch : block) {
		if (intChars.find(ch) == std::string::npos) {
			return 0;
		}
	}
	return 1;
}

Ffg::Ffg(const char* fname)
{
	std::ifstream in(fname);
	if (!in.is_open()) {
		std::cout << "FFG ERROR::Cant open ffg file with name " << fname << '\n';
		return;
	}
	fileName = std::string(fname);
	std::string* curBlock = new std::string;
	char ch;
	int mode = BLOCKFIND;
	while (in.get(ch)) {
		switch (mode)
		{
		case COMMENTSKIP:
			if (ch == '\n') {
				mode = BLOCKFIND;
				break;
			}
			break;
		case BLOCKFIND:
			if (skipChars.find(ch) == std::string::npos) {
				if (ch == '#') {
					mode = COMMENTSKIP;
					break;
				}
				if (strChars.find(ch) != std::string::npos) {
					*curBlock += ch;
					mode = STRINGREADING;
					break;
				}
				*curBlock += ch;
				if (splitChars.find(ch) != std::string::npos) {
					blocks.push_back(curBlock);
					curBlock = new std::string;
				}
				mode = BLOCKREADING;
				break;
			}
			break;
		case BLOCKREADING:
			if (skipChars.find(ch) != std::string::npos) {
				if(!curBlock->empty()) blocks.push_back(curBlock);
				curBlock = new std::string;
				mode = BLOCKFIND;
				break;
			}
			else {
				if (splitChars.find(ch) != std::string::npos) {
					if (!curBlock->empty()) {
						blocks.push_back(curBlock);
						curBlock = new std::string;
					}
					*curBlock += ch;
					blocks.push_back(curBlock);
					curBlock = new std::string;
					break;
				}
				*curBlock += ch;
				break;
			}
			break;
		case STRINGREADING:
			if (strChars.find(ch) != std::string::npos) {
				blocks.push_back(curBlock);
				curBlock = new std::string;
				mode = BLOCKFIND;
				break;
			}
			else {
				*curBlock += ch;
				break;
			}
			break;
		}
	}
	in.close();
}

Ffg::~Ffg()
{
	for (int i = 0; i < blocks.size(); i++) {
		delete blocks[i];
	}
}

void Ffg::intExport(std::string property, int value)
{
	for (int i = 0; i < blocks.size(); i++) {
		if (*blocks[i] == property && *blocks[i + 2] == "[import]") {
			*blocks[i + 2] = std::to_string(value);
		}
	}
}

void Ffg::strExport(std::string property, std::string value)
{
	for (int i = 0; i < blocks.size(); i++) {
		if (*blocks[i] == property && *blocks[i + 2] == "[import]") {
			*blocks[i + 2] = '"'+value;
		}
	}
}

void Ffg::prepare()
{
#ifndef FASTFFG
	prepCommands();
	fastDef();
	prepMath();
#else
	multiPrepare();
#endif
}

int Ffg::intGet(std::string name)
{
	
	int fOffs = 0;
	bool found = 0;
	for (std::string* block : blocks) {

		if (*block == name) {
			found = 1;
			break;
		}
		fOffs++;
	}
	if (!found) {
		std::cout << "FFG ERROR::Cant find propery with name " << name << '\n';
		return -1;
	}
	std::string* block = blocks[fOffs];
	std::string* nextBlock = blocks[fOffs+1];

	if (*nextBlock == "=") {
		int buf = std::stoi(*blocks[fOffs + 2]);
		if (delAfterRead) {
			delete block;
			delete nextBlock;
			delete blocks[fOffs + 2];
			blocks.erase(blocks.begin() + fOffs, blocks.begin() + fOffs+ 3);
		}
		return buf;
	}
	else {
		std::cout << "FFG ERROR::Cant get value of property with name " << name << '\n';
		return -1;
	}
	
	std::cout << "FFG ERROR::Unknown error while trying to get value of propery " << name << '\n';
	return -1;
}

std::string Ffg::strGet(std::string name)
{
	int fOffs = 0;
	bool found = 0;
	for (std::string* block : blocks) {

		if (*block == name) {
			found = 1;
			break;
		}
		fOffs++;

	}
	if (!found) {
		std::cout << "FFG ERROR::Cant find property with name " << name << '\n';
		return "";
	}
	std::string* block = blocks[fOffs];
	std::string* nextBlock = blocks[fOffs + 1];

	if (*nextBlock == "=") {
		std::string buf = blocks[fOffs + 2]->substr(1, blocks[fOffs + 2]->size() - 1);
		if (delAfterRead) {
			delete block;
			delete nextBlock;
			delete blocks[fOffs + 2];
			blocks.erase(blocks.begin() + fOffs, blocks.begin()+fOffs + 3);
		}
		return buf;
	}
	else {
		std::cout << "FFG ERROR::Cant get value of property with name " << name << '\n';
		return "";
	}

	std::cout << "FFG ERROR::Unknown error while trying to get value of propery " << name << '\n';
	return "";
}
