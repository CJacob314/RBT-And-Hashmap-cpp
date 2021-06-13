#include <iostream>

using namespace std;
template <typename kT, typename vT> class HashNode
{

    private:
    kT key;
    vT value;

    HashNode<kT, vT>* next;

    public:
    HashNode<kT, vT>(kT key, vT value)
    {
        this->key = key;
        this->value = value;
        this->next = nullptr;
    }

    kT getKey() { return key; }
    vT getValue() { return value; }

    void setValue(vT value) { this->value = value; }

    HashNode<kT, vT>* getNext() { return next; }

    void setNext(HashNode<kT, vT>* next) { this->next = next; }
};

template <typename kT, typename vT> class HashMap
{

    private:
    HashNode<kT, vT>** table;
    int TABLE_SIZE = PRIMES[0];
    int nElems = 0;
    static const int PRIMES[];
    int primeSizeIndex = 0;
    int primeSizeIndexMax = sizeof(PRIMES) / sizeof(int);

    bool needToGrow() { return nElems / (double)TABLE_SIZE > 0.75; }    // LF 0.75
    bool needToShrink() { return nElems / (double)TABLE_SIZE < 0.375; } // LF 0.375

    void goToNextPrime()
    {
        int oldT_S = TABLE_SIZE;
        TABLE_SIZE = PRIMES[++primeSizeIndex];
        HashNode<kT, vT>** oldTable = table;
        table = new HashNode<kT, vT>*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
        nElems = 0;
        for (int hash = 0; hash < oldT_S; hash++)
        {
            if (oldTable[hash] != nullptr)
            {
                HashNode<kT, vT>* oldEntry;
                HashNode<kT, vT>* entry = oldTable[hash];
                while (entry != nullptr)
                {
                    put(entry->getKey(), entry->getValue());
                    oldEntry = entry;
                    entry = entry->getNext();
                    delete oldEntry;
                }
            }
        }

        delete[] oldTable;
        // cout << "\n**TABLE UPSIZED TO NEXT (roughly) DOUBLE PRIME**\n";
    }

    void doubleTable()
    {
        int oldT_S = TABLE_SIZE;
        TABLE_SIZE *= 2;
        HashNode<kT, vT>** oldTable = table;
        table = new HashNode<kT, vT>*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
        nElems = 0;
        for (int hash = 0; hash < oldT_S; hash++)
        {
            if (oldTable[hash] != nullptr)
            {
                HashNode<kT, vT>* oldEntry;
                HashNode<kT, vT>* entry = oldTable[hash];
                while (entry != nullptr)
                {
                    put(entry->getKey(), entry->getValue());
                    oldEntry = entry;
                    entry = entry->getNext();
                    delete oldEntry;
                }
            }
        }

        delete[] oldTable;
        // cout << "\n**TABLE RESIZED (*=2)**\n";
    }

    void goToPrevPrime()
    {
        int oldT_S = TABLE_SIZE;
        TABLE_SIZE = PRIMES[--primeSizeIndex];
        HashNode<kT, vT>** oldTable = table;
        table = new HashNode<kT, vT>*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
        nElems = 0;
        for (int hash = 0; hash < oldT_S; hash++)
        {
            if (oldTable[hash] != nullptr)
            {
                HashNode<kT, vT>* oldEntry;
                HashNode<kT, vT>* entry = oldTable[hash];
                while (entry != nullptr)
                {
                    put(entry->getKey(), entry->getValue());
                    oldEntry = entry;
                    entry = entry->getNext();
                    delete oldEntry;
                }
            }
        }

        delete[] oldTable;
        // cout << "\n**TABLE DOWNSIZED TO (roughly) HALVED PRIME**\n";
    }

    void halveTable()
    {
        int oldT_S = TABLE_SIZE;
        TABLE_SIZE /= 2;
        HashNode<kT, vT>** oldTable = table;
        table = new HashNode<kT, vT>*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
        nElems = 0;
        for (int hash = 0; hash < oldT_S; hash++)
        {
            if (oldTable[hash] != nullptr)
            {
                HashNode<kT, vT>* oldEntry;
                HashNode<kT, vT>* entry = oldTable[hash];
                while (entry != nullptr)
                {
                    put(entry->getKey(), entry->getValue());
                    oldEntry = entry;
                    entry = entry->getNext();
                    delete oldEntry;
                }
            }
        }

        delete[] oldTable;
        // cout << "\n**TABLE RESIZED (/=2)**\n";
    }

    public:
    HashMap()
    {
        table = new HashNode<kT, vT>*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    vT get(kT key)
    {
        int hash = (std::hash<kT>{}(key) % TABLE_SIZE);
        if (table[hash] == nullptr)
            return "NULL"; // silly c++ not having easy nullable versions of primitive types...
                           // sigh...
        else
        {
            HashNode<kT, vT>* entry = table[hash];

            while (entry != nullptr && entry->getKey() != key) entry = entry->getNext();

            if (entry == nullptr)
                return "NULL";
            else
                return entry->getValue();
        }
    }

    void put(kT key, vT value)
    {
        int hash = (std::hash<kT>{}(key) % TABLE_SIZE);

        if (table[hash] == nullptr)
            table[hash] = new HashNode<kT, vT>(key, value);
        else
        {
            HashNode<kT, vT>* entry = table[hash];

            while (entry->getNext() != nullptr)
            {
                if (entry->getKey() == key)
                {
                    entry->setValue(value);
                    return;
                }
                entry = entry->getNext();
            }

            if (entry->getKey() == key)
            {
                entry->setValue(value);
                return;
            }

            entry->setNext(new HashNode<kT, vT>(key, value));
        }

        nElems++;
        if (needToGrow() && primeSizeIndex + 1 < primeSizeIndexMax) goToNextPrime();
    }

    void remove(kT key)
    {
        int hash = (std::hash<kT>{}(key) % TABLE_SIZE);

        if (table[hash] != nullptr)
        {
            HashNode<kT, vT>* prevEntry = nullptr;
            HashNode<kT, vT>* entry = table[hash];

            while (entry->getNext() != nullptr && entry->getKey() != key)
            {
                prevEntry = entry;
                entry = entry->getNext();
            }

            if (entry->getKey() == key)
            {
                if (prevEntry == nullptr)
                {
                    HashNode<kT, vT>* nextEntry = entry->getNext();
                    delete entry;
                    table[hash] = nextEntry;
                }
                else
                {
                    HashNode<kT, vT>* next = entry->getNext();
                    delete entry;
                    prevEntry->setNext(next);
                }
                nElems--;
                if (needToShrink() && primeSizeIndex - 1 >= 0) goToPrevPrime();
            }
        }
    }

    void dump()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            cout << "Bucket " << i << ": ";
            HashNode<kT, vT>* temp;
            temp = table[i];
            while (temp != nullptr)
            {
                cout << "k: " << temp->getKey() << " --> v: " << temp->getValue() << " | ";
                temp = temp->getNext();
            }
            cout << endl;
        }
    }

    ~HashMap()
    {

        for (int i = 0; i < TABLE_SIZE; i++)
            if (table[i] != nullptr)
            {
                HashNode<kT, vT>* prevEntry = nullptr;
                HashNode<kT, vT>* entry = table[i];

                while (entry != nullptr)
                {
                    prevEntry = entry;
                    entry = entry->getNext();
                    delete prevEntry;
                }
            }

        delete[] table;
    }
};

template <typename kT, typename vT>
const int HashMap<kT, vT>::PRIMES[] = {
    53,       97,       193,      389,       769,       1543,      3079,      6151,      12289,
    24593,    49157,    98317,    196613,    393241,    786433,    1572869,   3145739,   6291469,
    12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};