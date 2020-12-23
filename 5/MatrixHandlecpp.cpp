#include "MatrixHandle.h"
#include <iostream>
//#include <fstream>

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

		std::cout << "array increased\n";
	}
}

//function returns command code if comand is appropriate. if not, returns 0
int getCommand(const char const* command) {
	if ((command[0] == 'n') && (command[1] == 'e') && (command[2] == 'w')) {
		return 1;		//create new matrix
	}
	else if ((command[0] == 'd') && (command[1] == 'e') && (command[2] == 'l') && (command[3] == 'e') && (command[4] == 't') && (command[5] == 'e')) {
		return 2;		//delete existing matrix
	}
	else if ((command[0] == 'g') && (command[1] == 'e') && (command[2] == 't')) {
		return 3;
	}
	else if ((command[0] == 'f') && (command[1] == 'i') && (command[2] == 'l') && (command[3] == 'l')) {
		return 4;
	}
	else if ((command[0] == 'l') && (command[1] == 'i') && (command[2] == 's') && (command[3] == 't')) {
		return 5;
	}
	else if ((command[0] == 's') && (command[1] == 'a') && (command[2] == 'v') && (command[3] == 'e')) {
		return 6;
	}
	else if ((command[0] == 'i') && (command[1] == 'm') && (command[2] == 'p') && (command[3] == 'o') && (command[4] == 'r') && (command[5] == 't')) {
		return 7;
	}
	else if ((command[0] == 's') && (command[1] == 't') && (command[2] == 'o') && (command[3] == 'p')) {
		return 99;
	}
	else {
		return 0;		//error: no such comand exists
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
bool nameValidation(const char const* nameBeginPtr) {
	const int amount = 7;

	char invalidNames[amount][7] = {
		{"new"},
		{"delete"},
		{"get"},
		{"fill"},
		{"list"},
		{"save"},
		{"stop"}
	};

	unsigned char invNamesLengths[amount] = {
		3,
		6,
		3,
		4,
		4,
		4,
		4
	};

	int length = nameLength(nameBeginPtr);

	for (int i = 0; i < amount; ++i) {
		if (length == invNamesLengths[i]) {
			int coincidence;

			for (coincidence = 0; coincidence < length; ++coincidence) {
				if (nameBeginPtr[coincidence] != invalidNames[i][coincidence]) {
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

//adds matrix and a name to array
void ArrayOfMatrix::addMatrix(const char const* nameBeginPtr) {
	if (getMatrixIndex(nameBeginPtr) != -1) {
		std::cout << "matrix with such name already exists\n\n";
		return;
	}

	if (nameValidation(nameBeginPtr) == false) {
		std::cout << "name cannot match with possible command. unable to create matrix.\n\n";
		return;
	}

	increaseArraySize();

	int length = nameLength(nameBeginPtr);

	if (length < 1) {
		std::cout << "inappropriate name. unable to create matrix.\n\n";
		return;
	}
	
	//finding index without initialized matrix
	int availibleIndex;
	for (availibleIndex = 0; availibleIndex < matricesCount; ++availibleIndex) {
		if (matrixAtIndexExists[currentActiveArray][availibleIndex] == 0) {
			break;
		}
	}

	//getting matrix size
	std::cout << "enter matrix size:\n";
	int matrixSize;
	std::cin >> matrixSize;
	std::cout << "\n";

	if (matrixSize < 1) {
		std::cout << "matrix size cannot be less than 1. matrix is not created\n\n";
		std::cin.ignore();
		return;
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
	++matricesInitialized;
	

}

//
void ArrayOfMatrix::addMatrix(const char const* nameBeginPtr, std::ifstream file) {
	if (getMatrixIndex(nameBeginPtr) != -1) {
		std::cout << "matrix with such name already exists\n\n";
		return;
	}

	if (nameValidation(nameBeginPtr) == false) {
		std::cout << "name cannot match with possible command. unable to create matrix.\n\n";
		return;
	}

	increaseArraySize();

	int length = nameLength(nameBeginPtr);

	



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

	std::cout << "type in data source (console/file):\n";

	char input[100];
	std::cin.getline(input, 8);
	std::cout << "\n";

	if ((input[0] == 'c') && (input[1] == 'o') && (input[2] == 'n') && (input[3] == 's') && (input[4] == 'o') && (input[5] == 'l') && (input[6] == 'e')) {
		std::cout << "enter matrix data:\n";
		mtxArr[currentActiveArray][matrixIndex]->consoleInput();
		std::cout << "\n";

		std::cin.ignore();
	}
	else if ((input[0] == 'f') && (input[1] == 'i') && (input[2] == 'l') && (input[3] == 'e')) {
		std::cout << "enter file name (file should be located inside program folder):\n";
		std::cin.getline(input, 100);
		std::cout << "\n";;

		mtxArr[currentActiveArray][matrixIndex]->fileInput(input);
	}
}

//
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
	
	//finding last digit - amoun of saved matrices
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

		if (getMatrixIndex(name) != -1) {
			std::cout << "unable to save matrix \"" << name << "\" - this name is already occupied\n";

			do {
				file.ignore();

				//std::cout << (char)file.peek() << "-\n";
			} while (file.peek() != '\n');

			continue;
		}


		//std::cout << " - " << elementCount << " - ll\n\n";

		increaseArraySize();

		//mtxArr[currentActiveArray][matricesInitialized] = new Matrix(elementCount);

		for (int j = 0; j < matricesCount; ++j) {
			if (matrixAtIndexExists[currentActiveArray][j] == 0) {
				mtxArr[currentActiveArray][j] = new Matrix(elementCount);

				for (int row = 0; row < elementCount; ++row) {
					for (int col = 0; col < elementCount; ++col) {
						int input;
						file >> input;

						mtxArr[currentActiveArray][j]->setElement(row, col, input);
					}
				}

				for (int letter = 0; letter < 21; ++letter) {
					names[currentActiveArray][j][letter] = name[letter];
				}

				matrixAtIndexExists[currentActiveArray][j] = 1;

				++matricesInitialized;
			}



		}



		/*for (int row = 0; row < elementCount; ++row) {
			for (int col = 0; col < elementCount; ++col) {
				mtxArr[currentActiveArray][matricesInitialized]
			}
		}*/


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

		char* inputCommand = new char[80];

		std::cout << "enter a comand:\n";
		std::cin.getline(inputCommand, 80, '\n');
		std::cout << "\n";

		int command = getCommand(inputCommand);

		switch (command) {
			case (0): {	//inappropriate command
				std::cout << "no such command exists\n\n";

				break;
			}
			case (1): {	//"new /name/"
				addMatrix(&inputCommand[4]);

				break;
			}
			case (2): {	//"delete /name/"
				deleteMatrix(&inputCommand[7]);

				break;
			}
			case (3): {	//"get /name/"
				int foundMatrixIndex = getMatrixIndex(&inputCommand[4]);

				if (foundMatrixIndex != -1) {
					mtxArr[currentActiveArray][foundMatrixIndex]->consoleOutput();
				}
				else {
					std::cout << "no matrix found\n\n";
				}

				break;
			}
			case (4): {	//"fill /name/"
				fillMatrix(&inputCommand[5]);

				break;
			}
			case(5): {	//"list"
				outputNames();

				break;
			}
			case(6): {	//"save"
				saveInFile();

				break;
			}
			case(7): {	//"import"
				importFromFile();

				break;
			}
		}

		if (command == 99) {	//"stop"
			delete[] inputCommand;

			break;
		}


		delete[] inputCommand;
	}
}