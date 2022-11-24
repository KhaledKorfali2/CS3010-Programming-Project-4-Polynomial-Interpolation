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
string TrimString(string str);
void display1_DArray(vector<float>& matrix, int numofEquations, string matrixName);
void display1_DArray(vector<int>& matrix, int numofEquations, string matrixName);
void displayMatrix(vector<vector<float>>& matrix, int numOfEquations);
void displayMatrix2(vector<vector<float>>& matrix, int numOfEquations);
void combineMatricies(vector<vector<float>>& a, vector<float>& b, vector<vector<float>>& userEquations, int numOfEquations);
void dispalyNewton(const vector<double>& c, const vector<double>& x);
vector<double> generateNewton(const vector<double>& x, vector<double> f);          // Gets Newton polynomial
void displayLagrange(const vector<double>& x, vector<double> f);


int main()
{

	//Initialize cout to print only 2 places after decimal points
	cout << fixed;
	cout << setprecision(2);


	//Variable Declarations
	float diag;
	float sum = 0;
	string userFile = "";
	
	//Get User Input
	cout << "Please enter the name of your properly formated txt file: " << endl;
	cin >> userFile;


	//Reads in the data line by line storing values in userEquations
	vector<double> xw;
	vector<double> yw;
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
			if (curRow == 0)
			{
				while (getline(s, num, ' '))
				{
					xw.push_back(stod(num));
				}
			}
			else
			{
				while (getline(s, num, ' '))
				{
					yw.push_back(stod(num));
				}
			}
			

			//userEquations[curRow] = row;
			curRow += 1;
			//row.clear();
		}
	}

	fileData.close();

	vector<double> c = generateNewton(xw, yw);

	dispalyNewton(c, xw);

	displayLagrange(xw, yw);


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

vector<double> generateNewton(const vector<double>& x, vector<double> f)          // Gets Newton polynomial
{
	int N = x.size();
	int count = 0;
	vector<double> c(N), temp(N);

	c[0] = f[0];
	cout << "Divided Diffrence Tree" << endl;
	cout << "x: ";
	
	for (int i = 0; i < N;i++)
	{
		cout << x[i] << " ";
	}
	cout << endl <<"f[]: ";
	for (int i = 0; i < N;i++)
	{
		cout << f[i] << " ";
	}
	cout << endl;
	for (int i = 1; i < N; i++)       // Compute ith differences
	{
		count++;
		cout << "f[";
		for (int i = 0; i < count; i++)
		{
			cout << ",";
		}
		cout << "]: ";
		for (int j = 0; j < N - i; j++)
		{
			temp[j] = (f[j + 1] - f[j]) / (x[j + i] - x[j]);//calculations
			cout << temp[j] << " ";
		}
		cout << endl;
		f = temp;
		c[i] = f[0];
	}
	return c;
}


void dispalyNewton(const vector<double>& c, const vector<double>& x)
{
	cout << "\n\n";
	cout << "Newton Polynomial:   " << endl;
	int N = x.size();
	cout << c[0];
	for (int i = 1; i < N; i++)
	{
		cout << showpos << c[i];
		for (int j = 0; j < i; j++)
		{
			cout << "(x" << showpos << -x[j] << ")";
		}
	}
	cout << '\n';
}


void displayLagrange(const vector<double>& x, vector<double> f)
{
	cout << "\n\n";
	cout << "Lagrange Polynomial:   " << endl;
	int N = x.size();
	int count = 1;
	double den;
	vector<double> coef(N);
	vector<string> numer(N);

	for (int i = 0; i < N;i++)
	{
		den = f[i];
		for (int j = 0;j < N;j++)
		{
			if (x[i] != x[j])
			{
				den *= 1/(x[i] - x[j]);
				if (x[j] == 0)
				{
					numer[i] += "(x)";
					//cout << "(x)";
				}
				else
				{
					if (x[j] < 0)
					{
						numer[i] += "(x+" + TrimString( to_string(-x[j]) )+ ")";
					}
					else
					{
						numer[i] += "(x-" + TrimString(to_string(x[j])) + ")";
					}
					//cout << "(x" << showpos << -x[j] << ")";
				}
			}
			
		}		
		cout << noshowpos << den << numer[i];
	}
}

string TrimString(string str)
{
	for (int i = str.length() - 1; i > 0; --i)
	{
		if (str[i] == '0' || str[i] == '.')
		{
			str.erase(i, 1);
		}
		else
		{
			break;
		}
	}
	return str;
}
