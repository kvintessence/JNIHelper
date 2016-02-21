#include <sstream>
#include <jni.h>
#include "JavaHelper.hpp"

template <class T>
inline std::string to_string(const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

JH_JAVA_CUSTOM_CLASS(JavaExample, "com/quint/Example");

void testObjectCreation()
{
    jh::reportInternalInfo("Test #1: Object creation.");

    // new-style object creation
    jobject o1 = jh::createNewObject<JavaExample>();
    jobject o2 = jh::createNewObject<JavaExample, int, float>(7, 3.1415);

    // old-style object creation
    jobject o3 = jh::createNewObject<jstring>(JavaExample::className(), jh::createJString("someString"));
    jobject o4 = jh::createNewObject<JavaExample>(JavaExample::className(), o1);

    jh::reportInternalInfo("Test #1: End.");
}

void testStaticMethods()
{
    jh::reportInternalInfo("Test #2: Static methods.");

    // old-style static calls
    jh::callStaticMethod<void>(JavaExample::className(), "static1");
    jh::callStaticMethod<void, int, bool>(JavaExample::className(), "static2", 7, true);

    jlong x = jh::callStaticMethod<long, long, long>(JavaExample::className(), "static4", 7L, 8L);
    jh::reportInternalInfo("Static4: sum=" + to_string(x) + ".");

    // new-style static calls
    jobject o = jh::callStaticMethod<JavaExample, JavaExample, jstring>("static5", jh::createJString("static5"));
    jh::callStaticMethod<JavaExample, void, JavaExample>("static3", o);

    jh::reportInternalInfo("Test #2: End.");
}

void testInstanceMethods()
{
    jh::reportInternalInfo("Test #3: Instance methods.");

    jobject o = jh::createNewObject<JavaExample>();
    jh::callMethod<void>(o, "instance1");

    jobject o2 = jh::callMethod<JavaExample>(o, "instance5");

    int i = jh::callMethod<int>(o2, "instance3");
    jh::reportInternalInfo("instance3 method returned " + to_string(i));

    jstring s = jh::callMethod<jstring, jstring>(o2, "instance4", jh::createJString("kanojo"));
    jh::callMethod<void, jstring>(o, "instance2", s);

    jh::reportInternalInfo("Test #3: End.");
}

extern "C"
{
    void Java_com_example_hellojni_HelloJni_performTest(JNIEnv*, jobject)
    {
        testObjectCreation();
        testStaticMethods();
        testInstanceMethods();
    }
}
