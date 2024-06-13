#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>

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

std::string toLowercase(const std::string& str) {
    std::string lowercaseStr = str;
    for (char& c : lowercaseStr) {
        c = std::tolower(c);
    }
    return lowercaseStr;
}

std::string toUppercase(const std::string& str) {
    std::string uppercaseStr = str;
    for (char& c : uppercaseStr) {
        c = std::toupper(c);
    }
    return uppercaseStr;
}

void defineVisitor(std::ostream& outFile, std::string baseName, std::vector<std::string> types) {
    outFile << "    template <typename T>" << std::endl;
    outFile << "    class Visitor {" << std::endl;
    outFile << "    public:" << std::endl;
    for (int i = 0; i < types.size(); i++) {
        std::string typeName = trim(splitString(types[i], ':')[0]); //first split on ':', get the first element of split, which is the typeName, then trim it
        std::string baseLower = baseName;
        for (int i = 0; i < baseLower.size(); i++) {
            baseLower[i] = tolower(baseLower[i]);
        }
        outFile << "        virtual T visit" << typeName << baseName << " (" << typeName << "<R>* " << baseLower << ") = 0;" << std::endl;
    }
    outFile << "        virtual ~Visitor() = default;" << std::endl;
    outFile << "    };" << std::endl;
    outFile << std::endl;
}

void defineType(std::ofstream& outFile, std::string baseName, std::string className, std::string fields) {
    outFile << "template <typename R>" << std::endl;
    outFile << "class " << className << " : public " << baseName << "<R> {" << std::endl;
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
    outFile << "    R accept(typename " << baseName << "<R>::template Visitor<R>* visitor) override {" << std::endl;
    outFile << "        return visitor->visit" << className << baseName << "(this);" << std::endl;
    outFile << "    }" << std::endl;
    outFile << "};" << std::endl;
    outFile << std::endl;

}

void defineAst(std::string outputDir, std::string baseName, std::vector<std::string> types) {
    std::string path = outputDir + "/" + toLowercase(baseName) + ".h"; //Change accordingly
    std::ofstream outFile(path);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }
    outFile << "#ifndef " << toUppercase(baseName) << "_H" << std::endl; //Change accordingly
    outFile << "#define " << toUppercase(baseName) << "_H" << std::endl; //Change accordingly
    outFile << "#include <vector>" << std::endl;
    outFile << "#include <string>" << std::endl;
    outFile << "#include <any>" << std::endl;
    outFile << "#include <memory>" << std::endl;
    outFile << "#include \"token.h\"" << std::endl;
    outFile << "using namespace std;" << std::endl;
    outFile << std::endl;

     
    for (std::string type: types ) {
        std::vector<std::string> processed = splitString(type, ':');
        std::string className = trim(processed[0]);
        outFile << "template <typename R> class " << className << ";" << std::endl;
    }

    outFile << std::endl;

    // writing base class
    outFile << "template <typename R>" << std::endl;
    outFile << "class " << baseName << "{" << std::endl;
    outFile << "public:" << std::endl;
    defineVisitor(outFile, baseName, types);
    outFile << "    virtual R accept(Visitor<R>* visitor) = 0;" << std::endl;
    outFile << "    virtual ~" << baseName << "() = default;" << std::endl;
    outFile << "};" << std::endl;
    outFile << std::endl;

    // writing child classes (assign, binary etc.)
    for (std::string type: types ) {
        std::vector<std::string> processed = splitString(type, ':');
        std::string className = trim(processed[0]);
        std::string fields = trim(processed[1]);
        defineType(outFile, baseName, className, fields);
    }
    outFile << "#endif" << std::endl;
    outFile.close();
}

int main() {
    std::string outputDir = "..";
    
    std::vector<std::string> expressionTypes = {
      "Assign   : shared_ptr<Token> name, shared_ptr<Expr<R>> value",
      "Binary   : shared_ptr<Expr<R>> left, shared_ptr<Token> operation, shared_ptr<Expr<R>> right",
      "Call     : shared_ptr<Expr<R>> callee, shared_ptr<Token> paren, vector<shared_ptr<Expr<R>>> arguments",
      "Get      : shared_ptr<Expr<R>> object, shared_ptr<Token> name",
      "Grouping : shared_ptr<Expr<R>> expression",
      "Literal  : any value",
      "Logical  : shared_ptr<Expr<R>> left, shared_ptr<Token> operation, shared_ptr<Expr<R>> right", 
      "Set      : shared_ptr<Expr<R>> object, shared_ptr<Token> name, shared_ptr<Expr<R>> value",
      "This     : shared_ptr<Token> keyword",
      "Unary    : shared_ptr<Token> operation, shared_ptr<Expr<R>> right",
      "Variable : shared_ptr<Token> name"};
    defineAst(outputDir, "Expr", expressionTypes);
    
    std::vector<std::string> statementTypes = {
    "Block      : vector<shared_ptr<Stmt<R>>> statements",
    "Class      : shared_ptr<Token> name, vector<shared_ptr<Stmt<R>>> methods",
    "Expression : shared_ptr<Expr<R>> expression", 
    "Function   : shared_ptr<Token> name, vector<shared_ptr<Token>> params, vector<shared_ptr<Stmt<R>>> body",
    "If         : shared_ptr<Expr<R>> condition, shared_ptr<Stmt<R>> thenBranch, shared_ptr<Stmt<R>> elseBranch",
    "Print      : shared_ptr<Expr<R>> expression",
    "Return     : shared_ptr<Token> keyword, shared_ptr<Expr<R>> value",
    "Var        : shared_ptr<Token> name, shared_ptr<Expr<R>> initializer",
    "While      : shared_ptr<Expr<R>> condition, shared_ptr<Stmt<R>> body"};
    defineAst(outputDir, "Stmt", statementTypes);
}
