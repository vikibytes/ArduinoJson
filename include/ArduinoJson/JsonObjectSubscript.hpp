// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Configuration.hpp"
#include "JsonVariantBase.hpp"
#include "TypeTraits/ConstRefOrConstPtr.hpp"
#include "TypeTraits/EnableIf.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ArduinoJson {

template <typename TString>
class JsonObjectSubscript
    : public JsonVariantBase<JsonObjectSubscript<TString> > {
  // const String&
  // const std::string&
  // const char*
  typedef typename TypeTraits::ConstRefOrConstPtr<TString>::type TStringRef;

 public:
  FORCE_INLINE JsonObjectSubscript(JsonObject& object, TStringRef key)
      : _object(object), _key(key) {}

  JsonObjectSubscript<TString>& operator=(
      const JsonObjectSubscript<TString>& src) {
    _object.set<const JsonVariant&>(_key, src);
    return *this;
  }

  template <typename T>
  typename TypeTraits::EnableIf<JsonObject::CanSet<T&>::value,
                                JsonObjectSubscript<TString> >::type&
  operator=(const T& src) {
    _object.set<T&>(_key, const_cast<T&>(src));
    return *this;
  }

  template <typename T>
  typename TypeTraits::EnableIf<JsonObject::CanSet<T>::value,
                                JsonObjectSubscript<TString> >::type&
  operator=(T src) {
    _object.set<T>(_key, src);
    return *this;
  }

  FORCE_INLINE bool success() const {
    return _object.containsKey(_key);
  }

  FORCE_INLINE operator JsonVariant() const {
    return _object.get(_key);
  }

  template <typename TValue>
  FORCE_INLINE typename Internals::JsonVariantAs<TValue>::type as() const {
    return _object.get<TValue, TStringRef>(_key);
  }

  template <typename TValue>
  FORCE_INLINE bool is() const {
    return _object.is<TValue>(_key);
  }

  template <typename TValue>
  FORCE_INLINE bool set(TValue value) {
    return _object.set<TValue>(_key, value);
  }

  template <typename TValue>
  FORCE_INLINE bool set(TValue value, uint8_t decimals) {
    return _object.set(_key, value, decimals);
  }

  FORCE_INLINE JsonVariant get() {
    return _object.get(_key);
  }

 private:
  JsonObject& _object;
  TStringRef _key;
};

#if ARDUINOJSON_ENABLE_STD_STREAM
template <typename TString>
inline std::ostream& operator<<(std::ostream& os,
                                const JsonObjectSubscript<TString>& source) {
  return source.printTo(os);
}
#endif

template <typename TString>
inline JsonObjectSubscript<TString> JsonObject::operator[](const TString& key) {
  return JsonObjectSubscript<TString>(*this, key);
}

template <typename TImplem>
template <class TString>
inline const JsonObjectSubscript<TString> JsonVariantBase<TImplem>::operator[](
    const TString& key) const {
  return asObject()[key];
}
}  // namespace ArduinoJson

#ifdef _MSC_VER
#pragma warning(pop)
#endif
