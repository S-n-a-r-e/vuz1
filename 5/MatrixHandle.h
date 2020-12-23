#include "Matrix.h"
#include <fstream>

#define MAIN_MENU 0

#define ADD_MATRIX 1
#define DELETE_MATRIX 2
#define GET_MATRIX 3
#define FILL_MATRIX 4
#define GET_LIST 5
#define SAVE_TO_FILE 6
#define IMPORT_FROM_FILE 7

class ArrayOfMatrix {
	unsigned int matricesCount = 2, matricesInitialized = 0;;
	unsigned int currentActiveArray = 0, currentInactiveArray = 1;

	Matrix*** mtxArr = new Matrix ** [2];

	char*** names = new char** [2];

	char** matrixAtIndexExists = new char* [2];

	int currentMode = MAIN_MENU;




	char mainCommands[7][7] = {
		{"new"},
		{"delete"},
		{"get"},
		{"fill"},
		{"list"},
		{"save"},
		{"import"}
	};
	int mainCommandsLengths[7] = {
		3, 6, 3, 4, 4, 4, 6
	};



	
private:
	void outputNames();

	void addMatrix(const char const* nameBeginPtr);

	void addMatrix(const char const* nameBeginPtr, std::ifstream file);

	void deleteMatrix(const char const* name);

	int getMatrixIndex(const char const* name);

	void fillMatrix(const char const* nameBeginPtr);

	void increaseArraySize();

	void importFromFile();

	void saveInFile();



public:
	ArrayOfMatrix();

	~ArrayOfMatrix();

	void action();
};