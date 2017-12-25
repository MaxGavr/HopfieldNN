#pragma once

#include <armadillo>

class Symbol
{
public:
    Symbol(std::string name = "", std::string image = "");

    bool operator==(const Symbol& symbol) const;

    int getWidth() const;

    void setImage(std::string image);

    std::string mName;
    std::string mImage;
    arma::Row<int> mBinary;

    static arma::Row<int> convertImageToBinary(std::string image);

    friend std::ostream& operator<<(std::ostream& stream, const Symbol& symbol);
    friend std::istream& operator>>(std::istream& stream, Symbol& symbol);
};

class NeuralNetwork
{
public:

    using Alphavite = std::vector<Symbol>;

    NeuralNetwork();
    ~NeuralNetwork();

    Alphavite getAlphavite() const;

    void addSymbol(const Symbol& symbol);
    void loadAlphavite(const std::string& fileName);
    static void generateAlphavite(const std::string& outputFile, size_t imageSize, size_t amount);

private:
    Alphavite mAlphavite;
};

std::ostream& operator<<(std::ostream& stream, const Symbol& symbol);
std::istream& operator>>(std::istream& stream, Symbol& symbol);

