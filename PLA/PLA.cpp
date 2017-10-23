// PLA.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define pocket
using namespace std;
double train_data[4001][100];//[ccnt] is the overcome;
double valid_data[40001][100];
int result[4001];
int valid_result[4001];
int col = 0;
double w[100] = { 0 };
int sign(double value)
{
	if (value > 0)
	{
		return 1;
	}
	else if (value < 0)
	{
		return -1;
	}
	else return 0;
}
int  predict(int row) 
{
	double value = 0;
	for (int i = 0; i < col; i++)
	{
		value += w[i] * train_data[row][i];
	}
	if (result[row] != sign(value))
	{
		return -1;
	}
	else
	{
		return 1;
	}
}
int  valid_predict(int row)
{
	double value = 0;
	for (int i = 0; i < col; i++)
	{
		value += w[i] * valid_data[row][i];
	}
	if (valid_result[row] != sign(value))
	{
		return -1;
	}
	else
	{
		return 1;
	}
}
int main()
{
	ifstream tr("F:\\本科课程\\大三上\\人工智能\\lab3(PLA)\\lab3数据\\train.csv");
	string s;
	int rcnt = 0;//row counter

	while (getline(tr,s))
	{
		
		int ccnt = 0;//coloum counter
		train_data[rcnt][ccnt++] = 1;
		while (s.find(',') != -1)
		{
			train_data[rcnt][ccnt++] = stod(s.substr(0,s.find(',')));
			s = s.substr(s.find(',') + 1);
		}
		result[rcnt++] = stoi(s);
		col = ccnt;
	}

	for (int i = 0; i < col; i++)
	{
		w[i] = 1;
	}
#ifdef pocket
	double wk[100];
	int cort_max = 0;
#endif // pocket

	int n = 40;
	for (int i = 0; i < n; i++)
	{
		for (int k = 0; k < rcnt; k++)
		{
			int t = predict(k);
			if (t == -1 )
			{
				for (int j = 0; j < col; j++)
				{
					w[j] = w[j] + result[k] * train_data[k][j];
				}

#ifdef pocket
				double cort = 0;
				for (int j = 0; j < rcnt; j++)
				{
					if (predict(j) == 1)
					{
						cort++;
					}
				}
				if (cort > cort_max)
				{
					cort_max = cort;
					for (int j = 0; j < col; j++)
					{
						wk[j] = w[j];
					}
				}
#endif // pocket
			}
		}
	}

	ifstream va("F:\\本科课程\\大三上\\人工智能\\lab3(PLA)\\lab3数据\\val.csv");
	int vrcnt = 0;
	while (getline(va, s))
	{
		int ccnt = 0;//coloum counter
		valid_data[vrcnt][ccnt++] = 1;
		while (s.find(',') != -1)
		{
			valid_data[vrcnt][ccnt++] = stod(s.substr(0, s.find(',')));
			s = s.substr(s.find(',') + 1);
		}
		valid_result[vrcnt++] = stoi(s);
	}
#ifdef pocket
	for (int i = 0; i < col; i++)
	{
		w[i] = wk[i];
	}
#endif // pocket


	//validate
	double cort = 0.0000;
	double recall = 0, precision = 0, F1 = 0;
	int TP = 0, TN = 0, FP = 0, FN = 0;
	for (int i = 0; i < vrcnt; i++)
	{
		if (valid_predict(i) == 1)
		{
			if (valid_result[i] == 1)
			{
				TP++;
			}
			else
			{
				TN++;
			}
		}
		else
		{
			if (valid_result[i] == 1)
			{
				FN++;
			}
			else
			{
				FP++;
			} 
		}
	}
	double Accuracy = ((double)(TP + TN)) / (TP + TN + FP + FN),
		   Recall = ((double)(TP)) / (TP + FN),
		   Precision = (double)TP / (TP + FP);
	cout << "Accuracy : " << Accuracy << endl
		<< "Recall :    " << Recall << endl
		<< "Precision : " << Precision << endl
		<< "F1 :        " << 2 * Recall*Precision / (Recall + Precision);
	
	system("pause");
    return 0;
}

