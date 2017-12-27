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

    /*
    int WIDTH = 9;
    int m = 0;
    int step = 0;

	cout << "\nПросматривать выход нейрона сетив пошаговом режиме? (1-да, 2-нет.) :\n";
	cin >> step;
	cout << endl;

    // initialize weight matrix
    vector<vector<double>> W(WIDTH, vector<double>(WIDTH, 0));

	for (int i = 0; i < WIDTH; i++)
    {
		for (int j = 0; j < WIDTH; j++)
        {
            if (i == j)
                continue;

			for (int k = 0; k < m; k++)
				W[i][j] += parsedSymbols[k][j] * parsedSymbols[k][i];
		}
	}

    // print out weight matrix
    if (step == 1)
    {
        cout << "Весовая матрица: \n";

        for (const vector<double>& row : W)
        {
            for (double value : row)
                cout << setprecision(2) << value << " ";
            cout << endl;
        }

        system("pause");
        cout << endl;
    }

    vector<int> Y = parseFromString(testSymbol);

    vector<vector<int>> relax; // нужно чтоб понять достигла ли сеть релаксации или нет (по заданию не требуется)
    bool relaxBool = false;

    int result = -1;

	bool cont = true;
	int iteration = 1;
	int neuron = -1;
	while (cont && !relaxBool)
    {
		if (neuron < WIDTH-1)
            neuron++;
		else
            neuron = 0;

		double sum = 0;
		for (int i = 0; i < WIDTH; i++)
			sum += W[i][neuron] * Y[neuron];

		if (sum > 0)
            Y[neuron] = 1;
		else
            Y[neuron] = -1;

		for (int i = 0; i < relax.size(); i++)
        {
			if (relax[i] == Y && neuron == WIDTH)
                relaxBool = true;
		}

		for (int i = 0; i < m; i++)
        {
			if (Y == parsedSymbols[i])
            {
				result = i;
				cont = false;
			}
		}

		iteration++;
		relax.push_back(Y);

		cout << "\nИтерация " << iteration << ": ";
		for (int i = 0; i < WIDTH; i++)
        {
			if (Y[i] == 1)
                cout << "# ";
			else
                cout << ". ";
		}
		if (neuron == WIDTH)
            cout << endl;
	}

	if (relaxBool == true)
        cout << "\n\nСеть достигла релаксации.\n";

	if (result != -1 )
        cout << "Введенный образ соответствует образу " << 1 + result << " :" << symbols[result] << endl;
        */
	return 0;
}
