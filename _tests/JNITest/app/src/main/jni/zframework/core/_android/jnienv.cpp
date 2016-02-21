#include "jnienv.h"

static JavaVM* jvm;

extern "C"
{
    jint JNI_OnLoad(JavaVM *vm, void *reserved)
    {
        jvm = vm;
        return JNI_VERSION_1_6;
    }

    void JNI_OnUnload(JavaVM *vm, void *reserved)
    {

    }
}

namespace JNI
{
    JavaVM* getVM()
    {
        return jvm;
    }
}