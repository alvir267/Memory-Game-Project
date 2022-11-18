#include "MemoryGame.hpp"
#include <cstdlib> //srand, rand
#include <ctime>
#include <iomanip>  //setw
#include <iostream> //std
#include <vector>
using namespace std;

int *randomize(int numSlots); //function to randomize layout
void swap(int *arr, int i, int j);
void displaySeparateLine(int numSlots);

MemoryGame::MemoryGame() //default constructor,
    //set numPairs = 3, numSlots = 2 * numPairs + 2,
    //put 3 pairs of randomly generated ints in [0, 1000)
{
    srand(time(NULL));
    //different layouts of numbers in different runnings.

    numPairs = 3; //do not write int numPairs = 3;
    numSlots = 8; //3 pairs residing in 8 spaces,
                  //there are two empty spaces

    //generate three random ints in [0, 1000),
    //randomly set them in the layout of the spaces,

    vector<int> data;
    for (int i = 0; i < numPairs; i++)
    {
        data.push_back(rand() % 1000);
    }
    //numSlots = 2 * numPairs + 2;
    int *mixed = randomize(numSlots);
    values = new string[numSlots];
    for (int i = 0; i < numSlots; i++)
    {
        if (i < numPairs * 2)
        {
            values[mixed[i]] = to_string(data[i / 2]);
        }
        else
        {
            values[mixed[i]] = " ";
        }
    }
    delete[] mixed;
    mixed = nullptr;
}

MemoryGame::~MemoryGame()
{
    delete[] values;
    values = nullptr;
    //When an object is no longer in need,
    //release dynamically allocated memory by
    //data members of the current object.
}

//randomize is not a member function,
//so there is no MemoryGame:: before function name randomize.
//Return an array of randomly allocated 0, 1, .., size-1
//In constructors, randomly assign the data in dataVector
//to array values
int *randomize(int size)
{
    //idea to randomize 0, 1, 2, 3, 4, 5,
    //generate a random int in [0, 6), say 3,
    //then move arr[3] to the end,
    //say, 0, 1, 2, 5, 4, 3
    //generate a random int in [0, 5), say 3 again,
    //then swap arr[3], which is 5 now, with arr[4],
    //get 0, 1, 2, 4, 5, 3
    //generate a random int in [0, 4), say 2,
    //swap arr[2] with the current arr[3]
    //get 0, 1, 4, 2, 5, 3
    //continue to randomize arr[0..2].
    //afterwards, continue to randomize arr[0..1].

    int *arr = new int[size];
    for (int j = 0; j < size; j++)
    {
        arr[j] = j;
    }

    for (int i = size; i > 0; i--)
    {
        swap(arr, rand() % i, i - 1);
    }

    return arr;
}

//int* arr means int array arr, which implies the address
//of the first element of array arr.
//swap arr[i] and arr[j] in array of ints arr.
void swap(int *arr, int i, int j)
{
    swap(arr[i], arr[j]);
}

//Display -----+ for numSlots times.
//Add an extra + when for the first block (when index i is 0).
//So suppose numSlots is 8, we get
//+-----+-----+-----+-----+-----+-----+-----+-----+
//followed by a new line.
void displaySeparateLine(int numSlots)
{
    for (int i = 0; i < numSlots; i++)
    {
        if (i == 0)
            cout << "+";
        cout << "-----+";
    }
    cout << endl;
}

//display the items in values array where bShown is true.
void MemoryGame::display(bool *bShown)
{
    for (int i = 0; i < numSlots; i++)
    {
        if (i == 0)
        {
            cout << setw(4) << i;
        }
        else
        {
            cout << setw(6) << i;
        }
    }
    cout << "   " << endl;
    displaySeparateLine(numSlots);
    cout << "|";
    for (int i = 0; i < numSlots; i++)
    {
        if (bShown[i])
        {
            if (values[i] != " ")
            {
                cout << setw(5) << values[i] << "|";
            }
            else
            {
                cout << setw(5) << " "
                     << "|";
            }
        }
        else
        {
            cout << setw(5) << " "
                 << "|";
        }
    }
    cout << endl;
    displaySeparateLine(numSlots);
}

//rules for a round
//(1) pick a number, flip it
//(2) pick a second number, if the number is the same as
//    previous pick, display the second number,
//    otherwise, unflip the previous pick.
//(3) Finish until every pair are chosen correctly.
void MemoryGame::play()
{

    bool *bShown = new bool[numSlots];
    for (int i = 0; i < numSlots; i++)
    {
        bShown[i] = false;
    }
    int numFlips = 0;
    int pairsFound = 0;
    int toBeMatched;
    display(bShown);
    int index;
    while (numPairs != pairsFound)
    {
        cout << "Pick a cell to flip: ";
        cin >> index;
        while (index < 0 || index >= numSlots || bShown[index] == true)
        {
            if (index < 0 || index >= numSlots)
            {
                cout << "Out of range!" << endl;
                cout << "Re-enter an index: ";
                cin >> index;
                cout << endl;
            }
            else if (bShown[index] == true)
            {
                cout << "The cell indexed at " << index << " is shown." << endl;
                cout << "Re-enter an index: ";
                cin >> index;
            }
        }
        numFlips++;

        if (numFlips % 2 != 0)
        {
            bShown[index] = true;
            toBeMatched = index;
        }
        else
        {
            if (values[index] == values[toBeMatched] && (values[index] != " " && values[toBeMatched] != " "))
            {
                pairsFound++;
                bShown[index] = true;
            }
            else
            {
                bShown[toBeMatched] = false;
            }
        }
        display(bShown);
    }
    cout << "Congratulations! Take " << numFlips << " steps to find all matched pairs.";
    delete[] bShown;
    bShown = nullptr;
}