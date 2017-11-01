#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

int ** createNewTable(const unsigned int size){
    int** table = new int*[size];
    for(int i = 0; i < size; ++i)
        table[i] = new int[size];
    return table;
}

void saveTableToFile(const unsigned int size, int** table, const string &fileName) {
    ofstream file;
    file.open (fileName);
    for(unsigned int i = 0; i < size; i++) {
        for(unsigned int j = 0; j < size; j++) {
            if(j != 0)
                file << ",";
            file << table[j][i];
        }
        file << "\n";
    }
    file.close();
}

void generateFile(const string &fileName, const unsigned int size) {
    int **table = createNewTable(size);
    for(unsigned int i = 0; i < size; i++)
        for(unsigned int j = 0; j < size; j++)
            table[i][j] = rand() % 9 + 0;
    saveTableToFile(size,table,fileName);
}

int ** getTableFromFile(const unsigned int size, const string &fileName){
    string line;
    ifstream file (fileName);
    if (file.is_open()) {
        int** table = createNewTable(size);
        int k = 0;
        while ( getline (file, line) ){
            vector<int> vector;
            stringstream ss(line);
            int i;
            while (ss >> i) {
                vector.push_back(i);
                if (ss.peek() == ',')
                    ss.ignore();
            }
            for (i=0; i< vector.size(); i++) {
                table[i][k] = vector.at(static_cast<unsigned int>(i));
            }
            k++;
        }
        file.close();
        return table;
    }
    else{
        cout << "Unable to open file: " << fileName << endl;
        return nullptr;
    }
}

const unsigned int getSize(const string &fileName){
    string line;
    unsigned int size = 0;
    ifstream file (fileName);
    if (file.is_open()) {
        while ( getline (file, line) ){
            vector<int> vector;
            int i;
            stringstream ss(line);
            while (ss >> i) {
                vector.push_back(i);
                if (ss.peek() == ',')
                    ss.ignore();
            }
            if(size == 0)
                size = vector.size();
            else if(size != vector.size()) {
                cout << "Wrong data in file: " << fileName << endl;
                file.close();
                return 0;
            }
        }
    } else{
        cout << "Unable to open file: " << fileName << endl;
        return 0;
    }
    return size;
}

int ** codeTable1(const unsigned int size, int** table){
    int **newTable = createNewTable(size);
    unsigned int counter = 0;
    for(unsigned int i = 0; i < size; i++){
        if(counter == 0)
            for(unsigned int j = 0; j < size; j++){
                newTable[j][i] = table[i][counter];
                counter++;
            }
        else{
            counter = size - 1;
            for(unsigned int j = 0; j < size; j++){
                newTable[j][i] = table[i][counter];
                counter--;
            }
            counter = 0;
        }
    }
    return newTable;
}

bool checkCoord(int i, int j, const unsigned int size) {
    return i >= 0 && j >= 0 && i < size && j < size;
}

int ** codeTable2(const unsigned int size, int** table){
    int* tableHelper = new int[size * 2];
    int **newTable = createNewTable(size);
    unsigned int indexX = 0;
    unsigned int indexY = 0;
    unsigned int counter = 0;
    bool check = false;
    for (unsigned int k = 0; k < size; k++) {
        tableHelper[counter] = table[k][0];
        counter++;
        int i = k - 1;
        int j = 1;
        while (checkCoord(i, j, size)) {
            tableHelper[counter] = table[i][j];
            counter++;
            i--;
            j++;
        }
        if(check)
            for(unsigned int p = 0; p < counter; p++){
                if(indexX > size - 1){
                    indexX = 0;
                    indexY++;
                }
                newTable[indexX][indexY] = tableHelper[p];
                indexX ++;
            }
        else
            for(int p = counter - 1; p >= 0 ; p--){
                if(indexX > size - 1){
                    indexX = 0;
                    indexY++;
                }
                newTable[indexX][indexY] = tableHelper[p];
                indexX ++;
            }
        counter = 0;
        check = !check;
    }
    counter = 0;
    check = false;
    for (unsigned int k = 1; k < size; k++) {
        tableHelper[counter] = table[size-1][k];
        counter++;
        int i = size - 2;
        int j = k + 1;
        while (checkCoord(i, j, size)) {
            tableHelper[counter] = table[i][j];
            counter++;
            i--;
            j++;
        }
        if(check)
            for(unsigned int p = 0; p < counter; p++){
                if(indexX > size - 1){
                    indexX = 0;
                    indexY++;
                }
                newTable[indexX][indexY] = tableHelper[p];
                indexX ++;
            }
        else
            for(int p = counter - 1; p >= 0 ; p--){
                if(indexX > size - 1){
                    indexX = 0;
                    indexY++;
                }
                newTable[indexX][indexY] = tableHelper[p];
                indexX ++;
            }
        counter = 0;
        check = !check;
    }
    return newTable;
}

int ** codeTable3(const unsigned int size, int** table){
    int **newTable = createNewTable(size);
    unsigned int counter = 0;
    for(unsigned int i = 0; i < size; i++){
        if(counter == 0)
            for(unsigned int j = 0; j < size; j++){
                newTable[i][j] = table[i][counter];
                counter++;
            }
        else{
            counter = size - 1;
            for(unsigned int j = 0; j < size; j++){
                newTable[i][j] = table[i][counter];
                counter--;
            }
            counter = 0;
        }
    }
    return newTable;
}

bool executeMatrix(const string &fileName) {
    const unsigned int size = getSize(fileName + ".txt");
    if(size == 0){
        cout << "Problem with reading file: " << fileName << endl;
        return false;
    }
    int **table = getTableFromFile(size,fileName + ".txt");
    if(table == nullptr)
        return false;
    saveTableToFile(size, codeTable1(size,table), fileName + "Encrypted1.txt");
    saveTableToFile(size, codeTable2(size,table), fileName + "Encrypted2.txt");
    saveTableToFile(size, codeTable3(size,table), fileName + "Encrypted3.txt");
    return true;
}

int main() {
    //constants
    string fileNames[] = {"matrix1", "matrix2", "matrix3"};
    const unsigned int sizes[] = {5, 4, 10};
    bool filesCheck = true;

    srand (static_cast<unsigned int>(time(nullptr)));
    unsigned long number = (end(sizes) - begin(sizes));
    if(number != end(fileNames) - begin(fileNames)){
        cout << "Internal problem with parameters" << endl;
        return -1;
    }
    for(unsigned int i = 0; i < number; i++){
        string fileName = fileNames[i];
        if(filesCheck)
            generateFile(fileName + ".txt", sizes[i]);
        if(executeMatrix(fileName))
            cout << "Encryption of file: " + fileName + " SUCCESS" << endl;
        else
            cout << "Encryption of file: " + fileName + " FAIL" << endl;
    }
    return 0;
}
