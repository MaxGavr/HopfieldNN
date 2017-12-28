#include "NeuralNetwork.h"


Symbol::Symbol(std::string name, std::string image)
    : mName(name),
      mImage(image),
      mBinary(convertImageToBinary(image))
{
}

bool Symbol::operator==(const Symbol& symbol) const
{
    if (this->mImage == symbol.mImage)
        return true;
    else
        return this->mName == symbol.mName;
}

int Symbol::getWidth() const
{
    return mImage.size();
}

void Symbol::setImage(std::string image)
{
    mImage = image;
    mBinary = convertImageToBinary(mImage);
}

arma::Row<int> Symbol::convertImageToBinary(std::string image)
{
    image.erase(std::remove_if(image.begin(), image.end(), isspace), image.end());
    std::vector<int> tmpVector(image.size());

    auto toInt = [](char c) -> int
    {
        if (c == '.')
            return -1;
        else if (c == '#')
            return 1;
        else
            throw std::logic_error("Неверное значение входного вектора!");
    };

    std::transform(image.begin(), image.end(), tmpVector.begin(), toInt);

    return tmpVector;
}

void Symbol::updateImage()
{
    for (int i = 0; i < mBinary.n_elem; ++i)
    {
        if (mBinary(i) == 1)
            mImage.at(i) = '#';
        else if (mBinary(i) == -1)
            mImage.at(i) = '.';
        else
            throw std::logic_error("Неверное значение двоичного вектора!");
    }
}

std::ostream& operator<<(std::ostream& stream, const Symbol& symbol)
{
    stream << symbol.mName << " : " << symbol.mImage;
    return stream;
}

std::istream& operator>>(std::istream& stream, Symbol& symbol)
{
    std::string rawString;
    std::getline(stream, rawString);

    rawString.erase(std::remove_if(rawString.begin(), rawString.end(), isspace), rawString.end());

    std::istringstream sStream(rawString);

    std::getline(sStream, symbol.mName, ':');
    std::getline(sStream, symbol.mImage);

    symbol.mBinary = Symbol::convertImageToBinary(symbol.mImage);

    return stream;
}


NeuralNetwork::NeuralNetwork()
{
}


NeuralNetwork::~NeuralNetwork()
{
}

void NeuralNetwork::train()
{
    size_t imageWidth = getSymbolWidth();

    mWeights = arma::mat(imageWidth, imageWidth, arma::fill::zeros);

    for (const Symbol& symbol : mAlphavite)
    {
        arma::Row<int> img = symbol.mBinary;

        arma::mat first = mWeights * img.t() - img.t();
        arma::mat second = first.t();
        arma::mat third = img * img.t() - (img * mWeights) * img.t();

        mWeights += first * second * (1 / third(0, 0));
    }
}

bool NeuralNetwork::recognize(Symbol symbol)
{
    std::vector<arma::Row<int>> previous;
    bool relaxed = false;
    int iteration = 0;

    while (!relaxed)
    {
        int neuron = getRandomNeuron();

        arma::Row<double> tmp = symbol.mBinary * mWeights.col(neuron);
        double weightedSum = tmp(0, 0);

        // activation function
        if (weightedSum > 0)
            symbol.mBinary(neuron) = 1;
        else if (weightedSum < 0)
            symbol.mBinary(neuron) = -1;

        if (previous.size() >= 100)
        {
            for (auto it = previous.end() - 100; it != previous.end(); ++it)
            {
                if (!areVectorsEqual(*it, symbol.mBinary))
                {
                    relaxed = false;
                    break;
                }
                relaxed = true;
            }
        }

        iteration++;
        previous.push_back(symbol.mBinary);

        symbol.updateImage();

        std::cout << "Итерация " << iteration << ": " << symbol.mImage << std::endl;
    }

    if (relaxed)
        std::cout << std::endl << "Сеть достигла релаксации." << std::endl;

    for (const Symbol& sym : mAlphavite)
    {
        if (sym == symbol)
        {
            std::cout << "Введенный образ соответствует образу: " << std::endl;
            std::cout << sym << std::endl;
        }
    }

    return true;
}


NeuralNetwork::Alphavite NeuralNetwork::getAlphavite() const
{
    return mAlphavite;
}

size_t NeuralNetwork::getAlphaviteSize() const
{
    return mAlphavite.size();
}

size_t NeuralNetwork::getSymbolWidth() const
{
    // TODO: ensure alphavite is not empty
    return mAlphavite.at(0).getWidth();
}

void NeuralNetwork::addSymbol(const Symbol& symbol)
{
    mAlphavite.push_back(symbol);
}

void NeuralNetwork::loadAlphavite(const std::string& fileName)
{
    std::ifstream file;
    file.open(fileName, std::ifstream::in);

    if (!file.is_open())
        return;

    while (!file.eof())
    {
        Symbol symbol;
        file >> symbol;
        mAlphavite.push_back(symbol);
    }

    file.close();
}

void NeuralNetwork::generateAlphavite(const std::string& outputFile, size_t imageSize, size_t amount)
{
    auto randomChar = []() -> char
    {
        int r = rand() % 2;
        return r ? '#' : '.';
    };

    std::ofstream file;
    file.open(outputFile, std::ofstream::out);

    Alphavite alphavite;

    char letter = 'a';
    for (int i = 0; i < amount; ++i)
    {
        std::string symbolName;
        symbolName.push_back(letter);

        std::string symbolImage(imageSize, ' ');
        Symbol symbol(symbolName);

        do
        {
            std::generate(symbolImage.begin(), symbolImage.end(), randomChar);

        } while (std::find(alphavite.begin(), alphavite.end(), symbol) != alphavite.end());

        symbol.setImage(symbolImage);
        file << symbol;

        if (i < amount - 1)
            file << std::endl;

        ++letter;
    }

    file.close();
}

int NeuralNetwork::getRandomNeuron() const
{
    return rand() % getSymbolWidth();
}

bool NeuralNetwork::areVectorsEqual(const arma::Row<int>& first, const arma::Row<int>& second) const
{
    return all(first == second);
}
