#ifndef FFG_H
#define FFG_H
//#define FASTFFG

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

struct Ffg {
private:
	bool delAfterRead = 0;
	std::vector<std::string*> blocks;
	std::string fileName;

	void parseMath(int from, int& to);

#ifndef FASTFFG
	void prepCommands();
	void prepMath();
#else
	void multiPrepare();
#endif
	void fastDef();

	bool isNumber(std::string block);
public:
	Ffg(const char* fname);
	~Ffg();
	void intExport(std::string property, int value);
	void strExport(std::string property, std::string value);

	int intGet(std::string name);
	std::string strGet(std::string name);

	void prepare();

};

#endif