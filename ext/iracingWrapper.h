#ifndef IRACINGWRAPPER_H
#define IRACINGWRAPPER_H

#include <node.h>
#include <node_object_wrap.h>
#include "../lib/irsdk_defines.h"

class IRacingWrapper : public node::ObjectWrap {
	public:
		static void Init();
		static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);

	private:
		explicit IRacingWrapper();
		~IRacingWrapper();

		static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void WaitForDataReady(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void GetHeader(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void GetSessionYAML(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void GetTelemetry(const v8::FunctionCallbackInfo<v8::Value>& args);
		static v8::Persistent<v8::Function> constructor;
		char* data;
		int dataLen;
};

#endif