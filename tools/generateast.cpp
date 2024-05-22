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


void defineVisitor(std::ostream& outFile, std::string baseName, std::vector<std::string> types) {
    outFile << "    class Visitor {" << std::endl;
    outFile << "    public:" << std::endl;
    for (int i = 0; i < types.size(); i++) {
        std::string typeName = trim(splitString(types[i], ':')[0]); //first split on ':', get the first element of split, which is the typeName, then trim it
        std::string baseLower = baseName;
        for (int i = 0; i < baseLower.size(); i++) {
            baseLower[i] = tolower(baseLower[i]);
        }
        outFile << "        virtual string visit" << typeName << baseName << " (" << typeName << "* " << baseLower << ") = 0;" << std::endl;
    }
    outFile << "        virtual ~Visitor() = default;" << std::endl;
    outFile << "    };" << std::endl;
    outFile << std::endl;
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
    outFile << "    string accept(Visitor& visitor) {" << std::endl;
    outFile << "        return visitor.visit" << className << baseName << "(this);" << std::endl;
    outFile << "    }" << std::endl;
    outFile << "};" << std::endl;
    outFile << std::endl;

}

void defineAst(std::string outputDir, std::string baseName, std::vector<std::string> types) {
    std::string path = outputDir + "/expr.h";
    std::ofstream outFile(path);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    outFile << "#include <vector>" << std::endl;
    outFile << "#include <string>" << std::endl;
    outFile << "#include \"token.h\"" << std::endl;
    outFile << "using namespace std;" << std::endl;
    outFile << std::endl;

     
    for (std::string type: types ) {
        std::vector<std::string> processed = splitString(type, ':');
        std::string className = trim(processed[0]);
        outFile << "class " << className << ";" << std::endl;
    }

    outFile << std::endl;

    // writing Expr class
    outFile << "class " << baseName << "{" << std::endl;
    outFile << "public:" << std::endl;
    defineVisitor(outFile, baseName, types);
    //outFile << "    string accept(Visitor& visitor) {return \"\";}" << std::endl;
    outFile << "};" << std::endl;
    outFile << std::endl;

    // writing child classes (assign, binary etc.)
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
    std::vector<std::string> types = {"Binary   : Expr left, Token* operation, Expr right",
      "Grouping : Expr expression",
      "Literal  : string value",
      "Unary    : Token* operation, Expr right"};
    defineAst(outputDir, "Expr", types);
}
