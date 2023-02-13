#include "NodeBinding.h"
#include <functional>
#include <memory>

bool isAppInitialized = false;

Napi::Value NodeInitializeApp(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (!isAppInitialized)
    {
        isAppInitialized = InitializeApp();

        if (!isAppInitialized)
        {
            Napi::Error::New(env, "Initialize APP failed").ThrowAsJavaScriptException();
        }
    }
    else
    {
        Napi::TypeError::New(env, "Cannot re-initialize an initialized instance").ThrowAsJavaScriptException();
    }

    return Napi::Boolean::New(env, isAppInitialized);
}

Napi::ThreadSafeFunction buttonCbTsfn;
Napi::Value NodeRegisterButtonCallback(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (isAppInitialized)
    {
        Napi::TypeError::New(env, "Cannot re-binding a callback to an initialzied instance").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsFunction())
    {
        Napi::TypeError::New(env, "Wrong Argument, position 0 excepted function").ThrowAsJavaScriptException();
    }

    if (buttonCbTsfn)
    {
        buttonCbTsfn.Abort();
    }

    buttonCbTsfn = Napi::ThreadSafeFunction::New(
        env,
        info[0].As<Napi::Function>(),
        "Button Callback",
        0,
        1,
        (void *)nullptr);

    std::function<void()> btnCallbackWrapper = []() -> void
    {
        auto cppCallback = [](Napi::Env env, Napi::Function jsCallback) -> void
        {
            jsCallback.Call({});
        };

        buttonCbTsfn.BlockingCall(cppCallback);
    };

    RegisterButtonCallback(btnCallbackWrapper);

    return env.Null();
}

Napi::Value NodeRenderFrame(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!isAppInitialized)
    {
        Napi::TypeError::New(env, "Cannot Render an Uninitialized APP").ThrowAsJavaScriptException();
        return env.Null();
    }

    int message = -1;
    RenderFrame(&message);
    return Napi::Number::New(env, static_cast<double>(message));
}

Napi::Value NodeReleaseBtnCallback(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (buttonCbTsfn)
    {
        buttonCbTsfn.Release();
    }
    else
    {
        Napi::TypeError::New(env, "Callback is not registered").ThrowAsJavaScriptException();
    }

    return env.Null();
}

Napi::Value NodeShowAndUpdateWindow(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (!isAppInitialized)
    {
        Napi::TypeError::New(env, "Cannot Show an Uninitialized APP").ThrowAsJavaScriptException();
    }

    ShowAndUpdateWindow();

    return env.Null();
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "InitializeApp"),
        Napi::Function::New(env, NodeInitializeApp));
    exports.Set(
        Napi::String::New(env, "RegisterButtonCallback"),
        Napi::Function::New(env, NodeRegisterButtonCallback));
    exports.Set(
        Napi::String::New(env, "RenderFrame"),
        Napi::Function::New(env, NodeRenderFrame));
    exports.Set(
        Napi::String::New(env, "ReleaseBtnCallback"),
        Napi::Function::New(env, NodeReleaseBtnCallback));
    exports.Set(
        Napi::String::New(env, "ShowAndUpdateWindow"),
        Napi::Function::New(env, NodeShowAndUpdateWindow));
    return exports;
}

NODE_API_MODULE(CrashpadGui, Initialize)