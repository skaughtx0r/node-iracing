#include <v8.h>
#include <node.h>

#include "yaml_parser.h"
#include "irsdk_defines.h"

using namespace v8;

class IRacing: node::ObjectWrap {
private:
public:
  IRacing() {}
  ~IRacing() {}

  static v8::Persistent<FunctionTemplate> pft;

  static void Init(Handle<Object> target) {
    v8::HandleScope scope;

    v8::Local<FunctionTemplate> lft = v8::FunctionTemplate::New(New);
    
    IRacing::pft = v8::Persistent<FunctionTemplate>::New(lft);
    IRacing::pft->InstanceTemplate()->SetInternalFieldCount(1);
    IRacing::pft->SetClassName(v8::String::NewSymbol("IRacing"));

    target->Set(String::NewSymbol("iracing"), IRacing::pft->GetFunction());
  }

  static Handle<Value> New(const Arguments& args) {
    HandleScope scope;

    IRacing* ir = new IRacing();
    ir->Wrap(args.This());
    
    return args.This();
  }
};

v8::Persistent<FunctionTemplate> IRacing::pft;
extern "C" {
  static void init (Handle<Object> target) {
    IRacing::Init(target);
  }

  NODE_MODULE(iracing, init);
}