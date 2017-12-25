#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <ctime>

#include <armadillo>

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

	int WIDTH = 9;
	int m = 0;
	int step = 0;

	cout << "Сеть Хопфилда.\n\n";
    cout << "Введите размер входного образа:" << endl;
    cout << "1. Произвольный" << endl;
    cout << "2. По умолчанию (9)" << endl;

	int choose;
	cin >> choose;
	cout << "Алфавит образов: \n";
	if (choose == 2)
    {
		srand(time(NULL));
		for (char letter = 'a'; letter < 'c'; letter++)
        {
			string filename = "x.txt";
			filename[0] = letter;

			ifstream fileStream(filename);
			string representation(istreambuf_iterator<char>{fileStream}, {});
			fileStream.close();

			symbols.push_back(representation);
			parsedSymbols.push_back(parseFromString(representation));

			cout << m + 1 << ". " << symbols[m] << endl;
			m++;
		}

		cout << "\nВвести образ самостоятельно или воспользователься системным?\n1.Свой;\n2.Системный:\n";
		cout << testSymbol << endl;
		cin >> choose;
		if (choose ==1)
        {
			cout << "Введите " << WIDTH << " символов :\n";
			string temp;
			cin >> temp;

			testSymbol += temp;
		}
	}
	else
    {
		int number;
		cout << "Введите ширину символов : \n";
		cin >> WIDTH;
		cout << "Введите число символов в афавите : \n";
		cin >> number;

		string sign;
		for (int i = 0; i < number; i++)
        {
			cout << "Введите символ алфавита: \n";
			cout << "Введите " << WIDTH << "символов :\n";
			string temp;
			cin >> temp;
			sign += temp;

			symbols.push_back(sign);
			parsedSymbols.push_back(parseFromString(sign));

			m++;
		}
		cout << "Введите обрабатываемый образ " << WIDTH << " символов :\n";
		string temp;
		cin >> temp;

		testSymbol += temp;
	}

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
			if (relax[i] == Y/* && neuron == WIDTH*/)
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

	return 0;
}
