#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include "tensor.h"
#include "tensor_factory.h"
#include "m_algorithms_utilities.h"

#include <cstdint>
#include <memory>
#include <functional>
#include <map>
#include <vector>

#include <iostream>


    


namespace NeuralNetwork {

    constexpr u_int8_t FLAT = 1;


    using namespace NeuralNetwork::Computation::Graph;

    // template <typename ComputationalStepImpl>
    // concept StepLike = requires(ComputationalStepImpl step) {

    //     step.forward(std::shared_ptr<Tensor>{});
    // };


    // template <StepLike ComputationalStepImpl>
    // struct StepInterfaceConcept :  ComputationalStepImpl, tl {};

    // using AddStep      = StepInterfaceConcept<ComputationalStep<BinaryOperationStep<AddStep>>>;
    // using MultiplyStep = StepInterfaceConcept<ComputationalStep<BinaryOperationStep<MatrixMultiplyStep>>>;
    // using Layer        = StepInterfaceConcept<ComputationalStep<Layer>>;
    // using Sequential   = StepInterfaceConcept<ComputationalStep<Sequential>>;



    class StepInterface {

        public:
            virtual ~StepInterface() = default;
            virtual std::shared_ptr<Tensor> forward(std::shared_ptr<Tensor> input) = 0;
            };




    template <class Implementation>
    class ComputationalStep: public StepInterface {

        public:
            std::shared_ptr<Tensor> forward(std::shared_ptr<Tensor> input) noexcept override{ 
 
                assert(input != nullptr && "Tensor has no data (pointing to null).");

                auto out = Impl().doForward(input); 


                if (out->stats.has_value()) {
                    Matrix::Operations::Utility::PrintTensorStats p;
                    p(out->stats.value());
                }

                return out;
            }

            ~ComputationalStep() {}
        private:
            Implementation& Impl() { return *static_cast<Implementation*>(this); }
            ComputationalStep() = default;
            friend Implementation;
    };



    /*

    DESCRIPTION:

        Wraps around a tensor object, and used in NN abstraction.

    */
    template <class Operation>
    class BinaryOperationStep: public ComputationalStep<BinaryOperationStep<Operation>> {

        public:
            BinaryOperationStep(Matrix::Rows _l, Matrix::Columns _w) noexcept : 
                matrix(NeuralNetwork::Computation::Graph::TensorConstructor::create(_l, _w, Computation::Graph::IsTrackable(true), Computation::Graph::IsLeaf(true))) {}
            std::shared_ptr<Tensor> doForward(std::shared_ptr<Tensor> input) noexcept { return Impl()._doForward(input);}
        protected:
            std::shared_ptr<Tensor> matrix;
        private:
            friend Operation;
            Operation& Impl() { return *static_cast<Operation*>(this); }

    };

    
    class MatrixMultiplyStep: public BinaryOperationStep<MatrixMultiplyStep> {

        public:
            MatrixMultiplyStep(Matrix::Rows _l, Matrix::Columns _w) noexcept : 
                BinaryOperationStep<MatrixMultiplyStep>(_l, _w) {}
            std::shared_ptr<Tensor> _doForward(std::shared_ptr<Tensor> input) noexcept;
    };
    
    
    /*

        DESCRIPTION:

            Wrapper for bias term that is added during a perceptron.
    */

    class AddStep: public BinaryOperationStep<AddStep> {

        public:
            AddStep(Matrix::Columns _w) noexcept : 
                BinaryOperationStep<AddStep>(Matrix::Rows(FLAT), _w) {}
            std::shared_ptr<Tensor> _doForward(std::shared_ptr<Tensor> input) noexcept;
    };

    /*
    -----------------------------------------------------------------
    */


    template <class Implementation>
    class ComposedStep {

        public:
            void add(std::unique_ptr<StepInterface> layer) noexcept {

                assert(layer != nullptr && "Layer has no data (pointing to null).");

                Impl()._add(std::move(layer));
            }
        private:
            friend Implementation;
            Implementation& Impl() noexcept { return *static_cast<Implementation*>(this); }
    };


    class Layer: public ComputationalStep<Layer>, public ComposedStep<Layer> {

        public:
            Layer(std::unique_ptr<StepInterface> _w, 
                std::unique_ptr<StepInterface> _b) noexcept : 
                weights(std::move(_w)), bias(std::move(_b)) {}
                
            std::shared_ptr<Tensor> doForward(std::shared_ptr<Tensor> input) noexcept;
            void _add(std::unique_ptr<StepInterface> layer) noexcept;
            
        private:
            std::unique_ptr<StepInterface> weights;
            std::unique_ptr<StepInterface> bias;
    };


    /*
    DESCRIPTION:

        Responsible for creating a tree of layers to be computed and checking if we
        are allowed to compute in this order.

    USAGE:
            

        using matrix_t = NeuralNetwork::Computation::Graph::Tensor; 

        std::shared_ptr<matrix_t> ma = std::make_shared<matrix_t>(Matrix::Rows(1), Matrix::Columns(2000));


        NeuralNetwork::Sequential model;

        model.add(std::make_unique<NeuralNetwork::Layer>(
                std::make_unique<NeuralNetwork::MatrixMultiplyStep>(Matrix::Rows(2000), Matrix::Columns(1000)),
                std::make_unique<NeuralNetwork::AddStep>(Matrix::Columns(1000))    
        ));
        model.add(std::make_unique<NeuralNetwork::ActivationFunctions::ReLU>());
        model.add(std::make_unique<NeuralNetwork::Layer>(
                std::make_unique<NeuralNetwork::MatrixMultiplyStep>(Matrix::Rows(1000), Matrix::Columns(10)),
                std::make_unique<NeuralNetwork::AddStep>(Matrix::Columns(10))    
        ));
        model.add(std::make_unique<NeuralNetwork::ActivationFunctions::ReLU>());
        

        auto out = model.forward(ma);

    */
    class Sequential: public ComputationalStep<Sequential>, public ComposedStep<Sequential> {
        public:
            std::shared_ptr<Tensor> doForward(std::shared_ptr<Tensor> input) noexcept;
            void _add(std::unique_ptr<StepInterface> layer) noexcept;
        private:
            std::vector<std::unique_ptr<StepInterface>> _modules;

    };



}



#endif // NETWORK_LAYER_H