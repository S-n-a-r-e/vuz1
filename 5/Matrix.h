#include <fstream>

class Matrix {
	int** mat;
	int size;

	bool initialized = false;

public:
	Matrix(int s);

	Matrix(int s, int** inpMat);

	void consoleInput();

	void fileInput(const char* path);

	Matrix(Matrix const& otherMat);

	~Matrix();

	void operator=(Matrix const& otherMat);

	void operator*=(Matrix const& otherMat);

	Matrix operator*(Matrix const& otherMat);

	void operator^=(int power);

	Matrix operator^(int power);

	void operator+=(Matrix const& otherMat);

	Matrix operator+(Matrix const& otherMat);

	Matrix operator&(Matrix const& otherMat);

	int& operator()(int i, int j);

	int getSize();

	bool isInitialized();

	void setElement(int i, int j, int number);

	void setNegativeToZero();

	void consoleOutput();

	void fileOutput();

	void fileOutput(const char* path);

	void fileOutput(std::ofstream& file);

};