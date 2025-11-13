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

  if ((heapType == NOTYPE) || (structure == NOSTRUCT) || (regPrior <= 0)) {
 // if ((heapType == NOTYPE) || (structure == NOSTRUCT)) {
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
  clear();
}

// Clears the queue; delete all nodes in heap leaving it empty, and re-initialize member variables
void Region::clear() {
  // Recursive helper; postorder traversal of heap, deleting from bottom up
  clearSubheap(m_heap);

  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = nullptr;
  m_heapType = NOTYPE;
  m_structure = NOSTRUCT;
  m_regPrior = 0;    
}

/*
Copy constructor
Takes deep-copies of parameter Region's members including its heap
Builds this object's heap by copying rhs heap in pre-order fashion
*/
// Edge case: copy empty object (rhs.m_heap is empty)
Region::Region(const Region& rhs)
{
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;
  m_regPrior = rhs.m_regPrior;

  m_heap = copyHeap(rhs.m_heap);
}

/*
Overloaded assignment operator
Clears this Region's heap
Sets this Region's non-object members to values of parameter's non-object members
Deep-copies heap of parameter into this Region's heap
Guards against self-assignment; if attempted, returns this Region without any changes
*/
// Edge case: copy empty object (rhs is empty)
Region& Region::operator=(const Region& rhs) {
  // checks against self-copying; if both heap roots are the same address, they are the same Region
  if (&rhs != this) {
    // Delete heap and its Crop nodes
    this->clear();
    // Copy non-object members
    m_size = rhs.m_size;
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_regPrior = rhs.m_regPrior;
    // Copy heap by calling recursive helper
    m_heap = copyHeap(rhs.m_heap);
  }

  return *this;
}

void Region::mergeWithQueue(Region& rhs) {
  // checks against self-merging
  if (&rhs == this) {
    return;
  }
  // if both Regions do not match in structure or heap type, do not merge
  if ((rhs.m_structure != m_structure) || (rhs.m_heapType != m_heapType) || (rhs.m_priorFunc != m_priorFunc)) {
    throw domain_error("Mismatched heaptype or structure");
  }
  // Checks if rhs priority is valid
  if (rhs.m_regPrior < 0) {
    return;
  }
  // Calls recursive merge helper to merge both heaps and assign to this Region's m_heap member
  m_heap = merge(m_heap, rhs.m_heap);
  // Clears the heap of the rhs Region after merging
  rhs.m_heap = nullptr;
}

/*
This function will deep-copy the Crop parameter and insert the copy into the Region's heap
If the Region has any member values indicating emptiness, returns false before trying
to insert a Crop
If the Crop's priority value is invalid, returns false before trying to insert a Crop
*/
bool Region::insertCrop(const Crop& crop) {
  // if any of Region members are set to invalid values, do not add Crop to Region
  if ((m_priorFunc==nullptr) || (m_heapType==NOTYPE) || (m_structure==NOSTRUCT) || (m_regPrior <= 0)) {
    return false;
  }
  else if (m_priorFunc(crop) <= 0) {
    return false;
  }
  else {
    Crop* temp = new Crop(crop.m_cropID, crop.m_temperature, crop.m_moisture, crop.m_time, crop.m_type);
    m_heap = merge(m_heap, temp);
    m_size++;
    return true;
  }
}

/*
uses private helper countCrops to recursively preorder traverse heap and increment counter for each non-null node
returns number of Crops in heap
*/
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
// subheaps together, and return the removed Crop as an object
// If the heap is empty when this function is called, throw an out_of_range exception
Crop Region::getNextCrop() {
  // Check if the heap is empty
  if (m_heap==nullptr) {
    throw out_of_range("Crop queue is empty in this Region");
  }
  else {
    // store highest priority Crop in temp holder
    Crop* temp = m_heap;
    // merge left and right subheaps and store as new m_heap
    m_heap = merge(m_heap->m_left, m_heap->m_right);
    // unlink highest priority Crop from former children
    temp->m_left = nullptr;
    temp->m_right = nullptr;
    temp->m_npl = 0;
    // Return dereferenced Crop object
    return *temp;
  }
}

// Change the m_priorFunc and heapType members for the object to values of respective parameters
// Rebuild the heap using the new values (by using recursive rebuild helper and merge helper)
// Do not reallocate new memory, just reuse the pre-existing nodes
// Don't need to check that prifn is nullptr; make sure heapType is valid
// No need to verify compatibility of priFn and heapType
// Should operate normally for empty and non-empty Region heaps
void Region::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  // If NOTYPE passed as argument, clear the heap and set variables to reflect empty Region
  if (heapType==NOTYPE) {
    this->clear();
    return;
  }
  else {
    // Change heaptype and priority function to parameter values
    m_heapType = heapType;
    m_priorFunc = priFn;

    // Place heap in temp holder, clear pointer to heap root
    Crop* temp = m_heap;
    m_heap = nullptr;
    
    /// Call recursive helper to rebuild heap from temp holder into m_heap member
    m_heap = rebuildHeap(temp, m_heap);
  }
}

// Change the m_structure member for the object to value of parameter
// if it differs from the previous value and requires rebuilding, rebuild the heap using the new value
// Do not reallocate new memory, just reuse the pre-existing nodes
void Region::setStructure(STRUCTURE structure){
  // If NOSTRUCT is passed as argument, clear the heap and set variables to reflect empty Region
  if (structure==NOSTRUCT) {
    this->clear();
    return;
  }
  else {
    STRUCTURE prev = m_structure;
    m_structure = structure;

    if ((prev==SKEW) && (m_structure==LEFTIST)) {
      Crop* temp = m_heap;
      m_heap = nullptr;
      m_heap = rebuildHeap(temp, m_heap);
    }
    // else if ((prev==NOSTRUCT) && (m_structure==SKEW)) {
      
    // }
    // else if ((prev==NOSTRUCT) && (m_structure==LEFTIST)) {

    // }
  }
}

STRUCTURE Region::getStructure() const {
  return m_structure;
}
HEAPTYPE Region::getHeapType() const {
  return m_heapType;
}

void Region::printCropsQueue() const {
  printCrop(m_heap);
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

    /****************************************
     * Helper function definitions go here! *
     ****************************************/

/*

*/
void Region::printCrop(Crop* node) const {
  if (node==nullptr) return;

  cout << "[" << m_priorFunc(*node) << "] " << *node << endl;

  printCrop(node->m_left);

  printCrop(node->m_right);
}

/*
Traverses the heap in pre-order fashion, incrementing for every non-null node found
The counter is returned in the int& parameter
*/
void Region::countCrops(Crop* node, int& numCrops) const {
  // base case
  if (node == nullptr) {
    return;
  }

  numCrops++;

  countCrops(node->m_left, numCrops);

  countCrops(node->m_right, numCrops);

  return;
}

/*
Merges the heaps rooted at p1 and p2 recursively
According to structure (Leftist or Skew) and heaptype (Min or Max)
If either parameter heap is empty return the other non-empty
If both parameters are empty heaps, return nullptr
*/
Crop* Region::merge(Crop* p1, Crop* p2) {
  // base case
  // check if either heap to be merged is empty; if so, result is just non-empty heap
  if (p1==nullptr) return p2;
  else if (p2==nullptr) return p1;

  if (m_structure==LEFTIST) {
    ///////////////////////////////////////////////////////////////////////////////////
    // LEFTIST MIN-HEAP //
    if (m_heapType==MINHEAP) {
      // p1 is lower priority than p2
      if (m_priorFunc(*p1) > m_priorFunc(*p2)) {
        // swap p1 and p2 so p1 holds the higher-priority Crop
        Crop* temp = p1;
        p1 = p2;
        p2 = temp;
        temp = nullptr;
      }

      // recursively call merge on p1's right child and p2; p1 and its left child have been "hung"
      p1->m_right = merge(p1->m_right, p2);

      // update p1's npl after zipping up its right child
      p1->m_npl = 1 + minNPL(p1);

      // left child NPL must be greater than or equal to right child NPL
      // if this property not satisfied, swap the children
      if (!checkNPL(p1)) {
        Crop* temp = p1->m_left;
        p1->m_left = p1->m_right;
        p1->m_right = temp;
        temp = nullptr;
      }

      return p1;
    }
    ///////////////////////////////////////////////////////////////////////////////////
    // LEFTIST MAX-HEAP //
    else if (m_heapType==MAXHEAP) {
      // p1 is lower priority than p2
      if (m_priorFunc(*p1) < m_priorFunc(*p2)) {
        // swap p1 and p2 so p1 holds the higher-priority Crop
        Crop* temp = p1;
        p1 = p2;
        p2 = temp;
        temp = nullptr;
      }

      // recursively call merge on p1's right child and p2; p1 and its left child have been "hung"
      p1->m_right = merge(p1->m_right, p2);

      // update p1's npl after zipping up its right child
      p1->m_npl = 1 + minNPL(p1);

      // left child NPL must be greater than or equal to right child NPL
      // if this property not satisfied, swap the children
      if (!checkNPL(p1)) {
        Crop* temp = p1->m_left;
        p1->m_left = p1->m_right;
        p1->m_right = temp;
        temp = nullptr;
      }

      return p1;
    }

  }
  else if (m_structure==SKEW) {
    ///////////////////////////////////////////////////////////////////////////////////
    // SKEW MIN-HEAP //
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

      // update p1's npl after zipping up its right child
      p1->m_npl = 1 + minNPL(p1);

      return p1;
    }
    ///////////////////////////////////////////////////////////////////////////////////
    // SKEW MAX-HEAP //
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

      // update p1's npl after zipping up its right child
      p1->m_npl = 1 + minNPL(p1);

      return p1;
    }
  }

  // If this Region is of type NOSTRUCT or NOTYPE, return a nullptr
  return nullptr;
}

/*
Finds minimum NPL value between parameter's children
If parameter's children are empty/null, treat their NPL as -1
*/
int Region::minNPL (Crop* node) const {
  int leftNPL, rightNPL;

  // check for empty left child
  if (node->m_left==nullptr) {
    leftNPL = -1;
  }
  else {
    leftNPL = node->m_left->m_npl;
  }
  // check for empty right child
  if (node->m_right==nullptr) {
    rightNPL = -1;
  }
  else {
    rightNPL = node->m_right->m_npl;
  }

  // find minimum npl between children
  if (leftNPL > rightNPL) {
    return rightNPL;
  }
  // this covers if leftNPL is less than rightNPL (return left) or equal to rightNPL (doesn't matter which)
  else {
    return leftNPL;
  }
}

/*
Checks the NPL values of the parameter's children
If the left child NPL is less than the right child NPL, Leftist property is not satisfied; return false
Otherwise, return true
*/
bool Region::checkNPL(Crop* node) const {
  int leftNPL, rightNPL;

  if (node->m_left==nullptr) {
    leftNPL = -1;
  }
  else {
    leftNPL = node->m_left->m_npl;
  }

  if (node->m_right==nullptr) {
    rightNPL = -1;
  }
  else {
    rightNPL = node->m_right->m_npl;
  }

  if (leftNPL < rightNPL) {
    return false;
  }
  else {
    return true;
  }
}

/*
Helper for copy constructor
Traverses tree rooted at parameter in pre-order fashion recursively
Ultimately returns root of deep-copied tree
*/
Crop* Region::copyHeap(Crop* root) const {
  // base case
  if (root==nullptr) {
    return nullptr;
  }
  // create newly-allocated copy of parameter Crop
  // initializes members to parameter member values, right and left children to nullptr, and NPL to 0
  Crop* newRoot = new Crop(root->m_cropID, root->m_temperature, root->m_moisture, root->m_time, root->m_type);
  // because Crop parameterized constructor initializes m_npl to 0, must change to match value of root's m_npl
  newRoot->m_npl = root->m_npl;

  // recursive call to copy parameter's left child
  newRoot->m_left = copyHeap(root->m_left);

  // recursive call to copy parameter's right child
  newRoot->m_right = copyHeap(root->m_right);

  // return deep-copied root
  return newRoot;
}

/*
Helper for rebuilding heap
Postorder traversal of oldHeap parameter, populating rebuiltHeap parameter by merging
Does not allocate new memory
*/
Crop* Region::rebuildHeap(Crop* oldHeap, Crop* rebuiltHeap) {
  // base case
  if (oldHeap == nullptr) {
    return rebuiltHeap;
  }

  // Pluck and store child pointers from parameter before clearing their fields and NPL
  Crop* left = oldHeap->m_left;
  Crop* right = oldHeap->m_right;
  oldHeap->m_left = nullptr;
  oldHeap->m_right = nullptr;
  oldHeap->m_npl = 0;


  rebuiltHeap = rebuildHeap(left, rebuiltHeap);

  rebuiltHeap = rebuildHeap(right, rebuiltHeap);

  return merge(oldHeap, rebuiltHeap);
}

/*
Helper to clear queue
Postorder traversal of heap to delete node children before parents
*/
void Region::clearSubheap(Crop* node) {
  if (node == nullptr) {
    return;
  }

  clearSubheap(node->m_left);

  clearSubheap(node->m_right);

  delete node;
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