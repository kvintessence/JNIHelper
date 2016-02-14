#include <iostream>
#include <functional>
#include <set>

struct JNIEnv;
struct jobject_;
typedef jobject_* jobject;

// ========================================================================================================================

class JavaNativeMethodRegistration
{
public:
    JavaNativeMethodRegistration(void* methodPointer)
    {
        std::cout << methodPointer << std::endl;
    }
};

// ========================================================================================================================

template<class WrapperClass, class ReturnType, class ... Arguments>
class JavaNativeMethod
{
    using MethodImplementationPointer = ReturnType(WrapperClass::*)(Arguments...);
    
    static MethodImplementationPointer s_callback;
    
public:
    JavaNativeMethod(MethodImplementationPointer callback)
    {
        s_callback = callback;
        static JavaNativeMethodRegistration s_registration((void*)JavaNativeMethod<WrapperClass, ReturnType, Arguments...>::rawNativeMethod);
    }

private:
    static ReturnType rawNativeMethod(JNIEnv*, jobject javaObject, Arguments ... args)
    {
        WrapperClass::notifyCppObjects(javaObject, [=] (WrapperClass* wrapperInstance) {
            (wrapperInstance->*s_callback)(args...);
        });
    }
};

template<class WrapperClass, class ReturnType, class ... Arguments>
typename JavaNativeMethod<WrapperClass, ReturnType, Arguments...>::MethodImplementationPointer JavaNativeMethod<WrapperClass, ReturnType, Arguments...>::s_callback(nullptr);


// ========================================================================================================================

template <class WrapperClass>
class JavaObjectWrapper
{
    using WrapperObjectInfo = std::pair<jobject, WrapperClass*>;
    
    static std::set<WrapperObjectInfo> g_objectsCollection;
    
public:
    static void notifyCppObjects(jobject javaObject, std::function<void(WrapperClass*)> callback)
    {
        for (auto& entry: g_objectsCollection) {
//            if (jh::areEqual(entry.first, javaObject)) {
                callback(entry.second);
//            }
        }
    }
};

template <class WrapperClass>
std::set<typename JavaObjectWrapper<WrapperClass>::WrapperObjectInfo> JavaObjectWrapper<WrapperClass>::g_objectsCollection;

// ========================================================================================================================

class AMIntersitial : public JavaObjectWrapper<AMIntersitial>
{
public:
    JavaNativeMethod<AMIntersitial, void> oneMethod;

    AMIntersitial()
    : oneMethod(&AMIntersitial::nativeMethodImplementation)
    {
        
    }
    
private:
    void nativeMethodImplementation()
    {
        
    }
};

// ========================================================================================================================

int main(int argc, const char * argv[])
{
    AMIntersitial interstitial;
    return 0;
}
