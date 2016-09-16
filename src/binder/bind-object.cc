// Binds each value in the given object. Each parameter bound with this method
// is considered to be named.
// If an error occurs, error is set to an appropriately descriptive string.
// Regardless of whether an error occurs, the return value is the number of
// parameters that were bound.

int Binder::BindObject(v8::Local<v8::Object> obj, v8::Local<v8::Object> bindMap) {
	// Get array of properties.
	Nan::MaybeLocal<v8::Array> maybeKeys = Nan::GetPropertyNames(obj);
	if (maybeKeys.IsEmpty()) {
		error = "An error was thrown while trying to get the property names of the given object.";
		return 0;
	}
	v8::Local<v8::Array> keys = maybeKeys.ToLocalChecked();
	
	// Get property count.
	unsigned int key_length = keys->Length();
	int len = key_length > 0x7ffffffeU ? 0x7ffffffe : static_cast<int>(key_length);
	int symbol_count = 0;
	
	// Loop through each property.
	for (int i=0; i<len; ++i) {
		
		// Get current property name.
		Nan::MaybeLocal<v8::Value> maybeKey = Nan::Get(keys, i);
		if (maybeKey.IsEmpty()) {
			error = "An error was thrown while trying to get the property names of the given object.";
			return i - symbol_count;
		}
		v8::Local<v8::Value> key = maybeKey.ToLocalChecked();
		
		// If this property is a symbol, ignore it.
		if (key->IsSymbol()) {
			++symbol_count;
			continue;
		}
		
		// Get the parameter index of the current named parameter.
		v8::Local<v8::Value> indexValue = Nan::Get(bindMap, v8::Local<v8::String>::Cast(key)).ToLocalChecked();
		if (indexValue->IsUndefined()) {
			Nan::Utf8String utf8(key);
			error = "The named parameter \"%s\" does not exist.";
			error_extra = new char[utf8.length() + 1];
			strlcpy(error_extra, *utf8, utf8.length() + 1);
			return i - symbol_count;
		}
		int index = (int)(v8::Local<v8::Number>::Cast(indexValue)->Value());
		
		// Get the current property value.
		Nan::MaybeLocal<v8::Value> maybeValue = Nan::Get(obj, key);
		if (maybeValue.IsEmpty()) {
			error = "An error was thrown while trying to get property values of the given object.";
			return i - symbol_count;
		}
		
		// Bind value.
		BindValue(maybeValue.ToLocalChecked(), index);
		if (error) {
			return i - symbol_count;
		}
	}
	
	return len - symbol_count;
}
