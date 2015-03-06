#include <node.h>
#include <node_object_wrap.h>
#include "iracingWrapper.h"

v8::Persistent<v8::Function> IRacingWrapper::constructor;

IRacingWrapper::IRacingWrapper() : data(NULL), dataLen(0){
}

IRacingWrapper::~IRacingWrapper() {
}

void IRacingWrapper::Init() {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, "IRacingWrapper"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	
	// Methods
    NODE_SET_PROTOTYPE_METHOD(tpl, "waitForDataReady", WaitForDataReady);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getHeader", GetHeader);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getSessionYAML", GetSessionYAML);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getTelemetry", GetTelemetry);
	
	constructor.Reset(isolate, tpl->GetFunction());
	//exports->Set(v8::String::NewFromUtf8(isolate, "IRacingWrapper"), tpl->GetFunction());
}

void IRacingWrapper::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);
	
	if(args.IsConstructCall()) {
		IRacingWrapper* obj = new IRacingWrapper();
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	}
	else {
		v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
		args.GetReturnValue().Set(cons->NewInstance());
	}
}

void IRacingWrapper::NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);

	v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
	v8::Local<v8::Object> instance = cons->NewInstance();

	args.GetReturnValue().Set(instance);
}

void IRacingWrapper::WaitForDataReady(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);
	
	IRacingWrapper* obj = node::ObjectWrap::Unwrap<IRacingWrapper>(args.Holder());

    bool result = irsdk_waitForDataReady(args[0]->Int32Value(), NULL);

    if (result) {
      const irsdk_header *header = irsdk_getHeader();

       if (!obj->data || obj->dataLen != header->bufLen) {
         if (obj->data)
           delete [] obj->data;

         obj->dataLen = header->bufLen;
         obj->data = new char[obj->dataLen];

         irsdk_waitForDataReady(args[0]->Int32Value(), obj->data);  
      }
    }

	args.GetReturnValue().Set(v8::Boolean::New(isolate, result));
}

void IRacingWrapper::GetHeader(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);

    const irsdk_header *header = irsdk_getHeader();

    v8::Handle<v8::Object> obj = v8::Object::New(isolate);
    obj->Set(v8::String::NewFromUtf8(isolate, "ver"), v8::Int32::New(isolate, header->ver));
    obj->Set(v8::String::NewFromUtf8(isolate, "status"), v8::Int32::New(isolate, header->status));
    obj->Set(v8::String::NewFromUtf8(isolate, "tickRate"), v8::Int32::New(isolate, header->tickRate));

    args.GetReturnValue().Set(obj);
}

void IRacingWrapper::GetSessionYAML(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);

	args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, irsdk_getSessionInfoStr()));
}

void IRacingWrapper::GetTelemetry(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);
	
	IRacingWrapper* obj = node::ObjectWrap::Unwrap<IRacingWrapper>(args.Holder());

    v8::String::Utf8Value propName(args[0]->ToString());
    int offset = irsdk_varNameToOffset(*propName);

    if ( // First, the ints
      !strcmp(*propName, "CamCameraNumber") ||
      !strcmp(*propName, "CamCarIdx") ||
      !strcmp(*propName, "CamGroupNumber") ||
      !strcmp(*propName, "RaceLaps") ||
      !strcmp(*propName, "ReplayFrameNum") ||
      !strcmp(*propName, "ReplayFrameNumEnd") ||
      !strcmp(*propName, "ReplayPlaySpeed") ||
      !strcmp(*propName, "ReplaySessionNum") ||
      !strcmp(*propName, "Gear") ||
      !strcmp(*propName, "Lap") ||
      !strcmp(*propName, "LapBestLap") ||
      !strcmp(*propName, "SessionLapsRemain") ||
      !strcmp(*propName, "SessionNum") ||
      !strcmp(*propName, "SessionState") ||
      !strcmp(*propName, "SessionUniqueID")
      ) {
		args.GetReturnValue().Set(v8::Number::New(isolate, *((int *)(obj->data + offset))));
		return;

    } else if (
      !strcmp(*propName, "Brake") ||
      !strcmp(*propName, "Clutch") ||
      !strcmp(*propName, "FuelLevel") ||
      !strcmp(*propName, "FuelLevelPct") ||
      !strcmp(*propName, "FuelPress") ||
      !strcmp(*propName, "LapBestLapTime") ||
      !strcmp(*propName, "LapCurrentLapTime") ||
      !strcmp(*propName, "LapDeltaToBestLap") ||
      !strcmp(*propName, "LapDeltaToBestLap_DD") ||
      !strcmp(*propName, "LapDeltaToOptimalLap") ||
      !strcmp(*propName, "LapDeltaToOptimalLap_DD") ||
      !strcmp(*propName, "LapDeltaToSessionBestLap") ||
      !strcmp(*propName, "LapDeltaToSessionBestLap_DD") ||
      !strcmp(*propName, "LapDeltaToSessionOptimalLap") ||
      !strcmp(*propName, "LapDeltaToSessionOptimalLap_DD") ||
      !strcmp(*propName, "LapDist") ||
      !strcmp(*propName, "LapDistPct") ||
      !strcmp(*propName, "LapLastLapTime") ||
      !strcmp(*propName, "LatAccel") ||
      !strcmp(*propName, "LFshockDefl") ||
      !strcmp(*propName, "LongAccel") ||
      !strcmp(*propName, "LRshockDefl") ||
      !strcmp(*propName, "ManifoldPress") ||
      !strcmp(*propName, "OilLevel") ||
      !strcmp(*propName, "OilPress") ||
      !strcmp(*propName, "OilTemp") ||
      !strcmp(*propName, "Pitch") ||
      !strcmp(*propName, "PitchRate") ||
      !strcmp(*propName, "PitOptRepairLeft") ||
      !strcmp(*propName, "PitRepairLeft") ||
      !strcmp(*propName, "RFshockDefl") ||
      !strcmp(*propName, "Roll") ||
      !strcmp(*propName, "RollRate") ||
      !strcmp(*propName, "RPM") ||
      !strcmp(*propName, "RRshockDefl") ||
      !strcmp(*propName, "ShiftGrindRPM") ||
      !strcmp(*propName, "ShiftIndicatorPct") ||
      !strcmp(*propName, "ShiftPowerPct") ||
      !strcmp(*propName, "Speed") ||
      !strcmp(*propName, "SteeringWheelAngle") ||
      !strcmp(*propName, "SteeringWheelPctTorque") ||
      !strcmp(*propName, "SteeringWheelTorque") ||
      !strcmp(*propName, "Throttle") ||
      !strcmp(*propName, "VelocityX") ||
      !strcmp(*propName, "VelocityY") ||
      !strcmp(*propName, "VelocityZ") ||
      !strcmp(*propName, "VertAccel") ||
      !strcmp(*propName, "Voltage") ||
      !strcmp(*propName, "WaterLevel") ||
      !strcmp(*propName, "WaterTemp") ||
      !strcmp(*propName, "Yaw") ||
      !strcmp(*propName, "YawRate") ||
      !strcmp(*propName, "Alt") ||
      !strcmp(*propName, "LFpressure") ||
      !strcmp(*propName, "LFrideHeight") ||
      !strcmp(*propName, "LFspeed") ||
      !strcmp(*propName, "LFtempL") ||
      !strcmp(*propName, "LFtempM") ||
      !strcmp(*propName, "LFtempR") ||
      !strcmp(*propName, "LRpressure") ||
      !strcmp(*propName, "LRrideHeight") ||
      !strcmp(*propName, "LRspeed") ||
      !strcmp(*propName, "LRtempL") ||
      !strcmp(*propName, "LRtempM") ||
      !strcmp(*propName, "LRtempR") ||
      !strcmp(*propName, "RFpressure") ||
      !strcmp(*propName, "RFrideHeight") ||
      !strcmp(*propName, "RFspeed") ||
      !strcmp(*propName, "RFtempL") ||
      !strcmp(*propName, "RFtempM") ||
      !strcmp(*propName, "RFtempR") ||
      !strcmp(*propName, "RRpressure") ||
      !strcmp(*propName, "RRrideHeight") ||
      !strcmp(*propName, "RRspeed") ||
      !strcmp(*propName, "RRtempL") ||
      !strcmp(*propName, "RRtempM") ||
      !strcmp(*propName, "RRtempR")
      ) {
		args.GetReturnValue().Set(v8::Number::New(isolate, *((float *)(obj->data + offset))));
		return;

    } else if (
      !strcmp(*propName, "ReplaySessionTime") ||
      !strcmp(*propName, "SessionTime") ||
      !strcmp(*propName, "SessionTimeRemain") ||
      !strcmp(*propName, "Lat") ||
      !strcmp(*propName, "Lon")
      ) {

		args.GetReturnValue().Set(v8::Number::New(isolate, *((double *)(obj->data + offset))));
		return;

    } else if (
      !strcmp(*propName, "IsInGarage") ||
      !strcmp(*propName, "IsReplayPlaying") ||
      !strcmp(*propName, "ReplayPlaySlowMotion") ||
      !strcmp(*propName, "DriverMarker") ||
      !strcmp(*propName, "IsOnTrack") ||
      !strcmp(*propName, "LapDeltaToOptimalLap_OK") ||
      !strcmp(*propName, "LapDeltaToSessionBestLap_OK") ||
      !strcmp(*propName, "LapDeltaToSessionOptimalLap_OK") ||
      !strcmp(*propName, "OnPitRoad")
      ) {

		args.GetReturnValue().Set(v8::Boolean::New(isolate, *((bool *)(obj->data + offset))));
		return;

    }
}