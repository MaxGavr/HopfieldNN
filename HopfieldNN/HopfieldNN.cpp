#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <ctime>

#include "NeuralNetwork.h"

using namespace std;


vector<string> symbols;
vector<vector<int>> parsedSymbols;
string testSymbol = "...#.#.##";

vector<int> parseFromString(string letter)
{
    letter.erase(remove_if(letter.begin(), letter.end(), isspace), letter.end());
    vector<int> result(letter.size());

    auto toInt = [](char c) -> int
    {
        if (c == '.')
            return -1;
        else if (c == '#')
            return 1;
        else
            throw logic_error("Неверное значение входного вектора!");
    };

    transform(letter.begin(), letter.end(), result.begin(), toInt);

	return result;
}

int main()
{
	setlocale(LC_ALL, "");
    srand(time(NULL));

    NeuralNetwork::generateAlphavite("alphavite.txt", 9, 2);

    NeuralNetwork nn;

    cout << "1. Ввести множество образов вручную" << endl;
    cout << "   или " << endl;
    cout << "2. Загрузить образы из файла" << endl;
    cout << "Ваш выбор: ";

	int choice;
	cin >> choice;

    if (choice == 1)
    {
        cout << endl << "Введите размер входного образа: ";

        int imageSize;
        cin >> imageSize;

        cout << endl << "Введите количество образов: ";

        int imageAmount;
        // TODO: check for max value (N - 1)
        cin >> imageAmount;

        // ignore newline character
        string tmp;
        getline(cin, tmp);

        Symbol symbol;
        for (int i = 0; i < imageAmount; ++i)
        {
            cin >> symbol;
            nn.addSymbol(symbol);
        }
    }
    else
        nn.loadAlphavite("alphavite.txt");

    cout << endl << "Алфавит сети: " << endl;
    for (const Symbol& symbol : nn.getAlphavite())
        cout << symbol << endl;

    system("pause");

    nn.train();

    cout << nn.mWeights;

    Symbol test("test");
    test.setImage("....#....");

    nn.recognize(test);

	return 0;
}
