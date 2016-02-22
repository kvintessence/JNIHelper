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

static jh::JavaObjectPointer exampleObj;

void testObjectWrapperStart()
{
    jh::reportInternalInfo("Test #4: Object pointer.");

    jh::reportInternalInfo("Should be null: " + to_string(exampleObj == nullptr));
    exampleObj = jh::createNewObject<JavaExample>();
    jh::reportInternalInfo("Should not be null: " + to_string(exampleObj != nullptr));

    jh::JavaObjectPointer other = exampleObj;
    jh::reportInternalInfo("Copy should not be null either: " + to_string(other != nullptr));
    int res = jh::callMethod<int>(other, "instance3");
    jh::reportInternalInfo("calling method (should return 777): " + to_string(res));
    jh::reportInternalInfo("Will be continued later...");
}

void testObjectWrapperAndStaticNativeMethodsEnd()
{
    jh::reportInternalInfo("Continuing test #4 & #7...");

    jh::reportInternalInfo("Should not be null: " + to_string(exampleObj != nullptr));
    int res = jh::callMethod<int>(exampleObj, "instance3");
    jh::reportInternalInfo("calling method (should return 777): " + to_string(res));

    jh::reportInternalInfo("Test #4 & #7: End.");
}

void testLocalFrame()
{
    jh::reportInternalInfo("Test #5: Local frame.");

    jstring s1 = jh::createJString("s1");

    jh::LocalReferenceFrame frame;

    jstring s2 = jh::createJString("s2");
    jh::reportInternalInfo("s1 is alive: " + to_string(!jh::areEqual(s1, nullptr)));
    jh::reportInternalInfo("s2 is alive: " + to_string(!jh::areEqual(s2, nullptr)));

    frame.pop();
    jh::reportInternalInfo("Poped local references frame, s2 should be dead...");

    // seams like uncommenting the second line will crash the app; this is correct behaviour
    jh::reportInternalInfo("s1 is alive: " + to_string(!jh::areEqual(s1, nullptr)));
//    jh::reportInternalInfo("s2 is alive: " + to_string(!jh::areEqual(s2, nullptr)));

    jh::reportInternalInfo("New frame, new s2...");

    frame.push();
    s2 = jh::createJString("s2");

    jh::reportInternalInfo("s1 is alive: " + to_string(!jh::areEqual(s1, nullptr)));
    jh::reportInternalInfo("s2 is alive: " + to_string(!jh::areEqual(s2, nullptr)));

    jh::reportInternalInfo("Popping frame, but leaving s2...");

    frame.pop(&s2);
    jh::reportInternalInfo("s1 is alive: " + to_string(!jh::areEqual(s1, nullptr)));
    jh::reportInternalInfo("s2 is alive: " + to_string(!jh::areEqual(s2, nullptr)));

    jh::reportInternalInfo("Test #5: End.");
}

void jstringTest()
{
    jh::reportInternalInfo("Test #6: JString utils.");

    jh::LocalReferenceFrame frame;

    jh::reportInternalInfo("Lets transform jstring to std::string and back...");
    jh::reportInternalInfo("12345: [" + jh::jstringToStdString(jh::createJString("12345")) + "]");
    jh::reportInternalInfo("Рашн лангвич: [" + jh::jstringToStdString(jh::createJString("Рашн лангвич")) + "]");
    jh::reportInternalInfo("!@#$%^&*()[]{}|/.,: [" + jh::jstringToStdString(jh::createJString("!@#$%^&*()[]{}|/.,")) + "]");
    jh::reportInternalInfo("empty: [" + jh::jstringToStdString(jh::createJString("")) + "]");

    jh::reportInternalInfo("Test #6: End.");
}

void staticNativeMethodsTest()
{
    jh::reportInternalInfo("Test #7: Static native methods.");

    jh::reportInternalInfo("Registering native method, which will be called later...");
    jh::registerStaticNativeMethod<JavaExample, void>("staticNativeMethod", &testObjectWrapperAndStaticNativeMethodsEnd);
    jh::callStaticMethod<JavaExample, void>("callNativeStaticMethodLater");

    jh::reportInternalInfo("Will be continued later...");
}

class ExampleWrapper : public jh::JavaObjectWrapper<JavaExample, ExampleWrapper>
{
public:
    void test()
    {
        jh::reportInternalInfo("object=" + to_string(object()));
        jh::callMethod<void>(object(), "testNativeMethod");
    }

private:
    void linkJavaNativeMethods() override
    {
        registerNativeMethod<1, void>("native1", &ExampleWrapper::native1);
        registerNativeMethod<2, double, double, double>("native2", &ExampleWrapper::native2);
        registerNativeMethod<3, jstring, jstring>("native3", &ExampleWrapper::native3);
        registerNativeMethod<4, JavaExample>("native4", &ExampleWrapper::native4);
        registerNativeMethod<5, void, JavaExample>("native5", &ExampleWrapper::native5);
    }

    jobject initializeJavaObject() override
    {
        return jh::createNewObject<JavaExample>();
    }

    void native1()
    {
        jh::reportInternalInfo("native1 method called");
    }

    double native2(double x, double y)
    {
        return x + y;
    }

    jstring native3(jstring s)
    {
        std::string ss = jh::jstringToStdString(s);
        return jh::createJString(ss + "X" + ss);
    }

    jobject native4()
    {
        return jh::createNewObject<JavaExample>();
    }

    void native5(jobject example)
    {
        jh::callMethod<void>(example, "instance1");
    }
};

void testNativeMethod()
{
    jh::reportInternalInfo("Test #8: Instance native methods.");

    jh::reportInternalInfo("equal = " + to_string(jh::areEqual(nullptr, nullptr)));

    ExampleWrapper wrapper;
    wrapper.test();

    jh::reportInternalInfo("Will be continued later...");
}

extern "C"
{
    void Java_com_example_hellojni_HelloJni_performTest(JNIEnv*, jobject)
    {
        testObjectCreation();
        testStaticMethods();
        testInstanceMethods();
        testObjectWrapperStart();
        testLocalFrame();
        jstringTest();
        staticNativeMethodsTest();
        testNativeMethod();
    }
}
