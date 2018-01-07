#include "NeuralNetwork.h"

#include <math.h>


Symbol::Symbol(std::string name, std::string image)
    : mName(name),
      mImage(image),
      mBinary(convertImageToBinary(image)),
      mTwoDimensional(false)
{
}

bool Symbol::loadFromFile(std::string fileName)
{
    std::ifstream file;
    file.open(fileName, std::ifstream::in);

    if (!file.is_open())
        return false;

    Symbol symbol;
    symbol.setTwoDimensional(mTwoDimensional);
    file >> symbol;

    *this = symbol;

    file.close();
    return true;
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

void Symbol::setTwoDimensional(bool twoDimensional)
{
    mTwoDimensional = true;
}

Symbol::BinaryImage Symbol::convertImageToBinary(std::string image)
{
    // remove spaces
    image.erase(std::remove_if(image.begin(), image.end(), isspace), image.end());

    std::vector<int> tmpVector(image.size());

    auto toInt = [](char c) -> int
    {
        if (c == '.')
            return -1;
        else if (c == '#')
            return 1;
        else
            throw std::logic_error("Некорректный символ образа!");
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
    if (symbol.mTwoDimensional)
    {
        stream << symbol.mName << " : " << std::endl;

        const int rows = sqrt(symbol.getWidth());

        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < rows; ++j)
                stream << symbol.mImage.at(i * rows + j);
            stream << std::endl;
        }
    }
    else
        stream << symbol.mName << " : " << symbol.mImage;

    return stream;
}

std::istream& operator>>(std::istream& stream, Symbol& symbol)
{
    std::string rawString;
    std::getline(stream, rawString, symbol.mTwoDimensional ? '~' : '\n');

    // remove spaces
    rawString.erase(std::remove_if(rawString.begin(), rawString.end(), isspace), rawString.end());

    // read symbol's name and image
    std::istringstream sStream(rawString);
    std::getline(sStream, symbol.mName, ':');
    std::getline(sStream, symbol.mImage);

    // convert symbol's image to binary form
    symbol.mBinary = Symbol::convertImageToBinary(symbol.mImage);

    return stream;
}


NeuralNetwork::NeuralNetwork()
    : mRelaxationPeriod(30),
      mShowNeuronsOutput(true),
      mShowWeightMatrix(true),
      mTwoDimensionalImages(false)
{
}


NeuralNetwork::~NeuralNetwork()
{
}

int NeuralNetwork::getRelaxationPeriod() const
{
    return mRelaxationPeriod;
}

void NeuralNetwork::setRelaxationPeriod(size_t period)
{
    mRelaxationPeriod = period;
}

bool NeuralNetwork::getShowNeuronsOutput() const
{
    return mShowNeuronsOutput;
}

void NeuralNetwork::showNeuronsOutput(bool show)
{
    mShowNeuronsOutput = show;
}

bool NeuralNetwork::getShowWeightMatrix() const
{
    return mShowWeightMatrix;
}

void NeuralNetwork::showWeightMatrix(bool show)
{
    mShowWeightMatrix = show;
}

bool NeuralNetwork::getTwoDimensionalImages() const
{
    return mTwoDimensionalImages;
}

void NeuralNetwork::setTwoDimensionalImages(bool twoDimensional)
{
    mTwoDimensionalImages = twoDimensional;
}

void NeuralNetwork::train()
{
    const size_t imageWidth = getSymbolWidth();

    // initialize zero-filled weight matrix
    mWeights = Matrix(imageWidth, imageWidth, arma::fill::zeros);

    // calculate weights using Delta-rule
    for (const Symbol& symbol : mAlphavite)
    {
        const Symbol::BinaryImage& img = symbol.mBinary;

        // first multiplier
        Matrix first = mWeights * img.t() - img.t();
        // second multiplier
        Matrix second = first.t();
        // divider, size 1x1
        Matrix third = img * img.t() - (img * mWeights) * img.t();

        mWeights += first * second * (1.0 / third(0, 0));
    }

    if (mShowWeightMatrix)
    {
        std::cout << "Матрица весов: " << std::endl;
        std::cout << mWeights << std::endl;
        system("pause");
    }
}

void NeuralNetwork::recognize(Symbol symbol)
{
    Symbol initialSymbol = symbol;

    std::vector<Symbol::BinaryImage> previous;
    bool relaxed = false;
    int iteration = 0;

    while (!relaxed)
    {
        const int neuron = getRandomNeuron();

        // size 1x1
        arma::Row<double> tmp = symbol.mBinary * mWeights.col(neuron);
        double weightedSum = tmp(0, 0);

        // activation function
        if (weightedSum > 0)
            symbol.mBinary(neuron) = 1;
        else if (weightedSum < 0)
            symbol.mBinary(neuron) = -1;

        // check if reached relaxation
        if (previous.size() >= mRelaxationPeriod)
        {
            for (auto it = previous.end() - mRelaxationPeriod; it != previous.end(); ++it)
            {
                const Symbol::BinaryImage& mPrevImage = *it;

                if (!areImagesEqual(mPrevImage, symbol.mBinary))
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
        if (mShowNeuronsOutput)
        {
            if (mTwoDimensionalImages)
                std::cout << "Итерация " << iteration << ":" << std::endl << symbol << std::endl;
            else
                std::cout << "Итерация " << iteration << ": " << symbol.mImage << std::endl;
        }
        else
            std::cout << "Итерация " << iteration << std::endl;
    }

    if (relaxed)
        std::cout << std::endl << "Сеть достигла состояния релаксации." << std::endl << std::endl;

    // check if symbol has been recognized
    bool recognized = false;
    for (const Symbol& sym : mAlphavite)
    {
        if (sym == symbol)
        {
            std::cout << "Введенный образ: " << std::endl
                << initialSymbol << std::endl
                << " соответствует образу : " << std::endl
                << sym << std::endl;
            recognized = true;
            break;
        }
    }

    if (!recognized)
        std::cout << "Сеть не смогла распознать образ." << std::endl;
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
    try
    {
        return mAlphavite.at(0).getWidth();
    }
    catch (std::out_of_range e)
    {
        return 0;
    }
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

    Alphavite().swap(mAlphavite);

    while (!file.eof())
    {
        Symbol symbol;
        symbol.setTwoDimensional(mTwoDimensionalImages);
        file >> symbol;
        file.ignore(1, '\n');
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

bool NeuralNetwork::areImagesEqual(const Symbol::BinaryImage& first, const Symbol::BinaryImage& second) const
{
    return all(first == second);
}
