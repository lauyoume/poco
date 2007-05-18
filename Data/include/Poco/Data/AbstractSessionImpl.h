//
// AbstractSessionImpl.h
//
// $Id: //poco/Main/Data/include/Poco/Data/AbstractSessionImpl.h#4 $
//
// Library: Data
// Package: DataCore
// Module:  AbstractSessionImpl
//
// Definition of the AbstractSessionImpl class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Data_AbstractSessionImpl_INCLUDED
#define Data_AbstractSessionImpl_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/SessionImpl.h"
#include "Poco/Data/DataException.h"
#include <map>


namespace Poco {
namespace Data {


template <class C>
class AbstractSessionImpl: public SessionImpl
	/// A partial implementation of SessionImpl, providing
	/// features and properties management.
	///
	/// To implement a certain feature or property, a subclass
	/// must provide setter and getter methods and register
	/// them with addFeature() or addProperty().
{
public:
	typedef void (C::*FeatureSetter)(const std::string&, bool);
		/// The setter method for a feature.
		
	typedef bool (C::*FeatureGetter)(const std::string&);
		/// The getter method for a feature.
		
	typedef void (C::*PropertySetter)(const std::string&, const Poco::Any&);
		/// The setter method for a property.
		
	typedef Poco::Any (C::*PropertyGetter)(const std::string&);
		/// The getter method for a property.

	AbstractSessionImpl()
		/// Creates the AbstractSessionImpl.
	{
	}

	~AbstractSessionImpl()
		/// Destroys the AbstractSessionImpl.
	{
	}

	void setFeature(const std::string& name, bool state)
		/// Looks a feature up in the features map
		/// and calls the feature's setter, if there is one.
	{
		typename FeatureMap::const_iterator it = _features.find(name);
		if (it != _features.end())
		{
			if (it->second.setter)
				(static_cast<C*>(this)->*it->second.setter)(name, state);
			else
				throw NotImplementedException("set", name);
		}
		else throw NotSupportedException(name);
	}
	
	bool getFeature(const std::string& name)
		/// Looks a feature up in the features map
		/// and calls the feature's getter, if there is one.
	{
		typename FeatureMap::const_iterator it = _features.find(name);
		if (it != _features.end())
		{
			if (it->second.getter)
				return (static_cast<C*>(this)->*it->second.getter)(name);
			else
				throw NotImplementedException("get", name);
		}
		else throw NotSupportedException(name);
	}
	
	void setProperty(const std::string& name, const Poco::Any& value)
		/// Looks a property up in the properties map
		/// and calls the property's setter, if there is one.
	{
		typename PropertyMap::const_iterator it = _properties.find(name);
		if (it != _properties.end())
		{
			if (it->second.setter)
				(static_cast<C*>(this)->*it->second.setter)(name, value);
			else
				throw NotImplementedException("set", name);
		}
		else throw NotSupportedException(name);
	}

	Poco::Any getProperty(const std::string& name)
		/// Looks a property up in the properties map
		/// and calls the property's getter, if there is one.
	{
		typename PropertyMap::const_iterator it = _properties.find(name);
		if (it != _properties.end())
		{
			if (it->second.getter)
				return (static_cast<C*>(this)->*it->second.getter)(name);
			else
				throw NotImplementedException("set", name);
		}
		else throw NotSupportedException(name);
	}
	
protected:
	void addFeature(const std::string& name, FeatureSetter setter, FeatureGetter getter)
		/// Adds a feature to the map of supported features.
		///
		/// The setter or getter can be null, in case setting or getting a feature
		/// is not supported.
	{
		Feature feature;
		feature.setter = setter;
		feature.getter = getter;
		_features[name] = feature;
	}
		
	void addProperty(const std::string& name, PropertySetter setter, PropertyGetter getter)
		/// Adds a property to the map of supported properties.
		///
		/// The setter or getter can be null, in case setting or getting a property
		/// is not supported.
	{
		Property property;
		property.setter = setter;
		property.getter = getter;
		_properties[name] = property;
	}

private:
	struct Feature
	{
		FeatureSetter setter;
		FeatureGetter getter;
	};
	
	struct Property
	{
		PropertySetter setter;
		PropertyGetter getter;
	};
	
	typedef std::map<std::string, Feature>  FeatureMap;
	typedef std::map<std::string, Property> PropertyMap;
	
	FeatureMap  _features;
	PropertyMap _properties;
};


} } // namespace Poco::Data


#endif // Data_AbstractSessionImpl_INCLUDED