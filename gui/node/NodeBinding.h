#ifndef __NODE_BINDING_H__
#define __NODE_BINDING_H__

#include <napi.h>
#include "../gui.h"

Napi::Value NodeInitializeApp(const Napi::CallbackInfo &);
Napi::Value NodeRegisterButtonCallback(const Napi::CallbackInfo &);
Napi::Value NodeRenderFrame(const Napi::CallbackInfo &);
Napi::Value NodeReleaseBtnCallback(const Napi::CallbackInfo &);
Napi::Value NodeShowAndUpdateWindow(const Napi::CallbackInfo &);
Napi::Object Initialize(Napi::Env, Napi::Object);

#endif