/* Copyright (C) Jacob Cohen - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Jacob Cohen <jcohen30@uic.edu> or <jacob@jacobcohen.info>
 */

#include <algorithm>
#include <chrono>
#include <fstream>
#include <map>
#include <random>
#include <string.h>
#include <string>
#include <vector>

#include "redBlackTree.cpp"
#include "timer/src/Timer.cpp"

using namespace std::chrono;

static void processInputLoop(FILE* inFile, RBT<int, char>& rbt)
{
    char buffer[300];
    char* input;

    input = fgets(buffer, 300, inFile);

    while (strcmp(input, "exit") != 0 && strcmp(input, "exit\n") != 0)
    {
        char cmdChar;

        cmdChar = (char)tolower(*input);

        char dummy;
        if (cmdChar == 'f' || cmdChar == 'g')
        {
            int key;
            if (sscanf(input, "%c %d", &dummy, &key) == 2)
            {
                printf("Found value: '%c' from key: %d\n", rbt.get(key), key);
            }
        }
        else if (cmdChar == 'p' || cmdChar == 'i')
        {
            int key;
            char val;
            if (sscanf(input, "%c %d %c", &dummy, &key, &val) == 3)
            {
                rbt.put(key, val);
            }
        }
        else if (cmdChar == 'd')
        {
            rbt.dump();
        }
        else if (cmdChar == 'r')
        {
            int key;
            if (sscanf(input, "%c %d", &dummy, &key) == 2)
            {
                rbt.remove(key);
            }
        }
        input = fgets(buffer, 300, inFile);
    }
}

static void printUsage()
{
    cout << "+-------------------------------------------------------------------------------------"
            "---+\n|                                                                               "
            "         |\n|  Red-Black BST Interactive Demo (jcohen30):                             "
            "               |\n|                                                                   "
            "                     |\n|  This RBT uses keys of type 'int' and values of type 'char' "
            "                           |\n|                                                       "
            "                                 |\n|  Try the commands below:                        "
            "                                       |\n|                                           "
            "                                             |\n|  'exit' --> quit the program        "
            "                                                   |\n|                               "
            "                                                         |\n|  'f <int>' OR           "
            "                                                               |\n|     'g <int>' --> "
            "returns and prints the char value associated with integer '<int>'    |\n|             "
            "                                                                           |\n|  'p "
            "<int> <char>' OR                                                                   "
            "|\n|    'i <int> <char>' --> puts a key-value-pair int-to-char in the tree            "
            "      |\n|                                                                            "
            "            |\n|  'r <int>' --> removes the key and its value from the tree           "
            "                  |\n|                                                                "
            "                        |\n|  'd' --> dumps a human-readable representation of the "
            "tree                             |\n|                                                 "
            "                                       "
            "|\n+----------------------------------------------------------------------------------"
            "------+"
         << endl;
}

static bool checkArgs(int argCount, char** mainArgs, const char* toFind)
{
    for (int i = 0; i < argCount; i++)
        if (strcmp(mainArgs[i], toFind) == 0) return true;
    return false;
}

// not used anymore, vals don't matter, using same one
static string randomString(size_t length)
{
    const string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    mt19937 generator(time(0));
    uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    string rString;

    for (size_t i = 0; i < length; ++i)
    {
        rString += CHARACTERS[distribution(generator)];
    }

    return rString;
}

static bool writeCSV(vector<pair<int, int>> data, string fileName)
{
    ofstream csvFile(fileName);

    csvFile << "n,microseconds\n";

    for (int i = 0; i < data.size(); i++)
        csvFile << data.at(i).first << "," << data.at(i).second << "\n";
}

static const unsigned int timesToRun = 5000, linearIncreaseVal = 2000;

void shuffle_array(int arr[], int n)
{
    unsigned seed = time(0);

    shuffle(arr, arr + n, default_random_engine(seed));
}

int* makeShuffledArray(const int& n)
{
    int* arr = new int[n];
    for (int i = 0; i < n; i++) arr[i] = i;
    shuffle_array(arr, n);
    return arr;
}

static void doNewTiming()
{
    srand(time(0));

    RBT<int, string> opsRBT;
    vector<pair<int, int>> putData;

    int* allKeysNew = makeShuffledArray(linearIncreaseVal * timesToRun);
    for (int n = 0; n < timesToRun; n++)
    {
        // int keys[linearIncreaseVal];
        /*
        string vals[linearIncreaseVal];

        for (int i = 0; i < linearIncreaseVal; i++)
        {
            vals[i] = randomString(8);
            // keys[i] = rand();
        }
        */

        Timer t;
        t.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            // putRBT.put(allKeysNew[n * linearIncreaseVal + i], vals[i]);
            opsRBT.put(allKeysNew[n * linearIncreaseVal + i], "string");
        }
        t.stop();

        putData.push_back(make_pair(n * linearIncreaseVal, t.getElapsedTimeInMicroSec()));
    }
    writeCSV(putData,
             "RBT_put_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");

    cout << "\n**put CSV DONE!**\n**beginning get and remove...**\n";

    shuffle_array(allKeysNew, linearIncreaseVal * timesToRun);
    vector<pair<int, int>> getData, remData;
    for (int n = 0; n < timesToRun; n++)
    {
        Timer t;
        t.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            opsRBT.get(allKeysNew[n * linearIncreaseVal + i]);
        }
        t.stop();

        getData.push_back(make_pair(linearIncreaseVal * timesToRun - n * linearIncreaseVal,
                                    t.getElapsedTimeInMicroSec()));
        Timer t2;
        t2.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            opsRBT.remove(allKeysNew[n * linearIncreaseVal + i]);
        }
        t2.stop();

        remData.push_back(make_pair(linearIncreaseVal * timesToRun - n * linearIncreaseVal, // this
                                    t2.getElapsedTimeInMicroSec()));
    }

    reverse(getData.begin(), getData.end());
    reverse(remData.begin(), remData.end());
    writeCSV(getData,
             "RBT_get_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");
    writeCSV(remData,
             "RBT_remove_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");

    // delete[] totalKeysToRem;
    delete[] allKeysNew;
}

// UNFINISHED AND USLESS IN CURRENT STATE, was meant to debug by testing std::map which uses a 100%
// proper RBT
static void doNewTimingSTL()
{
    srand(time(0));

    map<int, string> putRBT;
    vector<pair<int, int>> putData;

    int* allKeysNew = makeShuffledArray(linearIncreaseVal * timesToRun);
    for (int n = 0; n < timesToRun; n++)
    {
        // int keys[linearIncreaseVal];
        /*
        string vals[linearIncreaseVal];

        for (int i = 0; i < linearIncreaseVal; i++)
        {
            vals[i] = randomString(8);
            // keys[i] = rand();
        }
        */

        Timer t;
        t.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            // putRBT.put(allKeysNew[n * linearIncreaseVal + i], vals[i]);
            putRBT.insert(pair<int, string>(allKeysNew[n * linearIncreaseVal + i], "string"));
        }
        t.stop();

        putData.push_back(make_pair(n * linearIncreaseVal, t.getElapsedTimeInMicroSec()));
    }
    writeCSV(putData,
             "stdmap_put_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");

    cout << "\n**put CSV DONE!**\n**beginning get and remove...**\n";

    map<int, string> getRemRBT;
    shuffle_array(allKeysNew, linearIncreaseVal * timesToRun);
    vector<pair<int, int>> getData, remData;
    for (int n = 0; n < timesToRun; n++)
    {
        Timer t;
        t.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            getRemRBT.find(allKeysNew[n * linearIncreaseVal + i]);
        }
        t.stop();

        getData.push_back(make_pair(linearIncreaseVal * timesToRun - n * linearIncreaseVal,
                                    t.getElapsedTimeInMicroSec()));
        Timer t2;
        t2.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            getRemRBT.erase(allKeysNew[n * linearIncreaseVal + i]);
        }
        t2.stop();

        remData.push_back(make_pair(linearIncreaseVal * timesToRun - n * linearIncreaseVal, // this
                                    t2.getElapsedTimeInMicroSec()));
    }

    reverse(getData.begin(), getData.end());
    reverse(remData.begin(), remData.end());
    writeCSV(getData,
             "RBT_get_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");
    writeCSV(remData,
             "RBT_remove_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");

    // delete[] totalKeysToRem;
    delete[] allKeysNew;
}

int main(int argc, char* argv[])
{
    if (checkArgs(argc, argv, "-t"))
    {
        cout << "RUNNING TIMING TESTS ON RBT... PLEASE WAIT..." << endl;
        // doRBTTimingTestsCSVAllFuncs();
        doNewTiming();
        cout << "\nTESTS ARE COMPLETE..." << endl;
        return 0;
        // RBT<int, char> rbt;
        // for (int i = 0; i < 10; i++)
        // {
        //     rbt.put(i, i + 26);
        // }
        // rbt.dump();
        // processInputLoop(stdin, rbt);
    }
    RBT<int, char> rbt; // int key or MUST BE object with functioning comparator operators > < ==
    printUsage();
    processInputLoop(stdin, rbt);
}