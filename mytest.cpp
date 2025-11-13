#include "irrigator.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = mt19937(10);// 10 is the fixed seed value
            m_unidist = uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = mt19937(10);// 10 is the fixed seed value
            m_uniReal = uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = mt19937(10);// 10 is the fixed seed value
        m_unidist = uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    random_device m_device;
    mt19937 m_generator;
    normal_distribution<> m_normdist;//normal distribution
    uniform_int_distribution<> m_unidist;//integer uniform distribution
    uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

// the followings are sample priority functions to be used by Region class
// Priority functions compute an integer priority for a crop.
int priorityFn1(const Crop &crop);// works with a MAXHEAP
int priorityFn2(const Crop &crop);// works with a MINHEAP

class Tester{
    public:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_Constructor_MinHeap_Leftist_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 region w/ Leftist min-heap" << endl;
            int rndRegion = regionGen.getRandNum();
            Region aRegion(priorityFn2, MINHEAP, LEFTIST, rndRegion);

            cout << "Checking if Region members match constructor arguments and default values" << endl;
            if ((aRegion.m_priorFunc == priorityFn2) && (aRegion.m_heapType == MINHEAP) 
                && (aRegion.m_structure == LEFTIST) && (aRegion.m_regPrior == rndRegion)
                && (aRegion.m_heap == nullptr) && (aRegion.m_size == 0)) {
                    return true;
            }
            else {
                return false;
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_Constructor_MinHeap_Leftist_Edge() {

            cout << "Creating 1 region w/ invalid region priority" << endl;
            Region aRegion(priorityFn2, MINHEAP, LEFTIST, 0);

            cout << "Checking if Region members correctly initialized to default values" << endl;
            if ((aRegion.m_priorFunc == nullptr) && (aRegion.m_heapType == NOTYPE) 
                && (aRegion.m_structure == NOSTRUCT) && (aRegion.m_regPrior == 0)
                && (aRegion.m_heap == nullptr) && (aRegion.m_size == 0)) {
                    return true;
            }
            else {
                return false;
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_CopyConstructor_MinHeap_Leftist_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 region w/ Leftist min-heap" << endl;
            int rndRegion = regionGen.getRandNum();
            Region aRegion(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating region with 5 crops" << endl;
            for (int i=0; i < 5; i++) {
                Crop aCrop(idGen.getRandNum(),
                            temperature,// it is the same in the region
                            moistureGen.getRandNum(),
                            time,       // it is the same in the region
                            typeGen.getRandNum());
                aRegion.insertCrop(aCrop);
            }
            aRegion.dump();

            cout << "Creating 1 region by copying previous region" << endl;
            Region copyRegion(aRegion);
            copyRegion.dump();

            return checkCopy(aRegion, copyRegion);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_CopyConstructor_MinHeap_Skew_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 region w/ Skew min-heap" << endl;
            int rndRegion = regionGen.getRandNum();
            Region aRegion(priorityFn2, MINHEAP, SKEW, rndRegion);
            cout << "Populating region with 5 crops" << endl;
            for (int i=0; i < 5; i++) {
                Crop aCrop(idGen.getRandNum(),
                            temperature,// it is the same in the region
                            moistureGen.getRandNum(),
                            time,       // it is the same in the region
                            typeGen.getRandNum());
                aRegion.insertCrop(aCrop);
            }
            aRegion.dump();

            cout << "Creating 1 region by copying previous region" << endl;
            Region copyRegion(aRegion);
            copyRegion.dump();

            return checkCopy(aRegion, copyRegion);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_CopyConstructor_MaxHeap_Leftist_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 region w/ Leftist max-heap" << endl;
            int rndRegion = regionGen.getRandNum();
            Region aRegion(priorityFn1, MAXHEAP, LEFTIST, rndRegion);
            cout << "Populating region with 5 crops" << endl;
            for (int i=0; i < 5; i++) {
                Crop aCrop(idGen.getRandNum(),
                            temperature,// it is the same in the region
                            moistureGen.getRandNum(),
                            time,       // it is the same in the region
                            typeGen.getRandNum());
                aRegion.insertCrop(aCrop);
            }
            aRegion.dump();

            cout << "Creating 1 region by copying previous region" << endl;
            Region copyRegion(aRegion);
            copyRegion.dump();

            return checkCopy(aRegion, copyRegion);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_CopyConstructor_MaxHeap_Skew_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 region w/ Skew max-heap" << endl;
            int rndRegion = regionGen.getRandNum();
            Region aRegion(priorityFn1, MAXHEAP, SKEW, rndRegion);
            cout << "Populating region with 5 crops" << endl;
            for (int i=0; i < 5; i++) {
                Crop aCrop(idGen.getRandNum(),
                            temperature,// it is the same in the region
                            moistureGen.getRandNum(),
                            time,       // it is the same in the region
                            typeGen.getRandNum());
                aRegion.insertCrop(aCrop);
            }
            aRegion.dump();

            cout << "Creating 1 region by copying previous region" << endl;
            Region copyRegion(aRegion);
            copyRegion.dump();

            return checkCopy(aRegion, copyRegion);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_CopyConstructor_OneNode_Edge() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 region w/ Leftist min-heap" << endl;
            int rndRegion = regionGen.getRandNum();
            Region aRegion(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating region with 1 crop" << endl;
            Crop aCrop(idGen.getRandNum(),
                        temperature,// it is the same in the region
                        moistureGen.getRandNum(),
                        time,       // it is the same in the region
                        typeGen.getRandNum());
            aRegion.insertCrop(aCrop);
            aRegion.dump();

            cout << "Creating 1 region by copying previous region" << endl;
            Region copyRegion(aRegion);
            copyRegion.dump();

            return checkCopy(aRegion, copyRegion);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_CopyConstructor_EmptyHeap_Edge() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 region w/ Leftist min-heap" << endl;
            Region aRegion(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Leaving Region empty" << endl;
            aRegion.dump();

            cout << "Creating 1 region by copying previous region" << endl;
            Region copyRegion(aRegion);
            copyRegion.dump();

            return checkCopy(aRegion, copyRegion);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_AssignmentOperator_MinHeap_Leftist_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 region w/ Leftist min-heap" << endl;
            int rndRegion = regionGen.getRandNum();
            Region aRegion(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating region with 5 crops" << endl;
            for (int i=0; i < 5; i++) {
                Crop aCrop(idGen.getRandNum(),
                            temperature,// it is the same in the region
                            moistureGen.getRandNum(),
                            time,       // it is the same in the region
                            typeGen.getRandNum());
                aRegion.insertCrop(aCrop);
            }
            aRegion.dump();

            cout << "Creating 1 region by copying previous region via assignment operator" << endl;
            Region copyRegion = aRegion;
            copyRegion.dump();

            return checkCopy(aRegion, copyRegion);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_AssignmentOperator_OneNode_Edge() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 region w/ Leftist min-heap" << endl;
            int rndRegion = regionGen.getRandNum();
            Region aRegion(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating region with 1 crop" << endl;
            Crop aCrop(idGen.getRandNum(),
                        temperature,// it is the same in the region
                        moistureGen.getRandNum(),
                        time,       // it is the same in the region
                        typeGen.getRandNum());
            aRegion.insertCrop(aCrop);
            aRegion.dump();

            cout << "Creating 1 region by copying previous region via assignment operator" << endl;
            Region copyRegion = aRegion;
            copyRegion.dump();

            return checkCopy(aRegion, copyRegion);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_AssignmentOperator_EmptyHeap_Edge() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 region w/ Leftist min-heap" << endl;
            Region aRegion(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Leaving Region empty" << endl;
            aRegion.dump();

            cout << "Creating 1 region by copying previous region via assignment operator" << endl;
            Region copyRegion = aRegion;
            copyRegion.dump();

            return checkCopy(aRegion, copyRegion);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_InsertCrop_MinHeap_Leftist_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Leftist min-heap" << endl;
            Region aHeap(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating Region with 300 Crops" << endl;
            for (int i=0; i < 300; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }
            aHeap.dump();

            cout << "Checking for min-heap property satisfaction at every Crop in heap" << endl;
            return checkHeapness(&aHeap);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_InsertCrop_MaxHeap_Leftist_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Leftist max-heap" << endl;
            Region aHeap(priorityFn1, MAXHEAP, LEFTIST, rndRegion);
            cout << "Populating Region with 300 Crops" << endl;
            for (int i=0; i < 300; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }
            aHeap.dump();

            cout << "Checking for max-heap property satisfaction at every Crop in heap" << endl;
            return checkHeapness(&aHeap);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_InsertCrop_InvalidRegion_Edge() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ NOSTRUCT structure" << endl;
            Region aRegion(priorityFn2, MINHEAP, NOSTRUCT, rndRegion);
            cout << "Populating region with 1 crop" << endl;
            Crop aCrop(idGen.getRandNum(),
                        temperature,// it is the same in the region
                        moistureGen.getRandNum(),
                        time,       // it is the same in the region
                        typeGen.getRandNum());
            aRegion.insertCrop(aCrop);
            //aRegion.dump();

            if (aRegion.m_heap==nullptr) {
                return true;
            }
            else {
                return false;
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_getNextCrop_MinHeap_Leftist_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Leftist min-heap" << endl;
            Region aHeap(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating Region with 300 Crops" << endl;
            for (int i=0; i < 300; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }
            //aHeap.dump();

            cout << "Removing root, comparing priority to next root, and checking heapness of heap after removal" << endl;
            Crop temp;
            bool isMinHeap;

            temp = aHeap.getNextCrop();
            while (aHeap.m_heap != nullptr) {
                if (priorityFn2(temp) > priorityFn2(*(aHeap.m_heap))) {
                    isMinHeap = false;
                }

                //delete &temp;

                isMinHeap = checkHeapness(&aHeap);

                temp = aHeap.getNextCrop();
            }
            //delete &temp;

            return isMinHeap;
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_getNextCrop_MaxHeap_Leftist_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Leftist max-heap" << endl;
            Region aHeap(priorityFn1, MAXHEAP, LEFTIST, rndRegion);
            cout << "Populating Region with 300 Crops" << endl;
            for (int i=0; i < 300; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }
            //aHeap.dump();

            cout << "Removing root, comparing priority to next root, and checking heapness of heap after removal" << endl;
            Crop temp;
            bool isMaxHeap;

            temp = aHeap.getNextCrop();
            while (aHeap.m_heap != nullptr) {
                if (priorityFn2(temp) < priorityFn2(*(aHeap.m_heap))) {
                    isMaxHeap = false;
                }

                //delete &temp;

                isMaxHeap = checkHeapness(&aHeap);

                temp = aHeap.getNextCrop();
            }
            //delete &temp;

            return isMaxHeap;
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_getNextCrop_EmptyHeap_Edge() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Leftist max-heap" << endl;
            Region aHeap(priorityFn1, MAXHEAP, LEFTIST, rndRegion);

            cout << "Attempting to dequeue Crop from empty Region" << endl;
            try {
                Crop temp = aHeap.getNextCrop();
            }
            catch(out_of_range& e) {
                cout << e.what() << endl;
                return true;
            }

            return false;
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_NPL_Value_MinHeap() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Leftist min-heap" << endl;
            Region aHeap(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating Region with 300 Crops" << endl;
            for (int i=0; i < 300; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }

            cout << "Checking if all NPL values in Region are valid" << endl;
            bool validNPL = true;
            return checkNPL(aHeap.m_heap, validNPL);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_Leftist_Property_MinHeap() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Leftist min-heap" << endl;
            Region aHeap(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating Region with 300 Crops" << endl;
            for (int i=0; i < 300; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }

            cout << "Checking if Leftist property is preserved throughout Region" << endl;
            bool isLeftist = true;
            return checkLeftist(aHeap.m_heap, isLeftist);
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_setPriority_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Leftist min-heap" << endl;
            Region aHeap(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating Region with 300 Crops" << endl;
            for (int i=0; i < 300; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }

            cout << "Checking if Leftist property is preserved throughout Region" << endl;
            bool originalIsLeftist = true;
            originalIsLeftist = checkLeftist(aHeap.m_heap, originalIsLeftist);

            cout << "Changing Region from min-heap to max-heap" << endl;
            aHeap.setPriorityFn(priorityFn1, MAXHEAP);

            cout << "Checking if Leftist property is preserved throughout Region after rebuilding" << endl;
            bool rebuiltIsLeftist = true;
            rebuiltIsLeftist = checkLeftist(aHeap.m_heap, rebuiltIsLeftist);

            if (originalIsLeftist && rebuiltIsLeftist) {
                cout << "Heaps before and after rebuilding both satisfy Leftist property" << endl;
                return true;
            }
            else if (originalIsLeftist && !rebuiltIsLeftist){
                cout << "Original heap is Leftist, but after rebuilding it is not" << endl;
                return false;
            }
            else if (!originalIsLeftist && rebuiltIsLeftist) {
                cout << "Original heap not Leftist, but after rebuilding it is" << endl;
                return false;
            }
            else {
                cout << "Heaps before and after rebuilding do not satisfy Leftist property" << endl;
                return false;
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_setPriority_Edge() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 empty Region w/ Leftist min-heap" << endl;
            Region aHeap(priorityFn2, MINHEAP, LEFTIST, rndRegion);

            cout << "Checking if Leftist property is preserved throughout empty Region" << endl;
            bool originalIsLeftist = true;
            originalIsLeftist = checkLeftist(aHeap.m_heap, originalIsLeftist);

            cout << "Changing Region from min-heap to max-heap" << endl;
            aHeap.setPriorityFn(priorityFn1, MAXHEAP);

            cout << "Checking if Leftist property is preserved throughout empty Region after rebuilding" << endl;
            bool rebuiltIsLeftist = true;
            rebuiltIsLeftist = checkLeftist(aHeap.m_heap, rebuiltIsLeftist);

            if (originalIsLeftist && rebuiltIsLeftist) {
                cout << "Heaps before and after rebuilding both satisfy Leftist property" << endl;
                return true;
            }
            else if (originalIsLeftist && !rebuiltIsLeftist){
                cout << "Original heap is Leftist, but after rebuilding it is not" << endl;
                return false;
            }
            else if (!originalIsLeftist && rebuiltIsLeftist) {
                cout << "Original heap not Leftist, but after rebuilding it is" << endl;
                return false;
            }
            else {
                cout << "Heaps before and after rebuilding do not satisfy Leftist property" << endl;
                return false;
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_setStructure_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Skew min-heap" << endl;
            Region aHeap(priorityFn2, MINHEAP, SKEW, rndRegion);
            cout << "Populating Region with 300 Crops" << endl;
            for (int i=0; i < 300; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }

            cout << "Checking if Leftist property is preserved throughout Region" << endl;
            bool originalIsLeftist = true;
            originalIsLeftist = checkLeftist(aHeap.m_heap, originalIsLeftist);

            cout << "Changing Region from Skew to Leftist" << endl;
            aHeap.setStructure(LEFTIST);

            cout << "Checking if Leftist property is preserved throughout Region after rebuilding" << endl;
            bool rebuiltIsLeftist = true;
            rebuiltIsLeftist = checkLeftist(aHeap.m_heap, rebuiltIsLeftist);

            if (!originalIsLeftist && rebuiltIsLeftist) {
                cout << "Heap before does not satisfy Leftist but after rebuilding it does" << endl;
                return true;
            }
            else if (!originalIsLeftist && !rebuiltIsLeftist){
                cout << "Heap does not satisfy Leftist before or after rebuilding" << endl;
                return false;
            }
            else if (originalIsLeftist && rebuiltIsLeftist) {
                cout << "Heap satisfies leftist before and after rebuild" << endl;
                return true;
            }
            else {
                cout << "Heap is leftist before rebuild but not after" << endl;
                return false;
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_printCropsQueue() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Skew min-heap" << endl;
            Region aHeap(priorityFn2, MINHEAP, SKEW, rndRegion);
            cout << "Populating Region with 20 Crops" << endl;
            for (int i=0; i < 20; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }

            aHeap.printCropsQueue();

            return true;
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_mergeWithQueue_Heap_Properties_Normal() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Leftist min-heap" << endl;
            Region aHeap(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating Region with 20 Crops" << endl;
            for (int i=0; i < 20; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }

            cout << "Creating 1 Region w/ Leftist min-heap" << endl;
            Region bHeap(priorityFn2, MINHEAP, LEFTIST, rndRegion);
            cout << "Populating Region with 20 Crops" << endl;
            for (int i=0; i < 20; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }

            cout << "Merging both Regions" << endl;
            aHeap.mergeWithQueue(bHeap);

            cout << "Checking Leftist property and min-heapness in new merged Region" << endl;
            bool isLeftist = true;
            isLeftist = checkLeftist(aHeap.m_heap, isLeftist);

            bool isMinHeap = checkHeapness(&aHeap);

            if (isLeftist && isMinHeap) {
                cout << "Merged queue maintains Leftist and min-heap properties" << endl;
                return true;
            }
            else if (!isLeftist && isMinHeap) {
                cout << "Merged queue is not Leftist but is min-heap" << endl;
                return false;
            }
            else if (isLeftist && !isMinHeap) {
                cout << "Merged queue is Leftist but is not min-heap" << endl;
                return false;
            }
            else {
                cout << "Merged queue is neither Leftist nor min-heap" << endl;
                return false;
            }

            return true;
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool test_mergeWithQueue_Edge() {
            //////////////////////Random Generators////////////////////////
            Random regionGen(1,30);
            Random idGen(MINCROPID,MAXCROPID);
            Random temperatureGen(MINTEMP,MAXTEMP);
            int temperature = temperatureGen.getRandNum();
            Random moistureGen(MINMOISTURE,MAXMOISTURE);
            Random timeGen(MINTIME,MAXTIME);
            int time = timeGen.getRandNum();
            Random typeGen(MINTYPE,MAXTYPE);
            int rndRegion = regionGen.getRandNum();
            ///////////////////////////////////////////////////////////////

            cout << "Creating 1 Region w/ Skew min-heap" << endl;
            Region aHeap(priorityFn2, MINHEAP, SKEW, rndRegion);
            cout << "Populating Region with 20 Crops" << endl;
            for (int i=0; i < 20; i++) {
                Crop aNode(idGen.getRandNum(), 
                    temperature, 
                    moistureGen.getRandNum(), 
                    time, 
                    typeGen.getRandNum());
                aHeap.insertCrop(aNode);
            }

            aHeap.printCropsQueue();

            return true;
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////// TEST HELPER FUNCTIONS /////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool checkHeapness (Region* heap) const {
            if (heap->getHeapType() == MINHEAP) {
                bool isMinHeap = true;
                return checkMinHeapness(heap->m_heap, isMinHeap);
            }
            else if (heap->getHeapType() == MAXHEAP) {
                bool isMaxHeap = true;
                return checkMaxHeapness(heap->m_heap, isMaxHeap);
            }
            // heap is of type NOTYPE
            else {
                return false;
            }
        }

        /*
        Takes Crop (node) pointer and reference to boolean representing heapness property
        Preorder traversal of tree, checking if Crop priority is lower (higher number) than that of its children
        Returns true if heap satisfies minHeap property at all nodes, false if not
        */
        bool checkMinHeapness(Crop* heap, bool& isHeap) const {
            // base case: empty node
            if (heap == nullptr) {
                return isHeap;
            }

            // check if root priority is lower priority (larger number) than its children
            if (heap->m_left != nullptr) {
                if (priorityFn2(*heap) > priorityFn2(*(heap->m_left))) {
                    isHeap = false;
                }
            }
            if (heap->m_right != nullptr) {
                if (priorityFn2(*heap) > priorityFn2(*(heap->m_right))) {
                    isHeap = false;
                }
            }

            isHeap = checkMinHeapness(heap->m_left, isHeap);

            isHeap = checkMinHeapness(heap->m_right, isHeap);

            return isHeap;
        }

        bool checkMaxHeapness(Crop* heap, bool& isHeap) const {
            // base case: empty node
            if (heap == nullptr) {
                return isHeap;
            }
            // check if root priority is lower priority (smaller number) than its children
            if (heap->m_left != nullptr) {
                if (priorityFn1(*heap) < priorityFn1(*(heap->m_left))) {
                    isHeap = false;
                }
            }
            if (heap->m_right != nullptr) {
                if (priorityFn1(*heap) < priorityFn1(*(heap->m_right))) {
                    isHeap = false;
                }
            }

            isHeap = checkMaxHeapness(heap->m_left, isHeap);

            isHeap = checkMaxHeapness(heap->m_right, isHeap);

            return isHeap;
        }

        // Compares two Region parameters to determine if they are deep-copies of each other or not
        // If this function returns true, the Regions are deep-copies (identical in value + shape, but not in memory addr)
        // If returns false, either the Regions are not copies or they are shallow copies
        bool checkCopy(Region& aRegion, Region& copyRegion) const {
            bool isCopied = true;

            // Checks if non-object members of parameter Regions have the same values
            if ((aRegion.m_size != copyRegion.m_size) || (aRegion.m_priorFunc != copyRegion.m_priorFunc) || 
                (aRegion.m_heapType != copyRegion.m_heapType) || (aRegion.m_structure != copyRegion.m_structure) ||
                (aRegion.m_regPrior != copyRegion.m_regPrior))
            {
                isCopied = false;
            }

            // Checks if the heaps in both parameters are copies of one another in node values
            bool isCropCopied = true;
            if (!checkHeapValue(aRegion.m_heap, copyRegion.m_heap, isCropCopied)) {
                isCopied = false;
            }

            // Checks if the heaps in both parameters are deep-copies; i.e. different addresses
            bool isDeepCopy = true;
            if (!checkHeapAddress(aRegion.m_heap, copyRegion.m_heap, isDeepCopy)) {
                isCopied = false;
            }

            return isCopied;
        }

        // Recursively pre-order traverses both heaps rooted in parameters, moving to same node in each tree
        // If any Crop node in heap does not match its cousin in other heap, isCopy parameter is set to false
        // If isCopy returns true, both parameter heaps are identical in value and shape
        // If isCopy returns false, the parameter heaps are not identical
        bool checkHeapValue(Crop* aRoot, Crop* copyRoot, bool& isCopy) const {
            // base case
            if ((aRoot==nullptr) && (copyRoot==nullptr)) {
                return isCopy;
            }
            // if the non-object data members of both parameter Crops do not match, the two heaps are not copies
            if (!compareCrops(aRoot, copyRoot)) {
                isCopy = false;
            }
            // recurse down left subheap of Crops
            isCopy = checkHeapValue(aRoot->m_left, copyRoot->m_left, isCopy);
            // recurse down right subtree of Crops
            isCopy = checkHeapValue(aRoot->m_right, copyRoot->m_right, isCopy);
            // if all Crops match so far, this returns true
            // if there have been any mismatches reflecting parameters not being copies value-wise, this returns false
            return isCopy;
        }

        // Compares the non-object members of two Crop parameters
        // If any members do not match in value, this function returns false
        // Else this function returns true, indicating all members match
        bool compareCrops(Crop* aCrop, Crop* copyCrop) const {
            bool sameVal = true;

            if (aCrop->m_cropID != copyCrop->m_cropID) sameVal = false;
            if (aCrop->m_temperature != copyCrop->m_temperature) sameVal = false;
            if (aCrop->m_moisture != copyCrop->m_moisture) sameVal = false;
            if (aCrop->m_time != copyCrop->m_time) sameVal = false;
            if (aCrop->m_type != copyCrop->m_type) sameVal = false;
            if (aCrop->m_npl != copyCrop->m_npl) sameVal = false;

            return sameVal;
        }

        // Compares the addresses pointed to by both Crop* parameters
        // If both point to the same object, a deep-copy was not properly performed
        // Returns true if deep-copy confirmed, false if disproved
        bool checkHeapAddress(Crop* aRoot, Crop* copyRoot, bool& isDeepCopy) const {
            // base case
            if ((aRoot==nullptr) && (copyRoot==nullptr)) {
                return isDeepCopy;
            }
            // if both Crop* point to the same object, a deep-copy was not performed
            if (aRoot == copyRoot) {
                isDeepCopy = false;
            }
            // recurse down left subheap of Crops
            isDeepCopy = checkHeapAddress(aRoot->m_left, copyRoot->m_left, isDeepCopy);
            // recurse down right subtree of Crops
            isDeepCopy = checkHeapAddress(aRoot->m_right, copyRoot->m_right, isDeepCopy);
            // If both Crop* parameters and their children point to different addresses, this returns true
            return isDeepCopy;
        }

        bool checkNPL(Crop* node, bool& validNPL) const {
            // base case
            if (node == nullptr) {
                return validNPL;
            }

            // Find the minimum NPL among node's children; empty node has NPL of -1
            int leftNPL = -1;
            if (node->m_left != nullptr) {
                leftNPL = node->m_left->m_npl;
            }

            int rightNPL = -1;
            if (node->m_right != nullptr) {
                rightNPL = node->m_right->m_npl;
            }

            int minNPL;
            if (leftNPL > rightNPL) minNPL = rightNPL;
            else minNPL = leftNPL;

            // If node NPL is not 1 more than minimum NPL of its children, its NPL is invalid
            if (node->m_npl != (minNPL + 1)) validNPL = false;

            validNPL = checkNPL(node->m_left, validNPL);

            validNPL = checkNPL(node->m_right, validNPL);

            return validNPL;
        }

        bool checkLeftist(Crop* node, bool& isLeftist) const {
            // base case
            if (node == nullptr) {
                return isLeftist;
            }

            // calculate children NPLs (-1 if empty)
            int leftNPL = -1;
            if (node->m_left != nullptr) {
                leftNPL = node->m_left->m_npl;
            }

            int rightNPL = -1;
            if (node->m_right != nullptr) {
                rightNPL = node->m_right->m_npl;
            }

            // test Leftist property, i.e. if left NPL is greater than or equal to right NPL
            if (leftNPL < rightNPL) isLeftist = false;

            isLeftist = checkLeftist(node->m_left, isLeftist);

            isLeftist = checkLeftist(node->m_right, isLeftist);

            return isLeftist;
        }

        // traverses Region heap in preorder and stores Crop* nodes in vector parameter
        void preorderVector(Crop* node, vector<Crop*>& heap) const {
            // base case
            if (node==nullptr) {
                return;
            }
            // Stores Crop* in vector
            heap.push_back(node);

            preorderVector(node->m_left, heap);

            preorderVector(node->m_right, heap);

            return;
        }

      
};

int main() {
    bool passed = true;
    Tester test;

    cout << "Testing" << endl;

    cout << "Checking Region parameterized constructor: normal case" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking with Leftist min-heap" << endl;
    if (test.test_Constructor_MinHeap_Leftist_Normal()) {
        cout << "test_Constructor_MinHeap_Leftist_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_Constructor_MinHeap_Leftist_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking Region parameterized constructor: edge case" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking with Leftist min-heap" << endl;
    if (test.test_Constructor_MinHeap_Leftist_Edge()) {
        cout << "test_Constructor_MinHeap_Leftist_Edge has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_Constructor_MinHeap_Leftist_Edge has FAILED" << endl << endl;
    }
 /////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking Region copy constructor: normal cases" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking with Leftist min-heap" << endl;
    if (test.test_CopyConstructor_MinHeap_Leftist_Normal()) {
        cout << "test_CopyConstructor_MinHeap_Leftist_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_CopyConstructor_MinHeap_Leftist_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking with Leftist max-heap" << endl;
    if (test.test_CopyConstructor_MaxHeap_Leftist_Normal()) {
        cout << "test_CopyConstructor_MaxHeap_Leftist_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_CopyConstructor_MaxHeap_Leftist_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking with Skew min-heap" << endl;
    if (test.test_CopyConstructor_MinHeap_Skew_Normal()) {
        cout << "test_CopyConstructor_MinHeap_Skew_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_CopyConstructor_MinHeap_Skew_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking with Skew max-heap" << endl;
    if (test.test_CopyConstructor_MaxHeap_Skew_Normal()) {
        cout << "test_CopyConstructor_MaxHeap_Skew_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_CopyConstructor_MaxHeap_Skew_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking Region copy constructor: edge cases" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking when copying Region with only 1 Crop in heap" << endl;
    if (test.test_CopyConstructor_OneNode_Edge()) {
        cout << "test_CopyConstructor_OneNode_Edge has PASSED" << endl << endl; 
    }
    else {
        passed = false;
        cout << "test_CopyConstructor_OneNode_Edge has FAILED" << endl << endl; 
    }
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking when copying Region with empty heap" << endl;
    if (test.test_CopyConstructor_EmptyHeap_Edge()) {
        cout << "test_CopyConstructor_EmptyHeap_Edge has PASSED" << endl << endl; 
    }
    else {
        passed = false;
        cout << "test_CopyConstructor_EmptyHeap_Edge has FAILED" << endl << endl; 
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking Region assignment operator: normal cases" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking with Leftist min-heap" << endl;
    if (test.test_AssignmentOperator_MinHeap_Leftist_Normal()) {
        cout << "test_AssignmentOperator_MinHeap_Leftist_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_AssignmentOperator_MinHeap_Leftist_Normal has FAILED" << endl << endl;
    }
 /////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking Region assignment operator: edge cases" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking with Leftist min-heap" << endl;
    if (test.test_AssignmentOperator_OneNode_Edge()) {
        cout << "test_AssignmentOperator_OneNode_Edge has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_AssignmentOperator_OneNode_Edge has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking with Leftist min-heap" << endl;
    if (test.test_AssignmentOperator_EmptyHeap_Edge()) {
        cout << "test_AssignmentOperator_EmptyHeap_Edge has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_AssignmentOperator_EmptyHeap_Edge has FAILED" << endl << endl;
    }
 /////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking Region insertCrop: normal cases" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking insertion to Leftist min-heap" << endl;
    if (test.test_InsertCrop_MinHeap_Leftist_Normal()) {
        cout << "test_InsertCrop_MinHeap_Leftist_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_InsertCrop_MinHeap_Leftist_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking insertion to Leftist max-heap" << endl;
    if (test.test_InsertCrop_MaxHeap_Leftist_Normal()) {
        cout << "test_InsertCrop_MaxHeap_Leftist_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_InsertCrop_MaxHeap_Leftist_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking Region insertCrop: edge cases" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking insertion of Crop to invalid Region" << endl;
    if (test.test_InsertCrop_InvalidRegion_Edge()) {
        cout << "test_InsertCrop_InvalidRegion_Edge has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_InsertCrop_InvalidRegion_Edge has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking Region getNextCrop: normal cases" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking removal from Leftist min-heap" << endl;
    if (test.test_getNextCrop_MinHeap_Leftist_Normal()) {
        cout << "test_getNextCrop_MinHeap_Leftist_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_getNextCrop_MinHeap_Leftist_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking removal from Leftist max-heap" << endl;
    if (test.test_getNextCrop_MaxHeap_Leftist_Normal()) {
        cout << "test_getNextCrop_MaxHeap_Leftist_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_getNextCrop_MaxHeap_Leftist_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking Region getNextCrop: edge cases" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking removal from empty Region" << endl;
    if (test.test_getNextCrop_EmptyHeap_Edge()) {
        cout << "test_getNextCrop_EmptyHeap_Edge has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_getNextCrop_EmptyHeap_Edge has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking if all Crops in a Leftist min-heap have correct NPLs" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    if (test.test_NPL_Value_MinHeap()) {
        cout << "test_NPL_Value_MinHeap has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_NPL_Value_MinHeap has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking if all Crops in a Leftist min-heap satisfy Leftist property" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    if (test.test_Leftist_Property_MinHeap()) {
        cout << "test_Leftist_Property_MinHeap has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_Leftist_Property_MinHeap has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking changing priority function for Region" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    if (test.test_setPriority_Normal()) {
        cout << "test_setPriority_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_setPriority_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////
    cout << "Checking changing priority function for empty Region" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    if (test.test_setPriority_Edge()) {
        cout << "test_setPriority_Edge has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_setPriority_Edge has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Checking changing structure for Region" << endl << endl;
/////////////////////////////////////////////////////////////////////////////////////////////
    if (test.test_setStructure_Normal()) {
        cout << "test_setStructure_Normal has PASSED" << endl << endl;
    }
    else {
        passed = false;
        cout << "test_setStructure_Normal has FAILED" << endl << endl;
    }
/////////////////////////////////////////////////////////////////////////////////////////////

    cout << "Testing printCropsQueue" << endl;
    test.test_printCropsQueue();

    if (passed) {
        cout << endl << "All tests passed! Yippee!" << endl;
    }
    else {
        cout << endl << "All tests did not pass, but you've got this!" << endl;
    }
    return 0;
}

int priorityFn1(const Crop &crop) {
    //needs MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [30-116]
    //the highest priority would be 110+6 = 116
    //the lowest priority would be 30+0 = 30
    //the larger value means the higher priority
    int minValue = 30;
    int maxValue = 116;
    int priority = crop.getTemperature() + crop.getType();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}

int priorityFn2(const Crop &crop) {
    //needs MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1-103]
    //the highest priority would be 1+0 = 1
    //the lowest priority would be 100+3 = 103
    //the smaller value means the higher priority
    int minValue = 1;
    int maxValue = 103;
    int priority = crop.getMoisture() + crop.getTime();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}