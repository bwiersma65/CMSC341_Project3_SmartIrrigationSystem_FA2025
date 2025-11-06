// CMSC 341 - Fall 2025 - Project 3
#include "irrigator.h"
Region::Region(){ 
  
}
Region::Region(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure, int regPrior)
{
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
  m_regPrior = regPrior;
}
Region::~Region()
{
  
}
void Region::clear() {
    
}
Region::Region(const Region& rhs)
{
  
}

Region& Region::operator=(const Region& rhs) {
  
}
void Region::mergeWithQueue(Region& rhs) {
  
}

bool Region::insertCrop(const Crop& crop) {
    
}

int Region::numCrops() const
{
  int numCrops = 0;
}
prifn_t Region::getPriorityFn() const {
  
}
Crop Region::getNextCrop() {
    
}
void Region::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
                   
}
void Region::setStructure(STRUCTURE structure){
    
}
STRUCTURE Region::getStructure() const {
  
}
HEAPTYPE Region::getHeapType() const {
  
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

//////////////////////////////////////////////////////////////
Irrigator::Irrigator(int size){
  
}
Irrigator::~Irrigator(){
  
}
bool Irrigator::addRegion(Region & aRegion){
  
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