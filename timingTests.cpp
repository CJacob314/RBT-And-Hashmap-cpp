#include <algorithm>
#include <fstream>
#include <map>
#include <random>
#include <string.h>
#include <string>
#include <time.h>
#include <vector>

#include "hashMap.cpp"
#include "redBlackTree.cpp"
#include "timer/src/Timer.cpp"

static bool checkArgs(int argCount, char** mainArgs, const char* toFind)
{
    for (int i = 0; i < argCount; i++)
        if (strcmp(mainArgs[i], toFind) == 0) return true;
    return false;
}

static bool writeCSV(vector<pair<int, int>> data, string fileName)
{
    ofstream csvFile(fileName);

    csvFile << "n,microseconds\n";

    for (int i = 0; i < data.size(); i++)
        csvFile << data.at(i).first << "," << data.at(i).second << "\n";
}

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

static void doNewTimingRBT(const int& linearIncreaseVal = 2000, const int& timesToRun = 5000)
{
    srand(time(0));

    RBT<int, string> opsRBT;
    vector<pair<int, int>> putData;

    int* allKeysNew = makeShuffledArray(linearIncreaseVal * timesToRun);
    for (int n = 0; n < timesToRun; n++)
    {
        Timer t;
        t.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            opsRBT.put(allKeysNew[n * linearIncreaseVal + i], "string");
        }
        t.stop();

        putData.push_back(make_pair(n * linearIncreaseVal, t.getElapsedTimeInMicroSec()));
    }
    writeCSV(putData,
             "RBT_put_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");

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

        t.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            opsRBT.remove(allKeysNew[n * linearIncreaseVal + i]);
        }
        t.stop();

        remData.push_back(make_pair(linearIncreaseVal * timesToRun - n * linearIncreaseVal, // this
                                    t.getElapsedTimeInMicroSec()));
    }

    reverse(getData.begin(), getData.end());
    reverse(remData.begin(), remData.end());
    writeCSV(getData,
             "RBT_get_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");
    writeCSV(remData,
             "RBT_remove_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");

    delete[] allKeysNew;
}

static void doNewTimingHash(const int& linearIncreaseVal = 2000, const int& timesToRun = 5000)
{
    srand(time(0));

    HashMap<int, string> opsHash;
    vector<pair<int, int>> putData;

    int* allKeysNew = makeShuffledArray(linearIncreaseVal * timesToRun);
    for (int n = 0; n < timesToRun; n++)
    {
        Timer t;
        t.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            opsHash.put(allKeysNew[n * linearIncreaseVal + i], "string");
        }
        t.stop();

        putData.push_back(make_pair(n * linearIncreaseVal, t.getElapsedTimeInMicroSec()));
    }
    writeCSV(putData,
             "HASH_put_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");

    shuffle_array(allKeysNew, linearIncreaseVal * timesToRun);
    vector<pair<int, int>> getData, remData;
    for (int n = 0; n < timesToRun; n++)
    {
        Timer t;
        t.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            opsHash.get(allKeysNew[n * linearIncreaseVal + i]);
        }
        t.stop();

        getData.push_back(make_pair(linearIncreaseVal * timesToRun - n * linearIncreaseVal,
                                    t.getElapsedTimeInMicroSec()));

        t.start();
        for (int i = 0; i < linearIncreaseVal; i++)
        {
            opsHash.remove(allKeysNew[n * linearIncreaseVal + i]);
        }
        t.stop();

        remData.push_back(make_pair(linearIncreaseVal * timesToRun - n * linearIncreaseVal, // this
                                    t.getElapsedTimeInMicroSec()));
    }

    reverse(getData.begin(), getData.end());
    reverse(remData.begin(), remData.end());
    writeCSV(getData,
             "HASH_get_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");
    writeCSV(remData,
             "HASH_remove_" + to_string(linearIncreaseVal) + "nX" + to_string(timesToRun) + ".csv");

    delete[] allKeysNew;
}

void interactive(bool& doHash, bool& doRBT, int& n, int& r)
{
    cout << "The program needs the following information. Please enter below:" << endl;

    cout << "Should we run timing tests for hashMap.cpp? (y/N) : ";
    char doHashYN = '\0';
    cin >> doHashYN;
    if (doHashYN == 'y' || doHashYN == 'Y')
        doHash = true;
    else
        doHash = false;
    cout << "\nShould we run timing tests for redBlackTree.cpp? (y/N) : ";
    char doTreeYN = '\0';
    cin >> doTreeYN;
    if (doTreeYN == 'y' || doTreeYN == 'Y')
        doRBT = true;
    else
        doRBT = false;

    if (!doRBT && !doHash)
    {
        cout << "\nWe are done. No testing required...\n";
        exit(EXIT_SUCCESS);
    }

    cout << "\nHow many operations should we test each time? n=? : ";
    cin >> n;
    if (n <= 0)
    {
        cerr << "\nn<=0. Not allowed...\n";
        exit(EXIT_FAILURE);
    }
    cout << "\nHow many times should we test the n operations? r=? : ";
    cin >> r;
    if (r <= 0)
    {
        cerr << "\nr<=0. Not allowed...\n";
        exit(EXIT_FAILURE);
    }
    cout << "\nThank you for inputting values!" << endl;
}

int main(int argc, char* argv[])
{
    bool doHash, doRBT;
    doHash = doRBT = false;
    int n, r;
    n = r = -1;
    for (int a = 1; a < argc; a++)
    {
        if (strcmp(argv[a], "-t") == 0) doRBT = true;
        if (strcmp(argv[a], "-h") == 0) doHash = true;
        if (strcmp(argv[a], "-n") == 0)
        {
            if (a + 1 >= argc)
            {
                cerr << "program read '-n' spec without following number\n";
                return EXIT_FAILURE;
            }
            n = stoi(argv[a + 1]);
        }
        if (strcmp(argv[a], "-r") == 0)
        {
            if (a + 1 >= argc)
            {
                cerr << "program read '-r' spec without following number\n";
                return EXIT_FAILURE;
            }
            r = stoi(argv[a + 1]);
        }
    }
    if (argc < 2 || n == -1 || r == -1)
    {
        interactive(doHash, doRBT, n, r);
    }
    else
    {

        if (!doRBT && !doHash)
        {
            cout << "We are done. No testing required...\n";
            exit(EXIT_SUCCESS);
        }

        if (n <= 0)
        {
            cerr << "\nn<=0. Not allowed...\n";
            exit(EXIT_FAILURE);
        }

        if (r <= 0)
        {
            cerr << "\nr<=0. Not allowed...\n";
            exit(EXIT_FAILURE);
        }
        cout << "Thank you for inputting values!\n";
    }

    if (doRBT)
    {
        cout << "Testing RBT... Please be patient...\n";
        doNewTimingRBT(n, r);
        cout << "RBT testing is complete...\n";
    }
    if (doHash)
    {
        cout << "Testing HashMap... Please be patient...\n";
        doNewTimingHash(n, r);
        cout << "HashMap testing is complete...\n";
    }

    cout << "Everything is done... Thank you...\n";
    return EXIT_SUCCESS;
}