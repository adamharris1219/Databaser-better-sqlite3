void Binder::BindValue(v8::Local<v8::Value> value, int index = 0) {
	if (value->IsNumber()) {
		BindNumber(v8::Local<v8::Number>::Cast(value), index);
	} else if (value->IsString()) {
		BindString(v8::Local<v8::String>::Cast(value), index);
	} else if (value->IsNull() || value->IsUndefined()) {
		BindNull(index);
	} else if (node::Buffer::HasInstance(value)) {
		BindBuffer(v8::Local<v8::Object>::Cast(value), index);
	} else {
		error = "SQLite3 can only bind numbers, strings, Buffers, and null.";
	}
}
