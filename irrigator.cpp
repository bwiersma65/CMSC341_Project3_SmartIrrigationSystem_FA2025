// CMSC 341 - Fall 2025 - Project 3
#include "irrigator.h"
// creates empty Region with default/empty values
Region::Region(){ 
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = nullptr;
  m_heapType = NOTYPE;
  m_structure = NOSTRUCT;
  m_regPrior = 0;
}
// creates empty Region specified by parameters
Region::Region(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure, int regPrior)
{
  m_heap = nullptr;
  m_size = 0;

  // if ((heapType == NOTYPE) || (structure == NOSTRUCT) || (regPrior <= 0)) {
  if ((heapType == NOTYPE) || (structure == NOSTRUCT)) {
    m_priorFunc = nullptr;
    m_heapType = NOTYPE;
    m_structure = NOSTRUCT;
    m_regPrior = 0;
  }
  else {
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_structure = structure;
    m_regPrior = regPrior;
  }
}
// Deallocates memory and re-initializes member variables
Region::~Region()
{
  this->clear();
}
// FIX ME
// Clears the queue; delete all nodes in heap leaving it empty, and re-initialize member variables
void Region::clear() {
  // implement this
  delete m_heap;

  m_size = 0;
  m_priorFunc = nullptr;
  //m_heapType = NOTYPE;
  m_structure = NOSTRUCT;
  m_regPrior = 0;    
}
// Edge case: copy empty object (rhs is empty)
Region::Region(const Region& rhs)
{
  
}
// Edge case: copy empty object (rhs is empty)
Region& Region::operator=(const Region& rhs) {
  // checks against self-copying; if both heap roots are the same address, they are the same Region
  if (rhs.m_heap == m_heap) {
    return;
  }

}
void Region::mergeWithQueue(Region& rhs) {
  // checks against self-merging; if both heap roots are the same address, they are the same Region
  if (rhs.m_heap == m_heap) {
  //if (rhs==this) {
    return;
  }
  // if both Regions do not match in structure or heap type, do not merge
  if ((rhs.m_structure != m_structure) || (rhs.m_heapType != m_heapType)) {
    throw domain_error("Mismatch heaptype or structure");
  }
  // CHECK THIS
  // Checks if rhs priority is valid
  if (rhs.m_regPrior < 0) {
    return;
  }
}
// Add check for if crop argument is nullptr
bool Region::insertCrop(const Crop& crop) {
  // if any of Region members are set to invalid values, do not add Crop to Region
  if ((m_priorFunc==nullptr) || (m_heapType==NOTYPE) || (m_structure==NOSTRUCT) || (m_regPrior <= 0)) {
    return false;
  }
  // checks if crop is null or not; if so, do not attempt to add to Region
  else if (&crop == nullptr) {
    return false;
  }
  // if Crop is of invalid priority value, do not add to Region heap
  else if (m_priorFunc(crop) <= 0) {
    return false;
  }
  else {

  }
}
// uses private helper countCrops to reursively preorder traverse heap and increment counter for each non-null node
int Region::numCrops() const
{
  // counter
  int numCrops = 0;
  // call to recursive helper
  countCrops(m_heap, numCrops);
  // returns number of non-null nodes visited in heap tree
  return numCrops;
}
prifn_t Region::getPriorityFn() const {
  return m_priorFunc;
}
// This should remove the highest priority Crop (root of heap, at m_heap), merge the resultant left and right
// subtrees together, and return the removed Crop as an object
// If the heap is empty when this function is called, throw an out_of_range exception
Crop Region::getNextCrop() {
  // Check if the heap is empty
  if (m_heap==nullptr) {
    throw out_of_range("Crop queue is empty in this Region");
  }
  else {

  }
}
// Change the m_priorFunc and heapType members for the object to values of respective parameters
// if they differ from the previous values, rebuild the heap using the new ones (by using merge helper)
// Do not reallocate new memory, just reuse the pre-existing nodes
// Don't need to check that prifn is nullptr; make sure heapType is valid
void Region::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  // If NOTYPE passed as argument, clear the heap and set variables to reflect empty Region
  if (heapType==NOTYPE) {
    this->clear();
    return;
  }
  else {

  }
}
// Change the m_structure member for the object to value of parameter
// if it differs from the previous value, rebuild the heap using the new one
// Do not reallocate new memory, just reuse the pre-existing nodes
void Region::setStructure(STRUCTURE structure){
  // If NOSTRUCT is passed as argument, clear the heap and set variables to reflect empty Region
  if (structure==NOSTRUCT) {
    this->clear();
    return;
  }
  else {
    STRUCTURE prev = m_structure;
    STRUCTURE next = structure;

    if ((prev==SKEW) && (next==LEFTIST)) {

    }
    else if ((prev==NOSTRUCT) && (next==SKEW)) {

    }
    else if ((prev==NOSTRUCT) && (next==LEFTIST)) {

    }
  }
}
STRUCTURE Region::getStructure() const {
  return m_structure;
}
HEAPTYPE Region::getHeapType() const {
  return m_heapType;
}
void Region::printCropsQueue() const {
  
}

void Region::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    cout << "Region " << m_regPrior << ": => ";
    dump(m_heap);
  }
  cout << endl;
}
void Region::dump(Crop *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(*pos) << ":" << pos->m_cropID;
    else
        cout << m_priorFunc(*pos) << ":" << pos->m_cropID << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Crop& crop) {
  sout << "Crop ID: " << crop.getCropID() 
        << ", current temperature: " << crop.getTemperature()
        << ", current soil moisture: " << crop.getMoisture() << "%"
        << ", current time: " << crop.getTimeString()
        << ", plant type: " << crop.getTypeString();
  return sout;
}

void Region::countCrops(Crop* node, int& numCrops) const {
  if (node == NULL) {
    return;
  }

  numCrops++;

  countCrops(node->m_left, numCrops);

  countCrops(node->m_right, numCrops);
}

Crop* Region::merge(Crop* p1, Crop* p2) {
  // check if either heap to be merged is empty; if so, result is just non-empty heap
  if (p1==nullptr) return p2;
  else if (p2==nullptr) return p1;

  if (m_structure==LEFTIST) {

    // LEFTIST MIN-HEAP
    if (m_heapType==MINHEAP) {
      // p1 is lower priority than p2
      if (m_priorFunc(*p1) > m_priorFunc(*p2)) {
        // swap p1 and p2 so p1 holds the higher-priority Crop
        Crop* temp = p1;
        p1 = p2;
        p2 = temp;
        temp = nullptr;
      }

      p1->m_right = merge(p1->m_right, p2);


    }
    // LEFTIST MAX-HEAP
    else if (m_heapType==MAXHEAP) {

    }

  }
  else if (m_structure==SKEW) {

    // SKEW MIN-HEAP
    if (m_heapType==MINHEAP) {
      // p1 is lower priority than p2
      if (m_priorFunc(*p1) > m_priorFunc(*p2)) {
        // swap p1 and p2 so p1 holds the higher-priority Crop
        Crop* temp = p1;
        p1 = p2;
        p2 = temp;
        temp = nullptr;
      }

      // swap the left and right child of p1
      Crop* temp = p1->m_left;
      p1->m_left = p1->m_right;
      p1->m_right = temp;
      temp = nullptr;

      // recursively call merge on p1's left child and p2; p1 and its right child have been "hung"
      p1->m_left = merge(p1->m_left, p2);

      return p1;
    }
    // SKEW MAX-HEAP
    else if (m_heapType==MAXHEAP) {
      // p1 is lower priority than p2
      if (m_priorFunc(*p1) < m_priorFunc(*p2)) {
        // swap p1 and p2 so p1 holds the higher-priority Crop
        Crop* temp = p1;
        p1 = p2;
        p2 = temp;
        temp = nullptr;
      }

      // swap the left and right child of p1
      Crop* temp = p1->m_left;
      p1->m_left = p1->m_right;
      p1->m_right = temp;
      temp = nullptr;

      // recursively call merge on p1's left child and p2; p1 and its right child have been "hung"
      p1->m_left = merge(p1->m_left, p2);

      return p1;
    }
  }
}

int minNPL (int leftNPL, int rightNPL) {
  if (leftNPL > rightNPL) {
    return rightNPL;
  }
  // this covers if leftNPL is less than rightNPL (return left) or equal to rightNPL (doesn't matter which)
  else {
    return leftNPL;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
Irrigator::Irrigator(int size){
  // check for invalid array size; if so, make invalid (empty) Irrigator
  if (size <= 0) {
    m_capacity = 0;
    m_heap = nullptr;
    m_size = 0;
  }
  else {
    // set total capacity of heap array to parameter
    m_capacity = size;
    // m_heap assigned with address of first element of empty array of Regions sized to parameter
    m_heap = new Region[m_capacity];
    //set size of heap to 0
    //the maximum value of m_size will be m_capacity-ROOTINDEX because index 0 is unused
    m_size = 0;
  }
}
Irrigator::~Irrigator(){
  
}
// Should make deep copy of region parameter and add that to queue
bool Irrigator::addRegion(Region & aRegion){
  //index 0 is unused so array is full when m_size equals m_capacity-ROOTINDEX (1)
  if (m_size>=(m_capacity-ROOTINDEX)) {
    return false;
  }
  else {
    // place aRegion at first empty index beyond last Region
    m_heap[m_size+1] = aRegion;
    int index = m_size+1;
    heapifyIrrigator(m_heap[m_size+1], index);
    return true;
  }
}
// Return Region in parameter
bool Irrigator::getRegion(Region & aRegion){
  
}
// Lowest valid value of n is ROOTINDEX
bool Irrigator::getNthRegion(Region & aRegion, int n){
  
}
void Irrigator::dump(){
    dump(ROOTINDEX);
    cout << endl;
}
void Irrigator::dump(int index){
  if (index <= m_size){
    cout << "(";
    dump(index*2);
    cout << m_heap[index].m_regPrior;
    dump(index*2 + 1);
    cout << ")";
  }
}
// Returns true if Region found with nth priority
bool Irrigator::setPriorityFn(prifn_t priFn, HEAPTYPE heapType, int n){
  
}
// Returns true if Region found with nth priority
bool Irrigator::setStructure(STRUCTURE structure, int n){
  
}
// This will call Region::getNextCrop() for the highest-priority Region currently in the Irrigator
// Returns the Crop in the parameter by deep-copying from result of getNextCrop() into parameter aCrop

// To find highest-priority Region, look at head of Region array and check if empty or not.
// If checked Region is empty, dequeue and remove it
// Otherwise if not empty, keep Region at its place in queue
// Return Crop into parameter aCrop (don't allocate, use stack)

// Regions w/ m_regPrior of 0 (empty) will get bubbled to top because Irrigator is a min-heap
// When getCrop(Crop&) encounters an empty Region on way down to first non-empty Region, it deletes empty Region
// Every time this function runs, it will delete all empty Regions that have bubbled to top
bool Irrigator::getCrop(Crop & aCrop){
  
}

void Irrigator::heapifyIrrigator(Region aRegion, int& index) {
  // base case: current Region and index are root of array (index 1)
  if (index == 1) {
    return;
  }

  // slight variation on parent index formula because the array of Regions leaves index 0 unused
  int parentIndex = ((index-1)/2)+1;

  Region parent = m_heap[parentIndex];
  
  if (parent.m_regPrior > aRegion.m_regPrior) {
    Region temp = parent;
    m_heap[parentIndex] = aRegion;
    m_heap[index] = temp;
    index = parentIndex;
    return heapifyIrrigator(m_heap[index], index);
  }
}

Region* Irrigator::findNthRegion(Region* aRegion, int n) const {
  
}