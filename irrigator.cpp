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

  if ((heapType == NOTYPE) || (structure == NOSTRUCT) || (regPrior == 0)) {
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
Region::~Region()
{
  
}
// FIX ME
void Region::clear() {
  delete m_heap;
  m_size = 0;
  m_priorFunc = nullptr;
  m_heapType = NOTYPE;
  m_structure = NOSTRUCT;
  m_regPrior = 0;    
}
Region::Region(const Region& rhs)
{
  
}

Region& Region::operator=(const Region& rhs) {
  
}
void Region::mergeWithQueue(Region& rhs) {
  
}

bool Region::insertCrop(const Crop& crop) {
  // if any of Region members are set to invalid values, do not add Crop to Region
  if ((m_priorFunc==nullptr) || (m_heapType==NOTYPE) || (m_structure==NOSTRUCT) || (m_regPrior <= 0)) {
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
Crop Region::getNextCrop() {
    
}
void Region::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
                   
}
void Region::setStructure(STRUCTURE structure){
    STRUCTURE prev = m_structure;
    STRUCTURE next = structure;

    if ((prev==SKEW) && (next==LEFTIST)) {

    }
    else if ((prev==NOSTRUCT) && (next==SKEW)) {

    }
    else if ((prev==NOSTRUCT) && (next==LEFTIST)) {

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

void Region::merge(Crop crop) {
  if (m_structure==LEFTIST) {

  }
  else if (m_structure==SKEW) {

  }
}

void Region::swap() {

}

//////////////////////////////////////////////////////////////
Irrigator::Irrigator(int size){
  // set total capacity of heap to parameter
  m_capacity = size;
  // m_heap assigned with address of first element of empty array of Regions sized to parameter
  Region array[m_capacity];
  m_heap = array;
  // m_heap = new Region[m_capacity];
  // set size of heap to 0
  m_size = 0;
}
Irrigator::~Irrigator(){
  
}
bool Irrigator::addRegion(Region & aRegion){
  if (m_capacity==m_size) {
    return false;
  }
  else {
    // place aRegion at end of array (this assumes first Region at index 0)
    m_heap[m_size] = aRegion;
    int index = m_size;
    heapifyIrrigator(m_heap[m_size], index);
    return true;
  }
}

bool Irrigator::getRegion(Region & aRegion){
  
}

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
bool Irrigator::setPriorityFn(prifn_t priFn, HEAPTYPE heapType, int n){
  
}

bool Irrigator::setStructure(STRUCTURE structure, int n){
  
}
bool Irrigator::getCrop(Crop & aCrop){
  
}

void Irrigator::heapifyIrrigator(Region aRegion, int& index) {
  int parentIndex = (index-1)/2;
  // base case: current Region and index are root of array (index 0)
  if (index == 0) {
    return;
  }
  Region parent = m_heap[parentIndex];
  
  if (parent.m_regPrior < aRegion.m_regPrior) {
    Region temp = parent;
    m_heap[parentIndex] = aRegion;
    m_heap[index] = temp;
    index = parentIndex;
    return heapifyIrrigator(m_heap[index], index);
  }
}

Region* Irrigator::findNthRegion(Region* aRegion, int n) const {
  
}