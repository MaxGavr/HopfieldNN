// HopfieldNN.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iomanip>
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
string testLetter = "...#.#.##";

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

	int WIDTH = 9;
	int m = 0;
	int step = 0;

	cout << "Лабораторная работа №3. \nРелаксационные нейронные сети.\nРаспознавание символов сетью Хопфилда.\n\n";
	cout << "Какой размер образа? (пользовательский (необохимо ввести алфавит) или системный (9))\n\n";
	cout << "1. Пользовательский.\n2. Системный.\n";
	int choose;
	cin >> choose;
	cout << "Алфавит образов: \n";
	if (choose == 2) {
		srand(time(NULL));
		for (char letter = 'a'; letter < 'c'; letter++) { //j
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
		cout << testLetter << endl;
		cin >> choose;
		if (choose ==1){
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
	cout << endl;
	vector<int> Y;
	vector<vector<int>> relax; // нужно чтоб понять достигла ли сеть релаксации или нет (по заданию не требуется)
	bool relaxBool = false;
	vector<vector<double>> W;
	vector<double> temp;
	int result = -1;
	if (step == 1) cout << "Весовая матрица: \n";
	for (int i = 0; i < WIDTH; i++) {
		W.push_back(temp);
		for (int j = 0; j < WIDTH; j++) {
			int sum = 0;
			for (int k = 0; k < m; k++) {
				sum += parsedLetters[k][j] * parsedLetters[k][i];
			}
			if (i == j) sum = 0;
			W[i].push_back(sum);
			cout.precision(2);
			if (step == 1) cout << W[i][j] << " ";
		}
		if (step == 1) cout << endl;
	}
	if (step == 1)  cout << endl;
	Y = parseFromString(testLetter);
	bool cont = true;
	int itr = 1;
	int neuron = -1;
	while (cont && relaxBool != true) {
		if (neuron < WIDTH-1) neuron++;
		else neuron = 0;		
		double sum = 0;
		for (int i = 0; i < WIDTH; i++) {
			sum += W[i][neuron] * Y[neuron];
		}
		if (sum > 0) Y[neuron] = 1;
		else Y[neuron] = -1;
		for (int i = 0; i < relax.size(); i++) {
			if (relax[i] == Y && neuron == WIDTH ) relaxBool = true;
		}
		for (int i = 0; i < m; i++) {
			if (Y == parsedLetters[i]) {
				result = i;
				cont = false;
			}
		}
		itr++;
		relax.push_back(Y);
		cout << "\nИтерация " << itr << ": ";
		for (int i = 0; i < WIDTH; i++) {
			if (Y[i] == 1) cout << "# ";
			else cout << ". ";
		}
		if (neuron == WIDTH) cout << endl;
	}
	if (relaxBool == true) cout << "\n\nСеть достигла релаксации.\n";
	if (result != -1 )cout << "Введенный образ соответствует образу " << 1 + result << " :" << letters[result] << endl;
	system("pause");
	return 0;
}
