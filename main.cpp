#include <iostream>
#include <emscripten.h>
#include <string>
#include <cassert>

#include <angelscript.h>
#include <angelscript/scriptstdstring/scriptstdstring.h>
#include <angelscript/scriptarray/scriptarray.h>
#include <angelscript/scriptdictionary/scriptdictionary.h>
#include <angelscript/scriptbuilder/scriptbuilder.h>

void print(const std::string& msg)
{
    std::cout << msg << std::endl;
}

void print_generic(asIScriptGeneric* gen)
{
    const auto * msg = static_cast<std::string*>(gen->GetArgObject(0));
    print(*msg);
}

void on_engine_message(const asSMessageInfo* msg, void* param)
{
    const char* prefix = "Unk : ";
    switch (msg->type) {
        case asMSGTYPE_ERROR:
            prefix = "Err : ";
            break;
        case asMSGTYPE_WARNING:
            prefix = "Warn: ";
            break;
        case asMSGTYPE_INFORMATION:
            prefix = "Info: ";
            break;
    }

    std::cout << prefix << msg->section << " (" << msg->row << ", " << msg->col << ") : " << msg->message << std::endl;
}

void ret_check(int r, const char* msg = "error")
{
    if (r < 0)
        throw std::runtime_error(msg);
}

void run_script(const std::string& code)
{
    auto engine = asCreateScriptEngine();
    {
        int r = engine->SetMessageCallback(asFUNCTION(on_engine_message), nullptr, asCALL_CDECL);
        assert(r >= 0);
    }

    RegisterScriptArray(engine, true);
    RegisterStdString(engine);
    RegisterStdStringUtils(engine);
    RegisterScriptDictionary(engine);

    {
        int r;
        r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print_generic), asCALL_GENERIC);
        assert(r >= 0);
    }
    try
    {
        int r;
        CScriptBuilder builder;
        r = builder.StartNewModule(engine, "main");
        ret_check(r, "start module fail");
        r = builder.AddSectionFromMemory("main.as", code.c_str(), code.length());
        ret_check(r, "add section fail");
        r = builder.BuildModule();
        ret_check(r, "build module fail");

        auto mod = engine->GetModule("main");
        auto func = mod->GetFunctionByDecl("void main()");
        if (func == nullptr)
            ret_check(-1, "no void main");

        auto ctx = engine->CreateContext();
        try
        {
            r = ctx->Prepare(func);
            std::cout << "Prepare: " << r << std::endl;
            r = ctx->Execute();
            std::cout << "Exec: " << r << std::endl;
            if (r != asEXECUTION_FINISHED) {
                if (r == asEXECUTION_EXCEPTION) {
                    std::cerr << "Exception: " << ctx->GetExceptionString() << std::endl;
                }
            }
        }
        catch (std::runtime_error& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        ctx->Release();
    }
    catch (std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    engine->ShutDownAndRelease();
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    run_script(R"(void main(){ print("From script"); })");
    return 0;
}

#include <angelscript/scriptstdstring/scriptstdstring.cpp>
#include <angelscript/scriptstdstring/scriptstdstring_utils.cpp>
#include <angelscript/scriptarray/scriptarray.cpp>
#include <angelscript/scriptdictionary/scriptdictionary.cpp>
#include <angelscript/scriptbuilder/scriptbuilder.cpp>
