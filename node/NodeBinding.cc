#include "NodeBinding.h"
#include <vector>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <codecvt>
#include <memory>

IntegrateCrashpad integrateCrashpad;

std::wstring to_wstring(const std::string &input)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(input);
}

bool checkType(const Napi::Env &env, const std::string &type, const std::vector<int> &position, const std::vector<Napi::Value> &values)
{
    if (position.size() != values.size())
    {
        fprintf(stderr, "Wrong number of arguments checker");
        exit(-1);
    }

    for (int i = 0; i < position.size(); i++)
    {
        if (type == "string")
        {
            if (!values[0].IsString())
            {
                Napi::TypeError::New(env, "Wrong Arguments, position" + std::to_string(position[i]) + " expected " + "string" + "type").ThrowAsJavaScriptException();
                return false;
            }
        }
        else if (type == "boolean")
        {
            if (!values[0].IsBoolean())
            {
                Napi::TypeError::New(env, "Wrong Arguments, position" + std::to_string(position[i]) + " expected " + "boolean" + "type").ThrowAsJavaScriptException();
                return false;
            }
        }
    }

    return true;
}

Napi::Value NodeNewIntegrateCrashpadInstance(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2)
    {
        Napi::TypeError::New(env, "Wrong Arguments, expected 2 arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!checkType(env, "string", {0, 1}, {info[0], info[1]}))
    {
        return env.Null();
    }

    integrateCrashpad = std::move(IntegrateCrashpad(to_wstring(info[0].As<Napi::String>().Utf8Value()), to_wstring(info[1].As<Napi::String>().Utf8Value())));
    return env.Null();
}

Napi::Value NodeSetPropertyValue(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2)
    {
        Napi::TypeError::New(env, "Wrong Arguments, expected 2 arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!checkType(env, "string", {0}, {info[0]}))
    {
        return env.Null();
    }

    std::string propertyKey = info[0].As<Napi::String>().Utf8Value();

    if (propertyKey == "productName")
    {
        if (!checkType(env, "string", {1}, {info[1]}))
        {
            return env.Null();
        }
        integrateCrashpad.SetProductName(info[1].As<Napi::String>().Utf8Value());
    }
    else if (propertyKey == "isDevlopement")
    {
        if (!checkType(env, "boolean", {1}, {info[1]}))
        {
            return env.Null();
        }
        integrateCrashpad.SetIsDevlopement(info[1].As<Napi::Boolean>().Value());
    }
    else if (propertyKey == "version")
    {
        if (!checkType(env, "string", {1}, {info[1]}))
        {
            return env.Null();
        }
        integrateCrashpad.SetVersion(info[1].As<Napi::String>().Utf8Value());
    }
    else if (propertyKey == "processType")
    {
        if (!checkType(env, "string", {1}, {info[1]}))
        {
            return env.Null();
        }
        integrateCrashpad.SetProcessType(info[1].As<Napi::String>().Utf8Value());
    }
    else if (propertyKey == "serverURL")
    {
        if (!checkType(env, "string", {1}, {info[1]}))
        {
            return env.Null();
        }
        integrateCrashpad.SetServerURL(info[1].As<Napi::String>().Utf8Value());
    }
    else if (propertyKey == "isUploadToServer")
    {
        if (!checkType(env, "boolean", {1}, {info[1]}))
        {
            return env.Null();
        }
        integrateCrashpad.SetIsUploadToServer(info[1].As<Napi::Boolean>().Value());
    }
    else
    {
        Napi::TypeError::New(env, std::string("Wrong Arguments, property key: ") + propertyKey + std::string(" is not allowed")).ThrowAsJavaScriptException();
        return env.Null();
    }

    return env.Null();
}
Napi::Value NodeAddCrashpadArgs(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong Argument, expected 1 argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!checkType(env, "string", {0}, {info[0]}))
    {
        return env.Null();
    }

    integrateCrashpad.AddCrashpadArgs(info[0].As<Napi::String>().Utf8Value());

    return env.Null();
}
Napi::Value NodeInitializeCrashpad(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    bool isInitialize = integrateCrashpad.InitializeCrashpad();

    return Napi::Boolean::New(env, isInitialize);
}
Napi::Value NodeStartCrashpad(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!integrateCrashpad.GetIsInitialized())
    {
        Napi::Error::New(env, "Cannot start crashpad before it is initialized").ThrowAsJavaScriptException();
        return env.Null();
    }

    bool isStart = integrateCrashpad.Start();

    return Napi::Boolean::New(env, isStart);
}

Napi::Value NodeMkCrash(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    int *i = NULL;
    *i = 1;
    return env.Null();
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "NewIntegrateCrashpadInstance"), Napi::Function::New(env, NodeNewIntegrateCrashpadInstance));
    exports.Set(Napi::String::New(env, "SetPropertyValue"), Napi::Function::New(env, NodeSetPropertyValue));
    exports.Set(Napi::String::New(env, "AddCrashpadArgs"), Napi::Function::New(env, NodeAddCrashpadArgs));
    exports.Set(Napi::String::New(env, "InitializeCrashpad"), Napi::Function::New(env, NodeInitializeCrashpad));
    exports.Set(Napi::String::New(env, "StartCrashpad"), Napi::Function::New(env, NodeStartCrashpad));
    exports.Set(Napi::String::New(env, "MakeCrash"), Napi::Function::New(env, NodeMkCrash));
    return exports;
}

NODE_API_MODULE(CrashpadIntegrate, Initialize)