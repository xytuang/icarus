#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

std::string trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    // Find the first non-whitespace character
    while (start < end && std::isspace(str[start])) {
        ++start;
    }

    // Find the last non-whitespace character
    while (end > start && std::isspace(str[end - 1])) {
        --end;
    }

    // Return the trimmed substring
    return str.substr(start, end - start);
}

std::vector<std::string> splitString(const std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while(std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


void defineType(std::ofstream& outFile, std::string baseName, std::string className, std::string fields) {
    outFile << "class " << className << " : public " << baseName << " {" << std::endl;
    outFile << "public:" << std::endl;
    std::vector<std::string> fieldList = splitString(fields, ',');
    for (int i = 0; i < fieldList.size(); i++) {
        fieldList[i] = trim(fieldList[i]);
        outFile << "    " << fieldList[i] << ";" << std::endl;
    }

    outFile << "    " << className << "(" << fields << ") {" << std::endl;
    for (int i = 0; i < fieldList.size(); i++) {
        std::vector<std::string> curr = splitString(fieldList[i], ' ');
        std::string name = curr[1];
        outFile << "        this->" << name << "=" << name << ";" << std::endl;   
    }
    outFile << "    }" << std::endl;
    outFile << "};" << std::endl;
    outFile << std::endl;

}

void defineAst(std::string outputDir, std::string baseName, std::vector<std::string> types) {
    std::string path = outputDir + "/expr.cpp";
    std::ofstream outFile(path);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    outFile << "#include <vector>" << std::endl;
    outFile << std::endl;
    outFile << "class " << baseName << "{" << std::endl;

    outFile << "};" << std::endl;
    
    for (std::string type: types ) {
        std::vector<std::string> processed = splitString(type, ':');
        std::string className = trim(processed[0]);
        std::string fields = trim(processed[1]);
        defineType(outFile, baseName, className, fields);
    }
    outFile.close();

}

int main() {
    std::string outputDir = "..";
    std::vector<std::string> types = {"Binary   : Expr left, Token operator, Expr right",
      "Grouping : Expr expression",
      "Literal  : Object value",
      "Unary    : Token operator, Expr right"};
    defineAst(outputDir, "Expr", types);
}
