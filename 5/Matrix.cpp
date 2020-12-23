#include "Matrix.h"

#include <iostream>
#include <fstream>

Matrix::Matrix(int s) : size(s) {
	mat = new int* [size];
	for (int i = 0; i < size; ++i) {
		mat[i] = new int[size];
	}
}

void Matrix::consoleInput() {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			std::cin >> mat[i][j];
		}
	}

	initialized = true;
}

void Matrix::fileInput(const char* path) {
	std::ifstream file(path, std::ios::in);
	if (file.is_open() == false) {
		std::cout << "file not found\n";
		return;
	}




	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			file >> mat[i][j];
		}
	}
	file.close();

	initialized = true;
}

Matrix::Matrix(int s, int** inpMat) : size(s) {
	mat = new int* [size];
	for (int i = 0; i < size; ++i) {
		mat[i] = new int[size];

		for (int j = 0; j < size; ++j) {
			mat[i][j] = inpMat[i][j];
		}

		delete[] inpMat[i];
	}
	delete[] inpMat;	

	initialized = true;
}

Matrix::Matrix(Matrix const& otherMat) : size(otherMat.size) {
	mat = new int* [size];
	for (int i = 0; i < size; ++i) {
		mat[i] = new int[size];

		for (int j = 0; j < size; ++j) {
			mat[i][j] = otherMat.mat[i][j];
		}
	}

	initialized = true;
}

Matrix::~Matrix() {
	for (int i = 0; i < size; ++i) {
		delete[] mat[i];
	}
	delete[] mat;
}

void Matrix::operator=(Matrix const& otherMat) {
	if (initialized == false) {
		size = otherMat.size;

		mat = new int* [size];
		for (int i = 0; i < size; ++i) {
			mat[i] = new int[size];

			for (int j = 0; j < size; ++j) {
				mat[i][j] = otherMat.mat[i][j];
			}
		}

		initialized = true;
	}
	else {
		for (int i = 0; i < size; ++i) {
			delete[] mat[i];
		}
		delete[] mat;

		size = otherMat.size;

		mat = new int* [size];
		for (int i = 0; i < size; ++i) {
			mat[i] = new int[size];

			for (int j = 0; j < size; ++j) {
				mat[i][j] = otherMat.mat[i][j];
			}
		}
	}
}

void Matrix::operator*=(Matrix const& otherMat) {
	if (initialized == false) {
		return;
	}

	//temporary array + memory
	int** temp = new int* [size];
	for (int i = 0; i < size; ++i) {
		temp[i] = new int[size];
	}

	//counting
	for (int row = 0; row < size; ++row) {
		for (int col = 0; col < size; ++col) {
			temp[row][col] = 0;

			for (int i = 0; i < size; ++i) {
				temp[row][col] += mat[row][i] * otherMat.mat[i][col];
			}
		}
	}

	//rewriting data
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			mat[i][j] = temp[i][j];
		}
	}

	//deleting temporary array
	for (int i = 0; i < size; ++i) {
		delete[] temp[i];
	}
	delete[] temp;
}

Matrix Matrix::operator*(Matrix const& otherMat) {
	if (initialized == false) {
		int** temp = new int* [size];
		for (int i = 0; i < size; ++i) {
			temp[i] = new int[size];

			for (int j = 0; j < size; ++j) {
				temp[i][j] = otherMat.mat[i][j];
			}
		}

		return Matrix(size, temp);
	}
	else {
		//temporary array + memory
		int** temp = new int* [size];
		for (int i = 0; i < size; ++i) {
			temp[i] = new int[size];
		}

		//counting
		for (int row = 0; row < size; ++row) {
			for (int col = 0; col < size; ++col) {
				temp[row][col] = 0;

				for (int i = 0; i < size; ++i) {
					temp[row][col] += mat[row][i] * otherMat.mat[i][col];
				}
			}
		}

		return Matrix(size, temp);
	}
}

void Matrix::operator^=(int power) {
	if (initialized == false) {
		return;
	}

	//saving initial matrix, previous power, current power, +memory
	int** initialMat = new int* [size];
	int** prevTemp = new int* [size];
	int** temp = new int* [size];

	for (int i = 0; i < size; ++i) {
		initialMat[i] = new int[size];
		prevTemp[i] = new int[size];
		temp[i] = new int[size];

		for (int j = 0; j < size; ++j) {
			initialMat[i][j] = mat[i][j];
			prevTemp[i][j] = mat[i][j];
			temp[i][j] = 0;
		}
	}

	//counting
	for (int pow = 1; pow < power; ++pow) {
		for (int row = 0; row < size; ++row) {
			for (int col = 0; col < size; ++col) {
				for (int i = 0; i < size; ++i) {
					temp[row][col] += prevTemp[row][i] * initialMat[i][col];
				}
			}
		}

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				prevTemp[i][j] = temp[i][j];
				temp[i][j] = 0;
			}
		}

	}

	//rewriting data
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			mat[i][j] = prevTemp[i][j];
		}
	}

	//releasing data
	for (int i = 0; i < size; ++i) {
		delete[] initialMat[i];
		delete[] prevTemp[i];
		delete[] temp[i];
	}
	delete[] initialMat;
	delete[] prevTemp;
	delete[] temp;
}

Matrix Matrix::operator^(int power) {
	if (initialized == false) {
		return Matrix(size);
	}

	//saving initial matrix, previous power, current power, +memory
	int** initialMat = new int* [size];
	int** prevTemp = new int* [size];
	int** temp = new int* [size];

	for (int i = 0; i < size; ++i) {
		initialMat[i] = new int[size];
		prevTemp[i] = new int[size];
		temp[i] = new int[size];

		for (int j = 0; j < size; ++j) {
			initialMat[i][j] = mat[i][j];
			prevTemp[i][j] = mat[i][j];
			temp[i][j] = 0;
		}
	}

	//counting
	for (int pow = 1; pow < power; ++pow) {
		for (int row = 0; row < size; ++row) {
			for (int col = 0; col < size; ++col) {
				for (int i = 0; i < size; ++i) {
					temp[row][col] += prevTemp[row][i] * initialMat[i][col];
				}
			}
		}

		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				prevTemp[i][j] = temp[i][j];
				temp[i][j] = 0;
			}
		}

	}

	//releasing data
	for (int i = 0; i < size; ++i) {
		delete[] initialMat[i];
		delete[] temp[i];
	}
	delete[] initialMat;
	delete[] temp;

	return Matrix(size, prevTemp);
}

void Matrix::operator+=(Matrix const& otherMat) {
	if (initialized == false) {
		return;
	}

	if (size <= otherMat.size) {
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				mat[i][j] += otherMat.mat[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < otherMat.size; ++i) {
			for (int j = 0; j < otherMat.size; ++j) {
				mat[i][j] += otherMat.mat[i][j];
			}
		}
	}
}

Matrix Matrix::operator+(Matrix const& otherMat) {
	if (initialized == false) {
		return Matrix(size);
	}

	int** temp = new int* [size];
	for (int i = 0; i < size; ++i) {
		temp[i] = new int[size];
	}

	if (size <= otherMat.size) {
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				temp[i][j] = mat[i][j] + otherMat.mat[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < otherMat.size; ++i) {
			for (int j = 0; j < otherMat.size; ++j) {
				temp[i][j] = mat[i][j] + otherMat.mat[i][j];
			}
		}
	}

	return Matrix(size, temp);
}

int& Matrix::operator()(int i, int j) {
	return mat[i][j];
}

int Matrix::getSize() {
	return size;
}

bool Matrix::isInitialized() {
	return initialized;
}

void Matrix::setElement(int i, int j, int number) {
	mat[i][j] = number;
}

void Matrix::setNegativeToZero() {
	if (initialized == false) {
		return;
	}

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (mat[i][j] < 0) {
				mat[i][j] = 0;
			}
		}
	}
}

void Matrix::consoleOutput() {
	if (initialized == false) {
		std::cout << "matrix is not initialized\n\n";
		return;
	}

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; j++) {
			std::cout << mat[i][j] << "\t";
		}
		std::cout << "\n\n";
	}
}

void Matrix::fileOutput() {
	if (initialized == false) {
		return;
	}

	std::ofstream file("result.txt", std::ios::trunc);

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			file << mat[i][j] << "\t";
		}
		file << "\n";
	}

	file.close();
}

void Matrix::fileOutput(const char* path) {
	if (initialized == false) {
		return;
	}

	std::ifstream fileTest(path, std::ios::in);

	if (fileTest.is_open() == false) {
		std::cout << "file not found\n";
		return;
	}
	fileTest.close();

	std::ofstream file(path, std::ios::app);

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			file << mat[i][j] << "\t";
		}
		file << "\n";
	}

	file.close();
}

void Matrix::fileOutput(std::ofstream& file) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			file << mat[i][j] << "\t";
		}
		file << "\n";
	}
}