#include "Matrix.h"
#include <fstream>

#define MAIN_MENU 0

#define ADD_MATRIX 101
#define DELETE_MATRIX 102
#define GET_MATRIX 103
#define FILL_MATRIX 104
#define GET_LIST 105
#define SAVE_TO_FILE 106
#define IMPORT_FROM_FILE 107
#define SET_MATRIX_ELEMENT 108

#define MATRIX_SETTING_MODE 109
#define MATRIX_MULTIPLY 110
#define MATRIX_PLUS 111




#define MATRIX_ERROR -1

#define STOP -3


class ArrayOfMatrix {
	unsigned int matricesCount = 2, matricesInitialized = 0;;
	unsigned int currentActiveArray = 0, currentInactiveArray = 1;

	Matrix*** mtxArr = new Matrix ** [2];

	char*** names = new char** [2];

	char** matrixAtIndexExists = new char* [2];

	int currentMode = MAIN_MENU;

	//availible commands if MAIN_MENU is set (user is in main menu)
	static const int commandsCount = 9;
	char mainCommands[commandsCount][7] = {
		{"new"},
		{"delete"},
		{"get"},
		{"fill"},
		{"list"},
		{"save"},
		{"stop"},
		{"import"},
		{"set"}
	};
	int mainCommandsLengths[commandsCount] = {
		3, 6, 3, 4, 4, 4, 4, 6, 3
	};
	
//utility
private:
	void addMatrix(const char const* nameBeginPtr, int elementsCount, std::ifstream& file);

	int getMatrixIndex(const char const* name);

	void increaseArraySize();

	bool nameValidation(const char const* nameBeginPtr);

//used in "action()"
private:
	int getCommand(const char const* command);

	void setMatrix(const char const* command);

	void outputNames();

	void addMatrix(const char const* nameBeginPtr);

	void deleteMatrix(const char const* name);

	void fillMatrix(const char const* nameBeginPtr);

	void setMatrixElement(const char const* nameBeginPtr);

	void importFromFile();

	void saveInFile();



public:
	ArrayOfMatrix();

	~ArrayOfMatrix();

	void action();
};