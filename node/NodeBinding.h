#ifndef __NODE_BINDING_H__
#define __NODE_BINDING_H__

#include <napi.h>
#include "../integrateCrashpad.h"
Napi::Value NodeNewIntegrateCrashpadInstance(const Napi::CallbackInfo &);
Napi::Value NodeSetPropertyValue(const Napi::CallbackInfo &);
Napi::Value NodeAddCrashpadArgs(const Napi::CallbackInfo &);
Napi::Value NodeInitializeCrashpad(const Napi::CallbackInfo &);
Napi::Value NodeStartCrashpad(const Napi::CallbackInfo &);
Napi::Object Initialize(Napi::Env, Napi::Object);

#endif