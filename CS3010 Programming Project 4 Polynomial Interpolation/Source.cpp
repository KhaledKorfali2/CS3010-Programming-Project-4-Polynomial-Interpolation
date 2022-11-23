#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<chrono>
#include<vector>
#include <iomanip>
using namespace std;


//constants
const int KMAX = 50;

//Function Prototypes
void display1_DArray(vector<float>& matrix, int numofEquations, string matrixName);
void display1_DArray(vector<int>& matrix, int numofEquations, string matrixName);
void displayMatrix(vector<vector<float>>& matrix, int numOfEquations);
void displayMatrix2(vector<vector<float>>& matrix, int numOfEquations);
void combineMatricies(vector<vector<float>>& a, vector<float>& b, vector<vector<float>>& userEquations, int numOfEquations);
void jacobiMethod(vector<vector<float>> a, vector<float> b, vector<float> xi, int numOfEquations, float delta, float error);
float calcL2Norm(vector<float>& xi, int numOfEquations);
float calcL2Norm(vector<float>& xi, vector<float>& yi, int numOfEquations);
void gaussSeidelMethod(vector<vector<float>> a, vector<float> b, vector<float> xi, int numOfEquations, float delta, float error);

int main()
{

	//Initialize cout to print only 2 places after decimal points
	cout << fixed;
	cout << setprecision(6);


	//Variable Declarations
	int numOfEquations = 0;
	int inputMethod = -1;
	int x;
	int i, j, k;
	int temp;
	float delta = 0.0000000001;
	float error = 0.000005;
	float tempf;

	float diag;
	float sum = 0;
	string userFile = "";
	vector<vector<float>> userEquations, a;
	vector<float> multipliers;
	vector<float> row;
	vector<float> l;
	vector<float> b;
	vector<float> xi;
	//vector<float> yi;


	//Get User Input Method
	cout << "How many equations will you be entering?: " << endl;
	cin >> numOfEquations;
	while (numOfEquations != 1 && numOfEquations != 2 && numOfEquations != 3 && numOfEquations != 4 && numOfEquations != 5 && numOfEquations != 6 && numOfEquations != 7 && numOfEquations != 8 && numOfEquations != 9 && numOfEquations != 10)
	{
		cin.clear();                        // clear error flags
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // empty buffer 
		cout << "Error: Please enter an integer between 2 and 10 (inclusive)." << endl;
		cin >> numOfEquations;
	}

	cout << "What is your stopping error: " << endl;
	cin >> error;


	//inizalize vectors with all 0's
	userEquations.assign(numOfEquations, vector<float>(numOfEquations + 1, 0));
	a.assign(numOfEquations, vector<float>(numOfEquations, 0));
	b.assign(numOfEquations, 0);
	l.assign(numOfEquations, 0);
	xi.assign(numOfEquations, 0);
	//yi.assign(numOfEquations, 0);


	//Get user inputs
	cout << "How do you want to input your equations?: " << endl;
	cout << "1. Manually enter each row including the b values." << endl;
	cout << "2. Enter a file name which has the augmented coeffiecent matrix includining" << endl;
	cout << "   the b values in a simple text file format" << endl;
	cin >> inputMethod;

	//Checks that User is Inputing Either a "1" or a "2"
	while (inputMethod != 1 && inputMethod != 2)
	{
		cin.clear();                        // clear error flags
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // empty buffer 
		cout << "Error: Please choose between options 1 and 2." << endl;
		cin >> inputMethod;
	}
	//User has chosen to manually input values for augmented matrix
	if (inputMethod == 1)
	{
		std::cout << "Enter coefficent values for your augmented coeffiecent matrix: \n";
		//Gets user input line by line and stores in vecor userEquations
		for (int i = 1; i <= numOfEquations; i++)
		{
			std::cout << i << ": ";

			int curCol = 0;
			while (curCol < numOfEquations + 1)
			{
				cin >> x;
				row.push_back(x);
				curCol += 1;
			}
			//cout << "reached end of while";

			userEquations[i - 1] = row;
			row.clear();
		}
	}
	//User has chosen to input the name of a txt file containg values for augmented matrix
	else if (inputMethod == 2)
	{
		cout << "Please enter the name of your properly formated txt file: " << endl;
		cin >> userFile;


		//Reads in the data line by line storing values in userEquations
		fstream fileData;
		string line;
		string num;
		int curRow = 0;
		fileData.open(userFile, ios::in);
		if (fileData.is_open())
		{
			while (getline(fileData, line))
			{
				//cout << line << endl;
				stringstream s(line);

				while (getline(s, num, ' '))
				{
					row.push_back(stoi(num));
				}

				userEquations[curRow] = row;
				curRow += 1;
				row.clear();
			}
		}

		fileData.close();
	}
	else
	{
		cout << "Error: Please choose between options 1 and 2." << endl;
	}

	//Get starting values from user
	cout << "Pleaes enter starting values: " << endl;
	for (int r = 1; r <= numOfEquations; r++)
	{
		std::cout << "x" << r << ": ";

		cin >> tempf;
		xi[r - 1] = tempf;
		//cout << "reached end of while";
		//cout << xi[r-1] << " ";
	}

	//Split userEquations into coefficient matrix "a" and vetor matrix "b"
	for (int row = 0; row < numOfEquations; row++)
	{
		for (int col = 0; col <= numOfEquations; col++)
		{
			if (col == numOfEquations)
			{
				b[row] = userEquations[row][col];
			}
			else
			{
				a[row][col] = userEquations[row][col];
			}
		}
	}
	cout << "\n\n";
	cout << "!!!Note: All indicies will start at 0 NOT 1!!!" << endl;
	cout << "\n\n";
	displayMatrix(userEquations, numOfEquations);
	cout << "\n\n";


	//Display Output
	jacobiMethod(a, b, xi, numOfEquations, delta, error);
	cout << "\n\n";
	gaussSeidelMethod(a, b, xi, numOfEquations, delta, error);



	////Outputs values for x
	//display1_DArray(xi, numOfEquations, "xi");
	//for (int row = 0; row < numOfEquations; row++)
	//{
	//	cout << "x" << row << " = " << xi[row] << endl;
	//}
}

//Combines adjecency matrix "a" with vector matrix "b"
void combineMatricies(vector<vector<float>>& a, vector<float>& b, vector<vector<float>>& userEquations, int numOfEquations)
{
	for (int row = 0; row < numOfEquations; row++)
	{
		for (int col = 0; col <= numOfEquations; col++)
		{
			if (col == numOfEquations)
			{
				userEquations[row][col] = b[row];
			}
			else
			{
				userEquations[row][col] = a[row][col];
			}
		}
	}
}


//Function that displays an integer-value matrix
void display1_DArray(vector<int>& matrix, int numofEquations, string matrixName)
{
	for (int i = 0; i < numofEquations; i++)
	{
		if (i == 0)
		{
			cout << matrixName << " = [" << matrix[i];
		}
		else
		{
			cout << ", " << matrix[i];
		}
	}
	cout << "]" << endl;
}

//Function that displays a float-value matrix of size n
void display1_DArray(vector<float>& matrix, int numofEquations, string matrixName)
{
	for (int i = 0; i < numofEquations; i++)
	{
		if (i == 0)
		{
			cout << matrixName << " = [" << matrix[i];
		}
		else
		{
			cout << ", " << matrix[i];
		}
	}
	cout << "]" << endl;
}

//Function that displays a float-value matrix of size n x n+1
void displayMatrix(vector<vector<float>>& matrix, int numOfEquations)
{
	for (int i = 0; i < numOfEquations; i++)
	{
		for (int j = 0; j < numOfEquations + 1; j++)
		{
			cout << matrix[i][j] << "\t";
		}
		cout << "\n";
	}
}

//Function that displays a float-value matrix of size n x n
void displayMatrix2(vector<vector<float>>& matrix, int numOfEquations)
{
	for (int i = 0; i < numOfEquations; i++)
	{
		for (int j = 0; j < numOfEquations; j++)
		{
			cout << matrix[i][j] << "\t";
		}
		cout << "\n";
	}
}

void jacobiMethod(vector<vector<float>> a, vector<float> b, vector<float> xi, int numOfEquations, float delta, float error)
{
	vector<float> yi;
	yi.assign(numOfEquations, 0);
	float sum, diag;

	cout << "--------------------Jacobi--------------------" << endl;
	cout << "Jacobi: k = " << 0 << " ";
	display1_DArray(xi, numOfEquations, "xi");
	cout << "L2-norm: " << calcL2Norm(xi, yi, numOfEquations) << endl;
	cout << "\n";
	for (int k = 0;k < KMAX;k++)
	{
		yi = xi;
		for (int i = 0;i < numOfEquations; i++)
		{
			sum = b[i];
			diag = a[i][i];
			if (abs(diag) < delta)
			{
				cout << "Diagonal Element Is Too Small" << endl;
				return;
			}
			for (int j = 0;j < numOfEquations;j++)
			{
				if (j != i)
				{
					sum -= a[i][j] * yi[j];
				}
			}
			xi[i] = sum / diag;
		}
		cout << "Jacobi: k = " << k + 1 << " ";
		display1_DArray(xi, numOfEquations, "xi");
		cout << "L2-norm: " << calcL2Norm(xi, yi, numOfEquations) << endl;
		cout << "\n";



		if (calcL2Norm(xi, yi, numOfEquations) < error)
		{
			cout << "Jacobi Answer: " << "k = " << k + 1 << " ";
			display1_DArray(xi, numOfEquations, "xi");
			return;
		}

		//for (int v = 0; v < numOfEquations;v++)
		//{
		//	/*if (abs(xi[v] - yi[v]) < error)
		//	{
		//		cout << "Jacobi Final Answer: " << "k = " << k + 1 << " ";
		//		display1_DArray(xi, numOfEquations, "xi");
		//		return;
		//	}*/
		//	if (curL2Norm - prevL2Norm < error)
		//	{
		//		cout << "Jacobi Final Answer: " << "k = " << k + 1 << " ";
		//		display1_DArray(xi, numOfEquations, "xi");
		//		return;
		//	}
		//}
	}
	cout << "Maximum iterations Reached" << endl;
}

void gaussSeidelMethod(vector<vector<float>> a, vector<float> b, vector<float> xi, int numOfEquations, float delta, float error)
{
	vector<float> yi;
	yi.assign(numOfEquations, 0);
	float sum, diag;

	cout << "--------------------Guass-Seidel--------------------" << endl;
	cout << "Guass-Seidel: k = " << 0 << " ";
	display1_DArray(xi, numOfEquations, "xi");
	cout << "L2-norm: " << calcL2Norm(xi, yi, numOfEquations) << endl;
	cout << "\n";
	for (int k = 0;k < KMAX;k++)
	{
		yi = xi;
		for (int i = 0;i < numOfEquations; i++)
		{
			sum = b[i];
			diag = a[i][i];
			if (abs(diag) < delta)
			{
				cout << "Diagonal Element Is Too Small" << endl;
				return;
			}
			for (int j = 0;j < i;j++)
			{
				sum -= a[i][j] * xi[j];
			}
			for (int j = i + 1;j < numOfEquations;j++)
			{
				sum -= a[i][j] * xi[j];
			}
			xi[i] = sum / diag;
		}
		cout << "Guass-Seidel: k = " << k + 1 << " ";
		display1_DArray(xi, numOfEquations, "xi");
		cout << "L2-norm: " << calcL2Norm(xi, yi, numOfEquations) << endl;
		cout << "\n";

		if (calcL2Norm(xi, yi, numOfEquations) < error)
		{
			cout << "Guass-Seidel Final Answer: " << "k = " << k + 1 << " ";
			display1_DArray(xi, numOfEquations, "xi");
			return;
		}

		/*for (int v = 0; v < numOfEquations;v++)
		{
			if (curL2Norm - prevL2Norm < error)
			{
				cout << "Guass-Seidel Final Answer: " << "k = " << k + 1 << " ";
				display1_DArray(xi, numOfEquations, "xi");
				return;
			}
		}*/
	}
	cout << "Maximum iterations Reached" << endl;
}

float calcL2Norm(vector<float>& xi, int numOfEquations)
{
	float L2Norm = 0;
	float sum = 0;
	for (int v = 0; v < numOfEquations;v++)
	{
		sum += xi[v] * xi[v];
	}
	L2Norm = sqrt(sum);

	return L2Norm;
}

float calcL2Norm(vector<float>& xi, vector<float>& yi, int numOfEquations)
{
	float L2Norm = 0;
	float sum = 0;
	for (int v = 0; v < numOfEquations;v++)
	{
		sum += pow(abs(xi[v] - yi[v]), 2);
	}
	L2Norm = sqrt(sum);

	return L2Norm;
}