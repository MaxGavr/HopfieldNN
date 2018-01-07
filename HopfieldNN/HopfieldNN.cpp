#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <ctime>

#include "NeuralNetwork.h"

using namespace std;

NeuralNetwork network;

void readInt(int& value)
{
    cin >> value;
    cin.ignore();
}

void alphaviteMenu()
{
    cout << endl << "Выберите способ задания алфавита:" << endl;
    cout << "1. Вручную" << endl;
    cout << "2. Загрузить из файла" << endl;
    cout << "---------------------------------" << endl;
    cout << "Выбор: ";

    int answer = -1;
    while (answer < 0 || answer > 2)
    {
        readInt(answer);

        if (answer == 1)
        {
            cout << "Длина образа: ";
            int imageWidth;
            readInt(imageWidth);

            cout << "Количество образов: ";
            int imageCount;
            readInt(imageCount);

            cout << "Введите образы в формате:\n\t<название образа> : <образ>" << endl;

            for (int i = 0; i < imageCount; ++i)
            {
                Symbol symbol;
                cout << "Введите образ №" << i + 1 << ":" << endl;

                cin >> symbol;
                network.addSymbol(symbol);
            }
        }
        else if (answer == 2)
        {
            cout << "Введите имя файла: ";
            string fileName;
            cin >> fileName;

            network.loadAlphavite(fileName);
        }
        else if (answer == 0)
            return;
    }

    cout << "Алфавит задан: " << network.getAlphaviteSize() << " образов, ";
    cout << "длина образа - " << network.getSymbolWidth() << endl;
    
    system("pause");
}

void recognizeMenu()
{
    if (network.getAlphaviteSize() == 0)
    {
        cout << endl << "Алфавит не задан!" << endl;
        return;
    }

    cout << endl << "Выберите способ задания распознаваемого образа: " << endl;
    cout << "1. Вручную" << endl;
    cout << "2. Загрузить из файла" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "Выбор: ";

    Symbol symbolToRecognize;
    symbolToRecognize.setTwoDimensional(network.getTwoDimensionalImages());

    int answer = -1;
    while (answer < 0 || answer > 2)
    {
        readInt(answer);

        if (answer == 1)
        {
            cout << "Введите образ длиной " << network.getSymbolWidth() << " в формате: " << endl;
            cout << "\t<название образа> : <образ>" << endl;

            cin >> symbolToRecognize;
        }
        else if (answer == 2)
        {
            cout << "Введите название файла: ";
            string fileName;
            cin >> fileName;

            symbolToRecognize.loadFromFile(fileName);
        }
        else if (answer == 0)
            return;
    }

    cout << "Распознаваемый образ задан:" << endl;
    cout << symbolToRecognize;
    system("pause");

    cout << "Обучение сети..." << endl;
    network.train();

    cout << "Распознавание образа..." << endl;
    network.recognize(symbolToRecognize);
}

void showAlphavite()
{
    if (network.getAlphaviteSize() == 0)
    {
        cout << endl << "Алфавит не задан!" << endl;
        return;
    }

    NeuralNetwork::Alphavite alphavite = network.getAlphavite();
    cout << "Алфавит сети содержит " << network.getAlphaviteSize() << " образов длиной " << network.getSymbolWidth() << endl;
    for (const Symbol& symbol : alphavite)
        cout << symbol << endl;
}

void paramsMenu()
{
    int answer = -1;
    while (answer != 0)
    {
        system("cls");

        cout << "Текущие параметры сети:" << endl;
        cout << "    - количество запомненных образов: " << network.getAlphaviteSize() << endl;
        cout << "    - размер образа: " << network.getSymbolWidth() << endl;
        cout << "(1) - отображать матрицу весов: " << std::boolalpha << network.getShowWeightMatrix() << endl;
        cout << "(2) - отображать выходы нейронов: " << std::boolalpha << network.getShowNeuronsOutput() << "." << endl;
        cout << "(3) - двумерные образы: " << std::boolalpha << network.getTwoDimensionalImages() << endl;
        cout << "(4) - период релаксации: " << network.getRelaxationPeriod() << endl;
        cout << "--------------------------------------" << endl;
        cout << "0. Назад" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Действие: ";

        readInt(answer);

        switch (answer)
        {
        case 1:
            network.showWeightMatrix(!network.getShowWeightMatrix());
            break;
        case 2:
            network.showNeuronsOutput(!network.getShowNeuronsOutput());
            break;
        case 3:
            network.setTwoDimensionalImages(!network.getTwoDimensionalImages());
            break;
        case 4:
            cout << "Введите длительность периода релаксации (кол-во итераций): ";
            int period;
            readInt(period);
            network.setRelaxationPeriod(period);
            break;
        case 0:
            return;
        }
    }
}

void menu()
{
    cout << "Релаксационная сеть Хопфилда" << endl;

    int answer = -1;
    while (answer != 0)
    {
        cout << endl;
        cout << "Действия:" << endl;
        cout << "1. Распознать образ" << endl;
        cout << "2. Задать алфавит" << endl;
        cout << "3. Просмотреть алфавит" << endl;
        cout << "4. Задать параметры сети" << endl;
        cout << "5. Очистить консоль" << endl;
        cout << "0. Выход" << endl;
        cout << "----------------------" << endl;
        cout << "Выбор: ";

        int answer;
        readInt(answer);

        switch (answer)
        {
        case 1:
            recognizeMenu();
            break;
        case 2:
            alphaviteMenu();
            break;
        case 3:
            showAlphavite();
            break;
        case 4:
            paramsMenu();
            break;
        case 5:
            system("cls");
            break;
        case 0:
            cout << "Завершение работы..." << endl;
            return;
            break;
        }
    }
}


int main()
{
	setlocale(LC_ALL, "");
    srand(time(NULL));

    menu();

	return 0;
}
