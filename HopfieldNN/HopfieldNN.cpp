// HopfieldNN.cpp: определяет точку входа для консольного приложения.
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
14.12.2017 автор - Ника Ковальчук
*/

using namespace std;

/* Глобальные переменные
*  letters - хранит алфавит в виде строки;
*  parsedLetters - хранит алфавит в виде чисел (1 и -1)
*  13.12.2017 - автор: Ника Ковальчук.
*/
vector<string> letters;
vector<vector<int>> parsedLetters;

/* Функция для вычисления перевода символов в числа
*  Составляет вектор из значений входного вектора, заменив их в 1 (для #) или -1(для .).
*  13.12.2017 - автор: Ника Ковальчук.
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

	cout << "Лабораторная работа №3. \nРелаксационные нейронные сети.\nРаспознавание символов сетью Хопфилда.\n\n";
	cout << "Какой размер образа? (пользовательский (необохимо ввести алфавит) или системный (5))\n\n";
	cout << "1. Пользовательский.\n2. Системный.\n";
	int choose;
	string testLetter;
	cin >> choose;
	cout << "Алфавит образов: \n";
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
		cout << "\nВвести образ самостоятельно или воспользователься системным?\n1.Свой;\n2.Системный:\n";
		cout << ". . . . . " << endl;
		cin >> choose;
		if (choose == 2) {
			testLetter = "#####";
		}
		else {
			cout << "Введите " << WIDTH << " символов :\n";
			string temp;
			cin >> temp;
			testLetter += temp;
		}
	}
	else {
		int number;
		cout << "Введите ширину символов : \n";
		cin >> WIDTH;
		cout << "Введите число символов в афавите : \n";
		cin >> number;
		string sign;
		for (int i = 0; i < number; i++) {
			cout << "Введите символ алфавита: \n";
			cout << "Введите " << WIDTH << "символов :\n";
			string temp;
			cin >> temp;
			sign += temp;
			letters.push_back(sign);
			parsedLetters.push_back(parseFromString(sign));
			m++;
		}
		cout << "Введите обрабатываемый образ " << WIDTH << " символов :\n";
		string temp;
		cin >> temp;
		testLetter += temp;
	}

	cout << "\nПросматривать выход нейрона сетив пошаговом режиме? (1-да, 2-нет.) :\n";
	cin >> step;

	vector<vector<int>> Y;
	vector<vector<int>> Yi;
	vector< vector<vector<int>>> W;
	vector<vector<int>> weight;
	vector<int> temp;

	// Составление весовых матриц для всех образов по формуле W = (2*Y -1).t * (2*Y -1) - единичная матрица.
	// Cоставление вектора Y, который равен m строкам по parseFromString(testLetter)
	// (!!!!! Все, что дальше сделано с Y, сделано наугад, так что лучше проверить)
	// В Головко был описан алгорит только для одного образа, а у нас много.
	// По сути, у меня будет работать m сетей, каждая будет сравнивать входной вектор с каким-то одним эталоном.
	// Та, что быстрее отработает, "победит".
	// (!!!!!) Повторяю, проверь, мб надо в этой лабе все запихать в одну сеть.

	if (step == 1) cout << "Весовые матрицы: \n";
	for (int i = 0; i < m; i++) {
		if (step == 1) cout << "Весовая матрица " << i << ": \n";
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
	if (step == 1) cout << " Распознавание образа : \n";
	while (nn) {
		for (int i = 0; i < WIDTH; i++) {
			int s = 0;
			for (int k = 0; k < m; k++) {
				for (int j = 0; j < WIDTH; j++) {
					s += Y[k][j] * W[k][i][j];
				}
				if (step == 1) cout << " Взвешенная сумма образа " << k + 1 << " элемента " << i << ": \n";
				if (s > 0) Y[k][i] = 1;
				else Y[k][i] = -1;
				if (step == 1) {
					cout << " Текущий результат : ";
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
		cout << "Итерация : " << itr << endl;
		itr++;
	}
	for (int i = 0; i < temp.size(); i++) {
		result = temp[i];
		cout << "Введенный образ соответствует образу " << 1 + result << " :" << letters[result] << endl;
	}
	system("pause");
	return 0;
}
