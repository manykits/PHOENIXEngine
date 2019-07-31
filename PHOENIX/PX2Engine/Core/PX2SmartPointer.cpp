// PX2SmartPointer.cpp

#include "PX2SmartPointer.hpp"
using namespace PX2;

PointerBase::ReferenceMap PointerBase::msMap;
Mutex PointerBase::msMutex;