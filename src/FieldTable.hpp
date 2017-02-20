/*
 * FieldTable.hpp
 *
 *  Created on: Aug 6, 2014
 *      Author: Pimenta
 */

#ifndef FIELDTABLE_HPP_
#define FIELDTABLE_HPP_

// standard
#include <map>
#include <string>

// local
#include "observer.hpp"

class FieldTable {
  public:
    struct RawFieldBase {
      virtual ~RawFieldBase();
    };
    
    template <typename T>
    struct RawField : public RawFieldBase {
      T raw;
      RawField(const T& value) : raw(value) {
        
      }
    };
  private:
    class Field {
      private:
        RawFieldBase* rawField;
      public:
        Field();
        ~Field();
        
        template <typename T> T read() {
          T ret = T();
          if (rawField) {
            ret = ((RawField<T>*)rawField)->raw;
          }
          return ret;
        }
        
        template <typename T> void write(const T& value) {
          delete rawField;
          rawField = new RawField<T>(value);
        }
    };
  public:
    class WriteEvent : public observer::Event<WriteEvent> {
      private:
        std::string fieldName;
      public:
        WriteEvent(const std::string& fieldName);
        std::string field() const;
    };
  private:
    std::map<std::string, std::pair<Field, observer::Subject>> fields;
  public:
    template <typename T> T read(const std::string& field) {
      return fields[field].first.read<T>();
    }
    
    template <typename T> void write(const std::string& field, const T& value) {
      auto& fieldPair = fields[field];
      fieldPair.first.write(value);
      fieldPair.second.broadcast(WriteEvent(field));
    }
    
    observer::Connection connect(
      const std::string& field,
      const std::function<void(const observer::EventBase&)>& callback
    );
};

#endif /* FIELDTABLE_HPP_ */
