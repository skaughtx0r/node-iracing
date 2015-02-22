#include <v8.h>
#include <node.h>
#include "iracingWrapper.h"

#include "../lib/irsdk_defines.h"

void CreateObject(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
  IRacingWrapper::NewInstance(args);
}

void init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module) {
	IRacingWrapper::Init();
	
	NODE_SET_METHOD(module, "exports", CreateObject);
}

NODE_MODULE(iracing, init);
