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


//constants
const int KMAX = 50;

//Function Prototypes
string TrimString(string str);

void dispalyNewton(const vector<double>& c, const vector<double>& x);
vector<double> generateNewton(const vector<double>& x, vector<double> f); 
void displayLagrange(const vector<double>& x, vector<double> f);
void displaySimplifiedPolynomial(const vector<double>& a);
vector<double> standardPolynomial(const vector<double>& newton, const vector<double>& x);
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

vector<double> generateNewton(const vector<double>& x, vector<double> f)
{
	int N = x.size();
	vector<double> c(N), temp(N);

	c[0] = f[0];
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


void dispalyNewton(const vector<double>& c, const vector<double>& x)
{
	cout << "\n\n";
	cout << "Newton Polynomial:   " << endl;
	ostringstream streamObj;
	streamObj << fixed;
	streamObj << setprecision(2);
	int start = 0;
	int N = x.size();
	if (c[0] != 0)
	{
		cout << TrimString(to_string(c[0]));
	}
	else
	{
		start += 1;
	}
	for (int i = 1; i < N; i++)
	{
		streamObj.str("");
		streamObj << c[i];
		if (c[i] != 0)
		{
			if (c[i] > 0)
			{
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


void displayLagrange(const vector<double>& x, vector<double> f)
{
	cout << "\n\n";
	cout << "Lagrange Polynomial:   " << endl;
	ostringstream streamObj;
	streamObj << fixed;
	streamObj << setprecision(2);
	int start = 0;
	int N = x.size();
	int count = 1;
	double den;
	vector<double> coef(N);
	vector<string> numer(N);
	
	for (int i = 0; i < N;i++)
	{
		den = f[i];
		streamObj.str("");
		for (int j = 0;j < N;j++)
		{
			streamObj.str("");
			if (x[i] != x[j])
			{
				den *= 1/(x[i] - x[j]);
				if (den != 0)
				{
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
		streamObj.str("");
		streamObj << den;
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
	ostringstream streamObj;
	streamObj << fixed;
	streamObj << setprecision(2);
	int N = a.size();
	int start = N - 1;
	for (int i = N-1; i > 0; --i)
	{
		streamObj.str("");
		if (a[i] != 0 && a[i] != 1)
		{
			streamObj << a[i];
		}
		else if (a[i] != 0 && a[i] == 1)
		{

		}
		else
		{
			start -= 1;
		}
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
	streamObj.str("");
	streamObj << a[0];
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
