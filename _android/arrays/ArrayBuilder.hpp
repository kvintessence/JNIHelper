/*!
   \file JavaArrays.hpp
   \brief
   \author Denis Sorokin
   \date 22.02.2016
 */

#ifndef JH_JAVA_ARRAYS_HPP
#define JH_JAVA_ARRAYS_HPP

#include <jni.h>
#include <vector>
#include "../core/ToJavaType.hpp"
#include "../core/JNIEnvironment.hpp"
#include "../arrays/ArrayAllocator.hpp"
#include "../arrays/ArrayGetter.hpp"
#include "../arrays/ArraySetter.hpp"

namespace jh
{
    template<class JavaArrayType>
    std::vector<typename ToJavaType<JavaArrayType>::ElementType> jarrayToVector(JavaArrayType array)
    {
        auto env = getCurrentJNIEnvironment();
        return std::move(JavaArrayGetter<JavaArrayType>::get(env, array));
    }

    /**
    * Java array builder for primitive types.
    */
    template<class JavaArrayType, class ElementType>
    class JavaBaseArrayBuilder
    {
        using ArgumentType = typename ToJavaType<ElementType>::CallReturnType;

    public:
        JavaBaseArrayBuilder() = default;
        JavaBaseArrayBuilder(std::size_t size) : m_elements(size) { };

        JavaBaseArrayBuilder<JavaArrayType, ElementType>& add(ArgumentType value)
        {
            m_elements.push_back(value);
            return *this;
        }

        JavaBaseArrayBuilder<JavaArrayType, ElementType>& add(std::initializer_list<ArgumentType> container)
        {
            for (auto element : container)
                m_elements.push_back(element);

            return *this;
        }

        template <class Iterator>
        JavaBaseArrayBuilder<JavaArrayType, ElementType>& add(Iterator b, Iterator e, typename Iterator::iterator_category* p = 0)
        {
            for (auto it = b; it != e; ++it)
                m_elements.push_back(*it);

            return *this;
        }

        JavaArrayType build()
        {
            JNIEnv* env = getCurrentJNIEnvironment();
            JavaArrayType array = JavaArrayAllocator<JavaArrayType, ElementType>::create(env, m_elements.size());
            JavaArraySetter<JavaArrayType>::set(env, array, m_elements.size(), &m_elements[0]);
            return array;
        }

    protected:
        std::vector<ArgumentType> m_elements;
    };

    template<class ElementType>
    class JavaArrayBuilder : public JavaBaseArrayBuilder<jobjectArray, ElementType> { };

    template<>
    class JavaArrayBuilder<bool> : public JavaBaseArrayBuilder<jbooleanArray, bool> { };

    template<>
    class JavaArrayBuilder<int> : public JavaBaseArrayBuilder<jintArray, int> { };

    template<>
    class JavaArrayBuilder<long> : public JavaBaseArrayBuilder<jlongArray, long> { };

    template<>
    class JavaArrayBuilder<float> : public JavaBaseArrayBuilder<jfloatArray, float> { };

    template<>
    class JavaArrayBuilder<double> : public JavaBaseArrayBuilder<jdoubleArray, double> { };
}

#endif
