#include "MatrixHandle.h"
#include <iostream>

ArrayOfMatrix::ArrayOfMatrix() {
	mtxArr[currentActiveArray] = new Matrix * [matricesCount];

	names[currentActiveArray] = new char* [matricesCount];
	matrixAtIndexExists[currentActiveArray] = new char[matricesCount];
	for (int i = 0; i < matricesCount; ++i) {
		names[currentActiveArray][i] = new char[21];

		matrixAtIndexExists[currentActiveArray][i] = 0;
	}

}

ArrayOfMatrix::~ArrayOfMatrix() {
	for (int i = 0; i < matricesInitialized; ++i) {
		if (matrixAtIndexExists[currentActiveArray][i] == 1) {
			delete mtxArr[currentActiveArray][i];
		}

		delete[] names[currentActiveArray][i];
	}

	delete[] mtxArr[currentActiveArray];
	delete[] mtxArr;

	delete[] names[currentActiveArray];
	delete[] names;

	delete[] matrixAtIndexExists[currentActiveArray];
	delete[] matrixAtIndexExists;
}

//function for dynamic array increase
void ArrayOfMatrix::increaseArraySize() {
	bool foundEmptySlot = false;
	for (int i = 0; i < matricesInitialized; ++i) {
		if (matrixAtIndexExists[currentActiveArray][i] == 0) {
			foundEmptySlot = true;
			break; 
		}
	}

	if ((foundEmptySlot == false) && (matricesCount == matricesInitialized)) {
		//increasing size
		matricesCount += (matricesCount / 2 < 1 ? 1 : matricesCount / 2);

		//rewriting data to another array with increased capacity
		mtxArr[currentInactiveArray] = new Matrix * [matricesCount];
		for (int i = 0; i < matricesInitialized; ++i) {
			mtxArr[currentInactiveArray][i] = new Matrix(*mtxArr[currentActiveArray][i]);
		}
		//clearing old array
		delete[] mtxArr[currentActiveArray];

		

		//allocating new memory for names
		names[currentInactiveArray] = new char* [matricesCount];
		for (int i = 0; i < matricesCount; ++i) {
			names[currentInactiveArray][i] = new char[21];
		}
		//clearing old names array and rewiting names data
		for (int i = 0; i < matricesInitialized; ++i) {
			for (int j = 0; j < 21; ++j) {
				names[currentInactiveArray][i][j] = names[currentActiveArray][i][j];
			}
			delete[] names[currentActiveArray][i];
		}
		delete[] names[currentActiveArray];



		//rewriting allocation data with allocationg new memory
		matrixAtIndexExists[currentInactiveArray] = new char[matricesCount];
		for (int i = 0; i < matricesInitialized; ++i) {
			matrixAtIndexExists[currentInactiveArray][i] = matrixAtIndexExists[currentActiveArray][i];
		}
		for (int i = matricesInitialized; i < matricesCount; ++i) {
			matrixAtIndexExists[currentInactiveArray][i] = 0;
		}
		delete[] matrixAtIndexExists[currentActiveArray];

		//swapping active and inactive arrays
		int tmpSwitch = currentInactiveArray;
		currentInactiveArray = currentActiveArray;
		currentActiveArray = tmpSwitch;

		//std::cout << "array increased\n";
	}
}

//returns amount of english alphabet symbols between first letter and last
int nameLength(const char const* nameBeginPtr) {
	int i;
	for (i = 0; i < 20; ++i) {
		if ( !((nameBeginPtr[i] != 0) && (nameBeginPtr[i] >= 97) && (nameBeginPtr[i] <= 122)) ) {
			break;
		}
	}
	return i;
}

//printing names
void ArrayOfMatrix::outputNames() {
	std::cout << "existing matrices names:\n";

	for (int i = 0; i < matricesInitialized; ++i) {
		if (matrixAtIndexExists[currentActiveArray][i] == 1) {
			std::cout << names[currentActiveArray][i] << "\n";
		}
	}

	std::cout << "\n";
}

//returns index in array of matrices
int ArrayOfMatrix::getMatrixIndex(const char const* nameBeginPtr) {
	int length = nameLength(nameBeginPtr);

	for (int i = 0; i < matricesInitialized; ++i) {
		if (matrixAtIndexExists[currentActiveArray][i] == 1) {

			int currentNameLength = nameLength(&names[currentActiveArray][i][0]);

			if ((nameBeginPtr[0] == names[currentActiveArray][i][0]) && (length == currentNameLength)) {

				int j;
				for (j = 1; j < length; ++j) {
					if (nameBeginPtr[j] != names[currentActiveArray][i][j]) {
						break;
					}
				}

				if (j == length) {
					return i;
				}
			}
		}
	}

	return -1;
}

//checks if name is not the same as possible command
bool ArrayOfMatrix::nameValidation(const char const* nameBeginPtr) {
	int length = nameLength(nameBeginPtr);

	for (int i = 0; i < commandsCount; ++i) {
		if (length == mainCommandsLengths[i]) {
			int coincidence;

			for (coincidence = 0; coincidence < length; ++coincidence) {
				if (nameBeginPtr[coincidence] != mainCommands[i][coincidence]) {
					break;
				}
			}

			if (coincidence == length) {
				return false;
			}
		}
	}

	return true;
}

//function returns command code if comand is appropriate. if not, returns 0
int ArrayOfMatrix::getCommand(const char const* command) {
	int currComLength = nameLength(command);

	//if user in main menu
	if (currentMode == MAIN_MENU) {
		for (int i = 0; i < commandsCount; ++i) {
			if (currComLength == mainCommandsLengths[i]) {
				int cnsd;

				for (cnsd = 0; cnsd < currComLength; ++cnsd) {
					if (command[cnsd] != mainCommands[i][cnsd]) {
						break;
					}
				}

				//returns command code if coincedence found
				if (cnsd == currComLength) {
					switch (i) {
						case(0): {
							return ADD_MATRIX;
						}
						case(1): {
							return DELETE_MATRIX;
						}
						case(2): {
							return GET_MATRIX;
						}
						case(3): {
							return FILL_MATRIX;
						}
						case(4): {
							return GET_LIST;
						}
						case(5): {
							return SAVE_TO_FILE;
						}
						case(6): {
							return STOP;
						}
						case(7): {
							return IMPORT_FROM_FILE;
						}
						case(8): {
							return SET_MATRIX_ELEMENT;
						}
					}
				}
			}
		}
	}
	
	//if first word of command is a name of existing matrix, returns special command code (matrixname = ... is typed in)
	if (getMatrixIndex(command) != -1) {
		return MATRIX_SETTING_MODE;
	}

	//if no coincedence with existing commands found, returns error code
	return MATRIX_ERROR;
}

//operation with matrix, depending on input (sum, multiply)
void ArrayOfMatrix::setMatrix(const char const* command) {
	char modifiedMatrixName[21], matrix1Name[21], matrix2Name[21];
	int modifiedMatrixNameLength = 0, matrix1NameLength = 0, matrix2NameLength = 0;

	int modifiedMatrixIndex, matrix1Index, matrix2Index;

	int approximateEqualsPos, approximateActionSignPos;

	//getting name of matrix for modification
	for (int i = 0; i < 20; ++i) {
		if ((command[i] > 'z') || (command[i] < 'a')) { break; }

		modifiedMatrixName[modifiedMatrixNameLength] = command[i];

		++modifiedMatrixNameLength;
	}
	modifiedMatrixName[modifiedMatrixNameLength] = 0;

	modifiedMatrixIndex = getMatrixIndex(modifiedMatrixName);

	//approximate position of '=' in command line
	approximateEqualsPos = modifiedMatrixNameLength + 1;

	//getting name of first matrix
	for (int i = approximateEqualsPos + 2; i < i + 20; ++i) {
		if ((command[i] > 'z') || (command[i] < 'a')) { break; }

		matrix1Name[matrix1NameLength] = command[i];

		++matrix1NameLength;
	}
	matrix1Name[matrix1NameLength] = 0;

	matrix1Index = getMatrixIndex(matrix1Name);

	//check if first matrix exist
	if (matrix1Index == -1) {
		std::cout << "matrix \"" << matrix1Name << "\" does not exist. operation canceled\n\n";

		return;
	}
	//check if first matrix initialized
	if (mtxArr[currentActiveArray][matrix1Index]->isInitialized() == false) {
		std::cout << "matrix \"" << matrix1Name << "\" is not initialized. operation canceled\n\n";

		return;
	}


	//approximate position of '+'/'*', etc. in command line
	approximateActionSignPos = approximateEqualsPos + 2 + matrix1NameLength + 1;

	//getting name of second matrix
	for (int i = approximateActionSignPos + 2; i < i + 20; ++i) {
		if ((command[i] > 'z') || (command[i] < 'a')) { break; }

		matrix2Name[matrix2NameLength] = command[i];

		++matrix2NameLength;
	}
	matrix2Name[matrix2NameLength] = 0;

	matrix2Index = getMatrixIndex(matrix2Name);

	//check if second matrix exist
	if (matrix2Index == -1) {
		std::cout << "matrix \"" << matrix2Name << "\" does not exist. operation canceled\n\n";

		return;
	}
	//check if second matrix initialized
	if (mtxArr[currentActiveArray][matrix2Index]->isInitialized() == false) {
		std::cout << "matrix \"" << matrix1Name << "\" is not initialized. operation canceled\n\n";

		return;
	}

	if (command[approximateActionSignPos] == '*') {
		if ((mtxArr[currentActiveArray][modifiedMatrixIndex]->getSize() != mtxArr[currentActiveArray][matrix1Index]->getSize()) ||
			(mtxArr[currentActiveArray][modifiedMatrixIndex]->getSize() != mtxArr[currentActiveArray][matrix2Index]->getSize())) {
			std::cout << "matrices have different sizes. operation canceled\n\n";

			return;
		}

		(*mtxArr[currentActiveArray][modifiedMatrixIndex]) = (*mtxArr[currentActiveArray][matrix1Index]) * (*mtxArr[currentActiveArray][matrix2Index]);

		return;

	}

	if (command[approximateActionSignPos] == '+') {
		(*mtxArr[currentActiveArray][modifiedMatrixIndex]) = (*mtxArr[currentActiveArray][matrix1Index]) + (*mtxArr[currentActiveArray][matrix2Index]);

		return;
	}

	

}

//adds matrix and a name to array	CONSOLE
void ArrayOfMatrix::addMatrix(const char const* nameBeginPtr) {
	if (getMatrixIndex(nameBeginPtr) != -1) {
		std::cout << "matrix with such name already exists\n\n";
		return;
	}

	if (nameValidation(nameBeginPtr) == false) {
		std::cout << "name cannot match with possible command. unable to create matrix.\n\n";
		return;
	}

	int length = nameLength(nameBeginPtr);

	if (length < 1) {
		std::cout << "inappropriate name. unable to create matrix.\n\n";
		return;
	}
	
	//finding index without initialized matrix
	int availibleIndex = -1;

	//getting matrix size (and returns if size set to less than 1)
	std::cout << "enter matrix size:\n";
	int matrixSize;
	std::cin >> matrixSize;
	std::cout << "\n";
	if (matrixSize < 1) {
		std::cout << "matrix size cannot be less than 1. matrix is not created\n\n";
		std::cin.ignore();
		return;
	}

	increaseArraySize();

	//check if there is empty array element. if there's none, new is initialized
	for (int i = 0; i < matricesInitialized; ++i) {
		if (matrixAtIndexExists[currentActiveArray][i] == 0) {
			availibleIndex = i;

			break;
		}
	}
	if (availibleIndex == -1) {
		availibleIndex = matricesInitialized;

		++matricesInitialized;
	}

	


	//adding name
	for (int i = 0; i < length; ++i) {
		names[currentActiveArray][availibleIndex][i] = nameBeginPtr[i];
	}
	names[currentActiveArray][availibleIndex][length] = 0;

	//adding matrix
	mtxArr[currentActiveArray][availibleIndex] = new Matrix(matrixSize);

	//setting flag to 1
	matrixAtIndexExists[currentActiveArray][availibleIndex] = 1;

	std::cin.ignore();
	

}

//adds matrix and a name to array	FILE
void ArrayOfMatrix::addMatrix(const char const* nameBeginPtr, int elementsCount, std::ifstream& file) {
	increaseArraySize();

	int length = nameLength(nameBeginPtr);

	//finding index without initialized matrix
	int availibleIndex = -1;

	//check if there is empty array element. if there's none, new is initialized
	for (int i = 0; i < matricesInitialized; ++i) {
		if (matrixAtIndexExists[currentActiveArray][i] == 0) {
			availibleIndex = i;

			break;
		}
	}
	if ((availibleIndex == -1) || (matricesInitialized == 0)) {
		availibleIndex = matricesInitialized;

		++matricesInitialized;
	}

	//adding name
	for (int i = 0; i < length; ++i) {
		names[currentActiveArray][availibleIndex][i] = nameBeginPtr[i];
	}
	names[currentActiveArray][availibleIndex][length] = 0; 

	//adding matrix
	mtxArr[currentActiveArray][availibleIndex] = new Matrix(elementsCount);

	int** tempMatrix = new int* [elementsCount];
	for (int i = 0; i < elementsCount; ++i) {
		tempMatrix[i] = new int[elementsCount];
	}

	//getting all elements
	for (int i = 0; i < elementsCount; ++i) {
		for (int j = 0; j < elementsCount; ++j) {
			int inputNumber;
			file >> inputNumber;

			tempMatrix[i][j] = inputNumber;
		}
	}

	mtxArr[currentActiveArray][availibleIndex] = new Matrix(elementsCount, tempMatrix);

	//setting flag to 1
	matrixAtIndexExists[currentActiveArray][availibleIndex] = 1;
}

//removes matrix and name from array
void ArrayOfMatrix::deleteMatrix(const char const* nameBeginPtr) {
	int matrixIndex = getMatrixIndex(nameBeginPtr);

	if (matrixIndex == -1) {
		std::cout << "no such matrix exists\n\n";
		return;
	}

	delete mtxArr[currentActiveArray][matrixIndex];

	delete[] names[currentActiveArray][matrixIndex];
	names[currentActiveArray][matrixIndex] = new char[21];

	matrixAtIndexExists[currentActiveArray][matrixIndex] = 0;

	std::cout << "matrix deleted\n\n";
}

//allows to fill matrix at any time
void ArrayOfMatrix::fillMatrix(const char const* nameBeginPtr) {
	int matrixIndex = getMatrixIndex(nameBeginPtr);

	if (matrixIndex == -1) {
		std::cout << "no such matrix exists\n\n";
		return;
	}

	std::cout << "enter matrix data:\n";
	mtxArr[currentActiveArray][matrixIndex]->consoleInput();
	std::cout << "\n";

	std::cin.ignore();
}

//parses number from input string
int parseNumberFromString(const char const* digitBeginPtr, int* numeralCountOutput) {
	int number = 0;
	int numeralCount = 0;

	while ((digitBeginPtr[numeralCount] != ' ') && (digitBeginPtr[numeralCount] != 0)) {
		++numeralCount;
	}

	for (int i = 0; i < numeralCount; ++i) {
		int currentDigit = digitBeginPtr[i] - '0';

		number += currentDigit * pow(10, numeralCount - i - 1);
	}

	if (numeralCountOutput != nullptr) {
		*numeralCountOutput = numeralCount;
	}

	return number;
}

//
void ArrayOfMatrix::setMatrixElement(const char const* nameBeginPtr) {
	if (nameBeginPtr[-1] == 0) {
		std::cout << "no name entered\n\n";
		return;
	}

	int matrixIndex = getMatrixIndex(nameBeginPtr);

	if (matrixIndex == -1) {
		std::cout << "no such matrix exists\n\n";
		return;
	}

	int digitIndex = 0;
	while ((nameBeginPtr[digitIndex] != ' ') && (nameBeginPtr[digitIndex] != 0)) {
		++digitIndex;
	}
	++digitIndex;

	if (nameBeginPtr[digitIndex - 1] == 0) {
		std::cout << "no index entered\n\n";
		return;
	}

	int xLength;
	int x = parseNumberFromString(&nameBeginPtr[digitIndex], &xLength);

	digitIndex += xLength + 1;

	int y = parseNumberFromString(&nameBeginPtr[digitIndex], nullptr);

	std::cout << x << "\t" << y << "\n";


}

//imports matrices into array from file
void ArrayOfMatrix::importFromFile() {
	char path[100];
	std::cout << "enter import file path:\n";
	std::cin.getline(path, 100);
	std::cout << "\n";

	std::ifstream file(path, std::ios::in);

	if (file.is_open() == false) {
		std::cout << "file not found. import canceled\n\n";
		return;
	}
	
	//finding last digit - amount of saved matrices
	int pos = -1;
	file.seekg(pos, file.end);
	while (file.peek() != '\n') {
		--pos;
		file.seekg(pos, file.end);
	}
	file.ignore();

	int mtxCountInFile;
	file >> mtxCountInFile;

	file.seekg(0, file.beg);

	bool needToSkipLine = false;

	for (int i = 0; i < mtxCountInFile; ++i) {
		char name[21] = { 0 };

		for (int j = 0; j < 20; ++j) {
			char letter = file.get();

			if ((letter <= 'z') && (letter >= 'a')) {
				name[j] = letter;
			}
			else {
				break;
			}
		}

		int elementCount;
		file >> elementCount;

		//check if matrix with that name already exists and skip it's data if needed
		if (getMatrixIndex(name) != -1) {
			std::cout << "unable to save matrix \"" << name << "\" - this name is already occupied\n";
			needToSkipLine = true;

			int tempToSkipMatrixData;
			for (int j = 0; j < elementCount * elementCount; ++j) {
				file >> tempToSkipMatrixData;
			}

			do {
				file.ignore();
			} while (file.peek() != '\n');
			file.ignore();
			file.ignore();

			continue;
		}

		addMatrix(name, elementCount, file);

		file.ignore();
		file.ignore();
		file.ignore();
	}

	if (needToSkipLine == true) {
		std::cout << "\n";
	}

	file.close();
}

//saves data into file
void ArrayOfMatrix::saveInFile() {
	char path[100];
	std::cout << "enter path to file for data or leave blank if you wish to create a new file:\n";
	std::cin.getline(path, 100);

	std::ifstream test(path, std::ios::in);

	//test if file exists
	if (test.is_open() == true) {
		test.close();

		std::ofstream file(path, std::ios::trunc);

		int savedMtxCount = 0;

		for (int i = 0; i < matricesInitialized; ++i) {
			if (matrixAtIndexExists[currentActiveArray][i] == 1) {
				if (mtxArr[currentActiveArray][i]->isInitialized() == false) {
					std::cout << "unable to save matrix \"" << names[currentActiveArray][i] << "\" - matrix is not initialized\n";

					continue;
				}

				//writing name and size
				file << names[currentActiveArray][i] << " " << mtxArr[currentActiveArray][i]->getSize() << "\n";

				//writing data
				mtxArr[currentActiveArray][i]->fileOutput(file);

				++savedMtxCount;
			}

			file << "\n";
		}

		file << savedMtxCount;

		file.close();

		std::cout << "data successfuly saved\n\n";
	}
	else {
		std::cout << "file in specified directory not found or path is blank. would you like to create new file \"save.txt\"? (y/n):\n";

		char answer;
		std::cin >> answer;
		std::cin.ignore();

		if (answer == 'n') {
			std::cout << "save canceled\n\n";
			return;
		}
		else if(answer != 'y') {
			std::cout << "no such option. save canceled\n\n";
			return;
		}


		//writing no a new file
		std::ofstream file("save.txt", std::ios::trunc);

		int savedMtxCount = 0;

		for (int i = 0; i < matricesInitialized; ++i) {
			if (matrixAtIndexExists[currentActiveArray][i] == 1) {
				//writing name and size
				file << names[currentActiveArray][i] << " " << mtxArr[currentActiveArray][i]->getSize() << "\n";

				//writing data
				mtxArr[currentActiveArray][i]->fileOutput(file);

				++savedMtxCount;
			}

			file << "\n";
		}

		file << savedMtxCount;

		file.close();

		std::cout << "data successfuly saved\n\n";
	}
}

//gets command from console and doing something according to it
void ArrayOfMatrix::action() {
	while (true) {

		char inputCommand[80];

		std::cout << "enter a comand:\n";
		std::cin.getline(inputCommand, 80, '\n');
		std::cout << "\n";

		int command = getCommand(inputCommand);

		switch (command) {
			case (MATRIX_ERROR): {	//inappropriate command
				std::cout << "no such command exists\n\n";

				break;
			}
			case (ADD_MATRIX): {	//"new /name/"
				addMatrix(&inputCommand[4]);

				break;
			}
			case (DELETE_MATRIX): {	//"delete /name/"
				deleteMatrix(&inputCommand[7]);

				break;
			}
			case (GET_MATRIX): {	//"get /name/"
				int foundMatrixIndex = getMatrixIndex(&inputCommand[4]);

				if (foundMatrixIndex != -1) {
					mtxArr[currentActiveArray][foundMatrixIndex]->consoleOutput();
				}
				else {
					std::cout << "no matrix found\n\n";
				}

				break;
			}
			case (FILL_MATRIX): {	//"fill /name/"
				fillMatrix(&inputCommand[5]);

				break;
			}
			case (GET_LIST): {	//"list"
				outputNames();

				break;
			}
			case (SAVE_TO_FILE): {	//"save"
				saveInFile();

				break;
			}
			case (IMPORT_FROM_FILE): {	//"import"
				importFromFile();

				break;
			}
			case (MATRIX_SETTING_MODE): {	//"matrix = mat1 * mat2"
				setMatrix(inputCommand);

				break;
			}
			case(SET_MATRIX_ELEMENT): {		//"set \matrixname\ 1 1
				setMatrixElement(&inputCommand[4]);

				break;
			}
		}

		if (command == STOP) {	//"stop"
			break;
		}
	}
}