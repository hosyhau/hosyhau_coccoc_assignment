#include <bits/stdc++.h>

using namespace std;

string OUTPUT_TEMP = "output_temp";

class Comparator
{
public:

    bool operator() (pair<string, int> pair1, pair<string, int> pair2)
    {
        return pair1.first > pair2.first;
    }
};


string ToString(int val)
{
    stringstream stream;
    stream << val;
    return stream.str();
}

void swap(string* a, string* b)
{
    string t = *a;
    *a = *b;
    *b = t;
}

int partition (string arr[], long left, long right)
{
    string pivot = arr[right];
    long l = left;
    long r = right - 1;
    while(true)
    {
        while(l <= r && arr[l] < pivot) l++;
        while(r >= l && arr[r] > pivot) r--;
        if (l >= r) break;
        swap(arr[l], arr[r]);
        l++;
        r--;
    }
    swap(arr[l], arr[right]);
    return l;
}

void quickSort(string arr[], long left, long right)
{
    if (left < right)
    {
        long pi = partition(arr, left, right);

        quickSort(arr, left, pi - 1);
        quickSort(arr, pi + 1, right);
    }
}

void sortAndWrite(string *values, int size, int numberOfChunks)
{
    quickSort(values, 0, size-1);
    string outputFileName = OUTPUT_TEMP + ToString(numberOfChunks) + ".txt";
    ofstream outputFile(outputFileName.c_str());
    for (int i = 0; i < size; i++)
    {
        if (i == size-1)
        {
            outputFile << values[i];
            break;
        }
        outputFile << values[i] << '\n';

    }
    outputFile.close();
}

bool isEmptyFile(string inputFileName)
{
    ifstream inFile(inputFileName);
    inFile.seekg(0, ios::end);
    if (inFile.tellg() == 0)
    {
        inFile.close();
        return true;
    }
    inFile.close();
    return false;
}

bool validateArguments(char ** args)
{
    string inputFileName = args[1];
    string outputFileName = args[2];
    string ramLimit = args[3];

    if (inputFileName.empty())
    {
        cout<<"input file name can not be not null or empty."<<endl;
        return false;
    }

    if (isEmptyFile(inputFileName))
    {
        cout<<"File can not be empty."<<endl;
        return false;
    }

    if (outputFileName.empty())
    {
        cout<<"output file name can not be not null or empty."<<endl;
        return false;
    }

    if (ramLimit.empty())
    {
        cout<<"ram limit can not be null or empty."<<endl;
        return false;
    }
    else
    {
        try
        {
            stringstream ss(ramLimit);
            long ramValue;
            ss>>ramValue;
            if (ramValue <= 0)
            {
                cout << "ram limit must be greater than 0."<<endl;
                return false;
            }
        }
        catch(exception& e)
        {
            cout<<"ram limit can not casted to long value."<<endl;
            return false;
        }
    }

    return true;
}

string mergeAllFiles(int counter, string outputFileName)
{
    priority_queue<pair<string, int>, vector<pair<string, int> >, Comparator> minHeap;
    ifstream* handles = new ifstream[counter];

    stringstream ss;
    string temp;
    for (int i = 1; i <= counter; i++)
    {
        string sortedInputFileName = OUTPUT_TEMP + ToString(i) + ".txt";
        handles[i - 1].open(sortedInputFileName.c_str());
        string firstValue;
        handles[i - 1] >> firstValue;

        pair<string, int> pairValue;
        pairValue.first = firstValue;
        pairValue.second = i-1;
        minHeap.push(pairValue);
    }
    ofstream outputFile(outputFileName.c_str());

    while (minHeap.size() > 0)
    {
        pair<string, int> minPair = minHeap.top();
        minHeap.pop();
        outputFile << minPair.first << '\n';
        string nextValue;
        flush(outputFile);
        if (handles[minPair.second] >> nextValue)
        {
            minHeap.push(pair<string, int>(nextValue, minPair.second));
        }
    }

    for (int i = 1; i <= counter; i++)
    {
        handles[i - 1].close();
    }
    outputFile.close();
    delete[] handles;

    return outputFileName;
}

long getSizeOfFile(string inFileName)
{
    ifstream inFile(inFileName, ios::in|ios::binary);
    inFile.seekg (0, inFile.end);
    long length = (long)inFile.tellg()-1;
    inFile.seekg (0, inFile.beg);
    inFile.close();
    return length;
}

string getEntireTextFile(string inputFileName)
{
    fstream file;
    file.open(inputFileName, ios::in| ios::binary);
    string input;
    file >> input;
    return input;
}

string getTextByPositionInFile(string inputFileName, long currentPosition, long length)
{
    ifstream inFile;
    string str = "";
    inFile.open(inputFileName, ios::in | ios::binary);
    char *buffer = new char [length];
    if (inFile.good())
    {
        inFile.seekg(currentPosition, ios::beg);
        inFile.read(buffer, length);
        str.assign(buffer, buffer + length);
    }
    inFile.close();
    return str;
}

long getLastPositionOfText(string text, string delimiter)
{
    long position = text.find_last_of(delimiter);
    return position;
}

string* splitByDelimiter(string stringToBeSplitted, string delimeter, long &sizeOfArray)
{
    vector<string> splittedString;
    int startIndex = 0;
    int  endIndex = 0;
    while( (endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size() )
    {

        string val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
        splittedString.push_back(val);
        startIndex = endIndex + delimeter.size();

    }
    if(startIndex < stringToBeSplitted.size())
    {
        string val = stringToBeSplitted.substr(startIndex);
        splittedString.push_back(val);
    }
    string *result = new string[splittedString.size()];
    copy(splittedString.begin(), splittedString.end(), result);
    sizeOfArray = splittedString.size();
    return result;
}

void processWithOneFile(string textValue, int numberOfChunks, string delimiter)
{
    long sizeOfArray = 0;
    string *inputValues = splitByDelimiter(textValue, delimiter, sizeOfArray);
    sortAndWrite(inputValues, sizeOfArray, numberOfChunks);
    delete[] inputValues;
}

void processWholeDataInFile(char **argv)
{
    string inputFileName = argv[1];
    string outputFileName = argv[2];
    stringstream ss(argv[3]);
    long sizeOfRam;
    ss>>sizeOfRam;
    long limit = sizeOfRam * 0.8;
    long sizeOfFile = getSizeOfFile(inputFileName);
    string delimiter = "\\n";

    string text;
    string temp;
    bool isReadDone = false;
    int numberOfChunksFile = 1;
    long currentPositon = 0;
    string result;
    if (sizeOfFile > limit)
    {
        while (currentPositon <= sizeOfFile-1 && isReadDone == false)
        {
            if (currentPositon == 0)
            {
                text = getTextByPositionInFile(inputFileName, 0, limit);
            }
            else if (limit + currentPositon > sizeOfFile)
            {
                text = getTextByPositionInFile(inputFileName, currentPositon, sizeOfFile - currentPositon + 1);
                isReadDone = true;
            }
            else
            {
                text = getTextByPositionInFile(inputFileName, currentPositon, limit);
            }

            if (isReadDone == false)
            {
                currentPositon = currentPositon + limit;
                long lengthOfText = text.size();
                string twoCharOfEndText = text.substr(lengthOfText-2, 2);
                string twoCharNext = getTextByPositionInFile(inputFileName, currentPositon, 2);

                if (twoCharNext[0] == 'n' && twoCharOfEndText[1] == '\\')
                {
                    currentPositon++;
                    text = text + twoCharNext[0];
                }
                else if (twoCharOfEndText != delimiter && twoCharNext != delimiter)
                {
                    long positionOfLastDelimter = getLastPositionOfText(text, delimiter)-1;
                    long sizeBeforeCut = text.size();
                    text = text.substr(0, positionOfLastDelimter);
                    long sizeAfterCut = text.size();
                    long numberOfCutText = sizeBeforeCut - sizeAfterCut;
                    currentPositon = currentPositon - numberOfCutText;
                }
            }

            processWithOneFile(text, numberOfChunksFile, delimiter);
            numberOfChunksFile++;
        }
    }
    else
    {
        string wholeText = getEntireTextFile(inputFileName);
        processWithOneFile(wholeText, numberOfChunksFile, delimiter);
    }

    if (numberOfChunksFile != 0)
        mergeAllFiles(numberOfChunksFile, outputFileName);
}


int main(int argc, char** argv)
{
    bool validate = validateArguments(argv);
    if (validate == false) return 0;

    processWholeDataInFile(argv);

    return 0;
}
