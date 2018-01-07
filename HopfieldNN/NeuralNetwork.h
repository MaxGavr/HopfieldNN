#pragma once

#include <armadillo>

class Symbol
{
public:
    using BinaryImage = arma::Row<int>;

    Symbol(std::string name = "", std::string image = "");

    bool loadFromFile(std::string fileName);

    bool operator==(const Symbol& symbol) const;
    friend std::ostream& operator<<(std::ostream& stream, const Symbol& symbol);
    friend std::istream& operator>>(std::istream& stream, Symbol& symbol);

    int getWidth() const;

    void setImage(std::string image);
    void setTwoDimensional(bool twoDimensional);
    void updateImage();

    static BinaryImage convertImageToBinary(std::string image);

    std::string mName;
    std::string mImage;
    BinaryImage mBinary;

    bool mTwoDimensional;
};

class NeuralNetwork
{
public:
    using Alphavite = std::vector<Symbol>;
    using Matrix = arma::mat;

    NeuralNetwork();
    ~NeuralNetwork();

    int getRelaxationPeriod() const;
    void setRelaxationPeriod(size_t period);

    bool getShowNeuronsOutput() const;
    void showNeuronsOutput(bool show = true);

    bool getShowWeightMatrix() const;
    void showWeightMatrix(bool show = true);

    bool getTwoDimensionalImages() const;
    void setTwoDimensionalImages(bool twoDimensional = true);

    void train();
    void recognize(Symbol symbol);

    Alphavite getAlphavite() const;
    size_t getAlphaviteSize() const;

    size_t getSymbolWidth() const;

    void addSymbol(const Symbol& symbol);
    void loadAlphavite(const std::string& fileName);
    static void generateAlphavite(const std::string& outputFile, size_t imageSize, size_t amount);

    arma::mat mWeights;

private:
    int getRandomNeuron() const;
    bool areImagesEqual(const Symbol::BinaryImage& first, const Symbol::BinaryImage& second) const;

    Alphavite mAlphavite;
    size_t mRelaxationPeriod;
    bool mShowNeuronsOutput;
    bool mShowWeightMatrix;
    bool mTwoDimensionalImages;
};

std::ostream& operator<<(std::ostream& stream, const Symbol& symbol);
std::istream& operator>>(std::istream& stream, Symbol& symbol);

