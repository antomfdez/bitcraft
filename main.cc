#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <map>
#include <sstream>
#include <iomanip>
#include <Eigen/Dense>

using namespace Eigen;

// Token Types
enum class TokenType {
    Number, Identifier, Plus, Minus, Star, Slash, Equals, Semicolon, LParen, RParen, Comma, Print, End, Unknown, String
};

// Token Structure
struct Token {
    TokenType type;
    std::string value;
};

// Lexer
class Lexer {
public:
    Lexer(const std::string& src) : src(src), index(0) {}

    Token nextToken() {
        while (index < src.size() && std::isspace(src[index])) {
            ++index;
        }

        if (index >= src.size()) return {TokenType::End, ""};

        char current = src[index];

        if (current == '/') {
            if (src[index + 1] == '/') {
                return parseComment();
            }
        }

        if (current == '"') {
            return parseString();
        }
        if (std::isdigit(current) || (current == '-' && std::isdigit(src[index + 1]))) {
            return parseNumber();
        }
        if (std::isalpha(current)) {
            return parseKeywordOrIdentifier();
        }
        if (current == '+') { ++index; return {TokenType::Plus, "+"}; }
        if (current == '-') { ++index; return {TokenType::Minus, "-"}; }
        if (current == '*') { ++index; return {TokenType::Star, "*"}; }
        if (current == '/') { ++index; return {TokenType::Slash, "/"}; }
        if (current == '=') { ++index; return {TokenType::Equals, "="}; }
        if (current == ';') { ++index; return {TokenType::Semicolon, ";"}; }
        if (current == '(') { ++index; return {TokenType::LParen, "("}; }
        if (current == ')') { ++index; return {TokenType::RParen, ")"}; }
        if (current == ',') { ++index; return {TokenType::Comma, ","}; }
        
        return {TokenType::Unknown, std::string(1, current)};
    }

private:
    std::string src;
    size_t index;

    Token parseNumber() {
        size_t start = index;
        bool hasDecimal = false;
        if (src[index] == '-') ++index; // Handle negative numbers
        while (index < src.size() && (std::isdigit(src[index]) || src[index] == '.')) {
            if (src[index] == '.') {
                if (hasDecimal) break; // Handle multiple decimal points
                hasDecimal = true;
            }
            ++index;
        }
        return {TokenType::Number, src.substr(start, index - start)};
    }

    Token parseString() {
        size_t start = ++index; // Skip the opening quote
        while (index < src.size() && src[index] != '"') {
            ++index;
        }
        if (index < src.size()) {
            ++index; // Skip the closing quote
        }
        return {TokenType::String, src.substr(start, index - start - 1)};
    }

    Token parseKeywordOrIdentifier() {
        size_t start = index;
        while (index < src.size() && (std::isalnum(src[index]) || src[index] == '_')) {
            ++index;
        }
        std::string word = src.substr(start, index - start);
        if (word == "print") {
            return {TokenType::Print, word};
        }
        return {TokenType::Identifier, word};
    }

    Token parseComment() {
        while (index < src.size() && src[index] != '\n') {
            ++index;
        }
        return nextToken(); // Skip to the next token after the comment
    }
};

// Parser and Evaluator
class Parser {
public:
    Parser(Lexer& lexer) : lexer(lexer), currentToken(lexer.nextToken()) {}

    void parse() {
        while (currentToken.type != TokenType::End) {
            if (currentToken.type == TokenType::Print) {
                eat(TokenType::Print);
                eat(TokenType::LParen);
                std::string output = expression();
                std::cout << output << std::endl;
                eat(TokenType::RParen);
                eat(TokenType::Semicolon);
            } else if (currentToken.type == TokenType::Identifier) {
                std::string varName = currentToken.value;
                eat(TokenType::Identifier);
                eat(TokenType::Equals);
                double value = expressionAsDouble();
                variables[varName] = value;
                eat(TokenType::Semicolon);
            } else {
                std::cerr << "Syntax Error: Unexpected token " << currentToken.value << "\n";
                return;
            }
        }
    }

private:
    Lexer& lexer;
    Token currentToken;
    std::map<std::string, double> variables;

    void eat(TokenType type) {
        if (currentToken.type == type) {
            currentToken = lexer.nextToken();
        } else {
            std::cerr << "Syntax Error: Expected " << static_cast<int>(type) << ", got " << static_cast<int>(currentToken.type) << "\n";
            exit(1);
        }
    }

    std::string formatDouble(double value) {
        std::ostringstream oss;
        oss << std::fixed << value;
        std::string str = oss.str();

        // Remove trailing zeros
        auto pos = str.find_last_not_of('0');
        if (str[pos] == '.') pos--;
        str.erase(pos + 1);
        
        return str;
    }

    std::string expression() {
        std::ostringstream resultStream;

        while (currentToken.type != TokenType::End && currentToken.type != TokenType::Semicolon && currentToken.type != TokenType::RParen) {
            if (currentToken.type == TokenType::String) {
                resultStream << currentToken.value;
                eat(TokenType::String);
            } else if (currentToken.type == TokenType::Identifier) {
                double value = variables[currentToken.value];
                resultStream << formatDouble(value); // Format dynamically
                eat(TokenType::Identifier);
            } else if (currentToken.type == TokenType::Number) {
                double value = std::stod(currentToken.value);
                resultStream << formatDouble(value); // Format dynamically
                eat(TokenType::Number);
            }
            
            // Skip commas and other separators without adding them
            if (currentToken.type == TokenType::Comma) {
                eat(TokenType::Comma);
            }
        }

        return resultStream.str();
    }

    double expressionAsDouble() {
        double result = termAsDouble();
        while (currentToken.type == TokenType::Plus || currentToken.type == TokenType::Minus) {
            if (currentToken.type == TokenType::Plus) {
                eat(TokenType::Plus);
                result += termAsDouble(); // Add numbers
            } else if (currentToken.type == TokenType::Minus) {
                eat(TokenType::Minus);
                result -= termAsDouble(); // Subtract numbers
            }
        }
        return result;
    }

    double termAsDouble() {
        double result = factor();
        while (currentToken.type == TokenType::Star || currentToken.type == TokenType::Slash) {
            if (currentToken.type == TokenType::Star) {
                eat(TokenType::Star);
                result *= factor(); // Multiply numbers
            } else if (currentToken.type == TokenType::Slash) {
                eat(TokenType::Slash);
                result /= factor(); // Divide numbers
            }
        }
        return result;
    }

    double factor() {
        if (currentToken.type == TokenType::Number) {
            double value = std::stod(currentToken.value);
            eat(TokenType::Number);
            return value;
        } else if (currentToken.type == TokenType::Identifier) {
            std::string varName = currentToken.value;
            eat(TokenType::Identifier);
            if (variables.find(varName) != variables.end()) {
                return variables[varName];
            } else {
                std::cerr << "Error: Undefined variable " << varName << "\n";
                exit(1);
            }
        } else if (currentToken.type == TokenType::LParen) {
            eat(TokenType::LParen);
            double result = expressionAsDouble();
            eat(TokenType::RParen);
            return result;
        } else {
            std::cerr << "Syntax Error: Expected number, identifier, or expression\n";
            exit(1);
        }
    }
};

// File Reading
std::string readSourceFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        exit(1);
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

// Main
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::string sourceCode = readSourceFile(filename);

    Lexer lexer(sourceCode);
    Parser parser(lexer);
    parser.parse();

    return 0;
}

