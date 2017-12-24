// HopfieldNN.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"


#include <vector>
#include <string>
#include <fstream>
#include <iostream>
// 2003-2010 Free Software Foundation Inc.
#include <ctime>
//1997-2015 Free Software Foundation, Inc.

/*
14.12.2017 ����� - ���� ���������
*/

using namespace std;

/* ���������� ����������
*  letters - ������ ������� � ���� ������;
*  parsedLetters - ������ ������� � ���� ����� (1 � -1)
*  13.12.2017 - �����: ���� ���������.
*/
vector<string> letters;
vector<vector<int>> parsedLetters;

/* ������� ��� ���������� �������� �������� � �����
*  ���������� ������ �� �������� �������� �������, ������� �� � 1 (��� #) ��� -1(��� .).
*  13.12.2017 - �����: ���� ���������.
*/
vector<int> parseFromString(string letter) {
	vector<int> result;
	for (int i = 0; i < letter.size(); ++i) {
		switch (letter[i]) {
		case '.': {
			result.push_back(-1);
			break;
		}
		case '#': {
			result.push_back(1);
			break;
		}
		}
	}
	return result;
}

int main() {
	setlocale(LC_ALL, "");

	int WIDTH = 5;
	int m = 0;
	int step = 0;

	cout << "������������ ������ �3. \n�������������� ��������� ����.\n������������� �������� ����� ��������.\n\n";
	cout << "����� ������ ������? (���������������� (��������� ������ �������) ��� ��������� (5))\n\n";
	cout << "1. ����������������.\n2. ���������.\n";
	int choose;
	string testLetter;
	cin >> choose;
	cout << "������� �������: \n";
	if (choose == 2) {
		srand(time(NULL));
		for (char letter = 'a'; letter < 'j'; letter++) {
			string filename = "x.txt";
			filename[0] = letter;
			ifstream fileStream(filename);
			string representation(istreambuf_iterator<char>{fileStream}, {});
			fileStream.close();
			letters.push_back(representation);
			parsedLetters.push_back(parseFromString(representation));
			cout << m + 1 << ". " << letters[m] << endl;
			m++;
		}
		cout << "\n������ ����� �������������� ��� ����������������� ���������?\n1.����;\n2.���������:\n";
		cout << ". . . . . " << endl;
		cin >> choose;
		if (choose == 2) {
			testLetter = "#####";
		}
		else {
			cout << "������� " << WIDTH << " �������� :\n";
			string temp;
			cin >> temp;
			testLetter += temp;
		}
	}
	else {
		int number;
		cout << "������� ������ �������� : \n";
		cin >> WIDTH;
		cout << "������� ����� �������� � ������� : \n";
		cin >> number;
		string sign;
		for (int i = 0; i < number; i++) {
			cout << "������� ������ ��������: \n";
			cout << "������� " << WIDTH << "�������� :\n";
			string temp;
			cin >> temp;
			sign += temp;
			letters.push_back(sign);
			parsedLetters.push_back(parseFromString(sign));
			m++;
		}
		cout << "������� �������������� ����� " << WIDTH << " �������� :\n";
		string temp;
		cin >> temp;
		testLetter += temp;
	}

	cout << "\n������������� ����� ������� ����� ��������� ������? (1-��, 2-���.) :\n";
	cin >> step;

	vector<vector<int>> Y;
	vector<vector<int>> Yi;
	vector< vector<vector<int>>> W;
	vector<vector<int>> weight;
	vector<int> temp;

	// ����������� ������� ������ ��� ���� ������� �� ������� W = (2*Y -1).t * (2*Y -1) - ��������� �������.
	// C���������� ������� Y, ������� ����� m ������� �� parseFromString(testLetter)
	// (!!!!! ���, ��� ������ ������� � Y, ������� ������, ��� ��� ����� ���������)
	// � ������� ��� ������ ������� ������ ��� ������ ������, � � ��� �����.
	// �� ����, � ���� ����� �������� m �����, ������ ����� ���������� ������� ������ � �����-�� ����� ��������.
	// ��, ��� ������� ����������, "�������".
	// (!!!!!) ��������, �������, �� ���� � ���� ���� ��� �������� � ���� ����.

	if (step == 1) cout << "������� �������: \n";
	for (int i = 0; i < m; i++) {
		if (step == 1) cout << "������� ������� " << i << ": \n";
		Y.push_back(parseFromString(testLetter));
		Yi.push_back(temp);
		W.push_back(weight);
		for (int j = 0; j < WIDTH; j++) {
			W[i].push_back(temp);
			for (int k = 0; k < WIDTH; k++) {
				W[i][j].push_back((2 * parsedLetters[i][j] - 1)*(2 * parsedLetters[i][k] - 1));
				if (j == k) W[i][j][k] = 0;
				if (step == 1) cout << W[i][j][k] << " ";
			}
			if (step == 1) cout << endl;
		}
	}
	int itr = 1;
	int result = 0;
	bool nn = true;
	if (step == 1) cout << " ������������� ������ : \n";
	while (nn) {
		for (int i = 0; i < WIDTH; i++) {
			int s = 0;
			for (int k = 0; k < m; k++) {
				for (int j = 0; j < WIDTH; j++) {
					s += Y[k][j] * W[k][i][j];
				}
				if (step == 1) cout << " ���������� ����� ������ " << k + 1 << " �������� " << i << ": \n";
				if (s > 0) Y[k][i] = 1;
				else Y[k][i] = -1;
				if (step == 1) {
					cout << " ������� ��������� : ";
					for (int n = 0; n < WIDTH; n++) {
						if (Y[k][n] == -1) cout << ". ";
						if (Y[k][n] == 1) cout << "# ";
					}
					cout << endl;
				}
				if (Y[k] == parsedLetters[k]) {
					if (Yi[k] == Y[k]) {
						temp.push_back(k);
						k = m;
						i = WIDTH;
						nn = false;
					}
					else Yi[k] = Y[k];
				}
			}
		}
		cout << "�������� : " << itr << endl;
		itr++;
	}
	for (int i = 0; i < temp.size(); i++) {
		result = temp[i];
		cout << "��������� ����� ������������� ������ " << 1 + result << " :" << letters[result] << endl;
	}
	system("pause");
	return 0;
}
