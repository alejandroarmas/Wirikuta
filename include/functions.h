#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H

#include <cstdint>

namespace ActivationFunctions {

    
    template <class T>
    class BaseFunction {

        public:
            virtual T operate(T x) = 0;
    };


    template<class T>
    class Sigmoid : public BaseFunction<T> {

        public:
            T operate(T x) override;
            
    };


    template class Sigmoid<float>;
    template class Sigmoid<double>;
    template class Sigmoid<int8_t>;
    template class Sigmoid<int16_t>;
    template class Sigmoid<int32_t>;
    template class Sigmoid<int64_t>;
    template class Sigmoid<uint8_t>;
    template class Sigmoid<uint16_t>;
    template class Sigmoid<uint32_t>;
    template class Sigmoid<uint64_t>;

    
    namespace Differentiable {

        template<class T>
        class Sigmoid : public ActivationFunctions::Sigmoid<T> {

            public:
                T operate(T x) override;
                
        };
    }

}


#endif // ACTIVATION_FUNCTION_H