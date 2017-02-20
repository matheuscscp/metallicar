/*
 * FieldTable.cpp
 *
 *  Created on: Aug 6, 2014
 *      Author: Pimenta
 */

// this
#include "FieldTable.hpp"

using namespace std;

FieldTable::RawFieldBase::~RawFieldBase() {
  
}

FieldTable::Field::Field() : rawField(nullptr) {
  
}

FieldTable::Field::~Field() {
  delete rawField;
}

FieldTable::WriteEvent::WriteEvent(const string& fieldName) :
fieldName(fieldName)
{
  
}

string FieldTable::WriteEvent::field() const {
  return fieldName;
}

observer::Connection FieldTable::connect(
  const string& field,
  const function<void(const observer::EventBase&)>& callback
) {
  return fields[field].second.connect<WriteEvent>(callback);
}
