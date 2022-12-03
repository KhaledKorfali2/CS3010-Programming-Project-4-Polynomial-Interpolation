#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<chrono>
#include<vector>
#include <iomanip>
#include<sstream>
using namespace std;


//max constants
const int KMAX = 50;

//Function Prototypes
string TrimString(string str);
vector<double> generateNewton(const vector<double>& x, vector<double> f); 
vector<double> standardPolynomial(const vector<double>& newton, const vector<double>& x);
void dispalyNewton(const vector<double>& c, const vector<double>& x);
void displayLagrange(const vector<double>& x, vector<double> f);
void displaySimplifiedPolynomial(const vector<double>& a);
void displayDivDiffTree(const vector<double>& x, vector<double> f);


int main()
{

	//Initialize cout to print only 2 places after decimal points
	cout << fixed;
	cout << setprecision(2);


	//Variable Declarations
	float sum = 0;
	string userFile = "";
	
	//Get User Input
	cout << "Please enter the name of your properly formated txt file: " << endl;
	cin >> userFile;


	//Reads in the data line by line storing values in xw and yw respectively
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
			curRow += 1;
		}
	}

	fileData.close();

	//Calculate Newton and Standard Polynomials
	vector<double> c = generateNewton(xw, yw);
	vector<double> a = standardPolynomial(c, xw);
	
	
	//Displays Outputs
	displayDivDiffTree(xw, yw);
	dispalyNewton(c, xw);
	displayLagrange(xw, yw); //caluclations for lagrange done while displaying
	displaySimplifiedPolynomial(a);
}

//Displays the divided diffrence tree
void displayDivDiffTree(const vector<double>& x, vector<double> f)
{
	int N = x.size();
	int count = 0;
	vector<double> temp(N);

	cout << "Divided Diffrence Tree" << endl;
	
	//Displays x values
	cout << "x: ";
	for (int i = 0; i < N;i++)
	{
		cout << x[i] << " ";
	}
	
	//displays f[] values
	cout << endl << "f[]: ";
	for (int i = 0; i < N;i++)
	{
		cout << f[i] << " ";
	}

	//displays f[,], f[,,], ... f[,...,] values
	cout << endl;
	for (int i = 1; i < N; i++)
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
			//originally temp[j]
			f[j] = (f[j + 1] - f[j]) / (x[j + i] - x[j]);
			//originally temp[j]
			cout << f[j] << " ";
		}
		cout << endl;
		//f = temp;
	}
}

//Calculates Newton Polynomial
vector<double> generateNewton(const vector<double>& x, vector<double> f)
{
	int N = x.size();
	vector<double> c(N), temp(N);

	//Sets first element in c equal to first coefficent in function
	c[0] = f[0];
	//Computes calculations 
	for (int i = 1; i < N; i++)
	{
		for (int j = 0; j < N - i; j++)
		{
			temp[j] = (f[j + 1] - f[j]) / (x[j + i] - x[j]);
		}
		f = temp;
		c[i] = f[0];
	}
	return c;
}

//Displays Newton Polynomial
void dispalyNewton(const vector<double>& c, const vector<double>& x)
{
	cout << "\n\n";
	cout << "Newton Polynomial:   " << endl;
	ostringstream streamObj;
	streamObj << fixed;
	streamObj << setprecision(2);
	int start = 0;
	int N = x.size();
	//Only prints the first element of c if its not 0, if it is 0 don't print and mark next element as potential first element to print
	if (c[0] != 0)
	{
		cout << TrimString(to_string(c[0]));
	}
	else
	{
		start += 1;
	}
	//iterates over each element of c
	for (int i = 1; i < N; i++)
	{
		//clear streamObj
		streamObj.str("");
		streamObj << c[i];
		//If an element in c = 0 don't print and move to next element
		if (c[i] != 0)
		{
			if (c[i] > 0)
			{
				//if the current element is the new start point don't include a "+" at begining
				if (c[i] == start)
				{
					cout << TrimString(streamObj.str());
				}
				else
				{
					cout << "+" << TrimString(streamObj.str());
				}
			}
			else
			{
				cout << TrimString(streamObj.str());
			}
			//Finds all xj in (x-xj) 
			for (int j = 0; j < i; j++)
			{
				streamObj.str("");
				if (x[j] == 0)
				{
					cout << "(x)";
				}
				else if (x[j] < 0)
				{
					streamObj << -x[j];
					cout << "(x" << "+" << TrimString(streamObj.str()) << ")";
				}
				else
				{
					streamObj << x[j];
					cout << "(x" << "-" << TrimString(streamObj.str()) << ")";
				}
				//cout << "(x" << showpos << TrimString(to_string(-x[j])) << ")";
			}
		}
		else
		{
			start += 1;
		}
		
	}
	cout << '\n';
}

//Computes and Displays lagrange polynomial
void displayLagrange(const vector<double>& x, vector<double> f)
{
	cout << "\n\n";
	cout << "Lagrange Polynomial:   " << endl;

	//Format streamObj
	ostringstream streamObj;
	streamObj << fixed;
	streamObj << setprecision(2);

	//Initialize values
	int start = 0;
	int N = x.size();
	int count = 1;
	double den;
	vector<double> coef(N);
	vector<string> numer(N);
	
	//For each coefficent in f
	for (int i = 0; i < N;i++)
	{
		//initializes den as the ith coeffiecnt of the function
		den = f[i];
		//Clear streamObj
		streamObj.str("");

		//For each value xj in x check that the values are not equal and then put it in (x - xj) form
		for (int j = 0;j < N;j++)
		{
			//Clear streamObj
			streamObj.str("");
			if (x[i] != x[j])
			{
				//Compute the denominator
				den *= 1/(x[i] - x[j]);
				if (den != 0)
				{
					//Prints (x-xj) in proper format depending on if xj is "+", "-", or 0
					if (x[j] == 0)
					{
						numer[i] += "(x)";
						//cout << "(x)";
					}
					else
					{
						if (x[j] < 0)
						{
							streamObj << -x[j];
							numer[i] += "(x+" + TrimString(streamObj.str()) + ")";
							//numer[i] += "(x+" + TrimString(to_string(-x[j])) + ")";
						}
						else
						{
							streamObj << x[j];
							numer[i] += "(x-" + TrimString(streamObj.str()) + ")";
							//numer[i] += "(x+" + TrimString(to_string(x[j])) + ")";
						}
						//cout << "(x" << showpos << -x[j] << ")";
					}
				}

				else
				{
					start += 1;
				}
				
			}
			
		}
		//Clear streamObj
		streamObj.str("");
		streamObj << den;
		//Prints polynomial in newton form (den +/- den() +/- den()() +/-...)
		if (den < 0)
		{
			cout << TrimString(streamObj.str()) << numer[i];
		}
		else if (den > 0)
		{
			if (i == start)
			{
				cout << TrimString(streamObj.str()) << numer[i];
			}
			else
			{
				cout << "+" << TrimString(streamObj.str()) << numer[i];
			}
		}
		//cout << noshowpos << TrimString(streamObj.str()) << numer[i];
	}
}

//Convert Newton Polynomial to Standard Form
vector<double> standardPolynomial(const vector<double>& newton, const vector<double>& x)
{
	int N = x.size();

	//Final coefficeints for x^N, x^N-1, ... x^1, x^0
	vector<double> coeficient(N, 0.0);        
	//Stores the factors for newton polynimial
	vector<double> poly(N), prev(N);

	//Multiplies out the coeffiecents in newton
	poly[0] = 1;
	coeficient[0] = newton[0] * poly[0];
	for (int i = 1; i < N; i++)
	{
		prev = poly;
		poly[0] = -x[i - 1] * prev[0];
		coeficient[0] += newton[i] * poly[0];
		for (int j = 1; j <= i; j++)
		{
			poly[j] = prev[j - 1] - x[i - 1] * prev[j];
			coeficient[j] += newton[i] * poly[j];
		}
	}

	return coeficient;
}

void displaySimplifiedPolynomial(const vector<double>& a)
{
	cout << "\n\n";
	cout << "Simplified Polynomial" << endl;
	//Formats streamObj
	ostringstream streamObj;
	streamObj << fixed;
	streamObj << setprecision(2);
	//Initialize values
	int N = a.size();
	int start = N - 1;

	//For each coefficent in a print out appropriate value
	for (int i = N-1; i > 0; --i)
	{
		//Clear streamObj
		streamObj.str("");
		//Don't display coefficient if it is 0 or 1
		if (a[i] != 0 && a[i] != 1)
		{
			streamObj << a[i];
		}
		else if (a[i] != 0 && a[i] == 1)
		{
			//do nothing
		}
		//mark next element in a as potential first element to print
		else
		{
			start -= 1;
		}
		//if degree is 1 don't show x^{1} only x
		if (i == 1)
		{
			if (i == start)
			{
				cout << TrimString(streamObj.str()) << "x";
			}
			else if (a[i] < 0)
			{
				cout <<  TrimString(streamObj.str()) << "x";
			}
			else if (a[i] > 0)
			{
				cout << "+" << TrimString(streamObj.str()) << "x";
			}

			//cout << showpos << TrimString(to_string(a[i])) << "x";	
		}
		//Show coefficent times x^{i}
		else
		{
			//cout << showpos << TrimString(to_string(a[i])) << "x^" << noshowpos << i;
			if (i == start)
			{
				cout << TrimString(streamObj.str()) <<"x^{" << i << "}";
			}
			else if (a[i] < 0)
			{
				cout <<  TrimString(streamObj.str()) << "x^{" <<  i << "}";
			}
			else if (a[i] > 0)
			{
				cout << "+" << TrimString(streamObj.str()) << "x^{" <<  i << "}";
			}
		}
	}
	//Clear streamObj
	streamObj.str("");
	streamObj << a[0];
	//Print first element in a
	if (a[0] < 0)
	{
		cout <<  TrimString(streamObj.str());
	}
	else if (a[0] > 0)
	{
		cout << "+" << TrimString(streamObj.str());
	}
	//cout << showpos << TrimString(to_string(a[0]));
	cout << '\n';
}

//Trims extra 0's in a decimal number ex. 2.40000 => 2.4
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
