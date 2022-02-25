#include "m_algorithms.h"

#include <cilk/cilk.h>
#include <cilk/reducer.h>


namespace Matrix {

    namespace Operations {


        namespace Addition {

            std::unique_ptr<Matrix::Representation> Std::operator()(
                    std::unique_ptr<Matrix::Representation>& l, 
                    std::unique_ptr<Matrix::Representation>& r) {

                if ((l->num_rows() != r->num_rows()) && (l->num_cols() != r->num_cols())) {
                    throw std::length_error("Matrix A not same size as Matrix B.");
                }
                    
                auto output = std::make_unique<Matrix::Representation>(l->num_rows(), r->num_cols());

                std::transform(l->scanStart(), l->scanEnd(), r->scanStart(), output->scanStart(), std::plus<float>());

                return output;
            }
        }


        namespace HadamardProduct {

            std::unique_ptr<Matrix::Representation> Std::operator()(
                    std::unique_ptr<Matrix::Representation>& l, 
                    std::unique_ptr<Matrix::Representation>& r) {

                    auto output = std::make_unique<Matrix::Representation>(l->num_rows(), r->num_cols());

                    
                    std::transform(l->scanStart(), l->scanEnd(), r->scanStart(), output->scanStart(), std::multiplies<float>()); 
                    
                return output;
            }


            std::unique_ptr<Matrix::Representation> Naive::operator()(
                    std::unique_ptr<Matrix::Representation>& l, 
                    std::unique_ptr<Matrix::Representation>& r) {

                if ((l->num_rows() != r->num_rows()) && (l->num_cols() != r->num_cols())) {
                    throw std::length_error("Matrix A not same size as Matrix B.");
                }

                std::unique_ptr<Matrix::Representation> output = std::make_unique<Matrix::Representation>(l->num_rows(), r->num_cols());


                for (u_int64_t i = 0; i < l->num_rows(); i++) {
                    
                    for (u_int64_t j = 0; j < r->num_cols(); j++) {


                        float val = l->get(i, j) * r->get(i, j);

                        output->put(i, j, val);

                    }

                }


                return output;
            }
        } 


        namespace Multiplication {

            std::unique_ptr<Matrix::Representation> Naive::operator()(
                    std::unique_ptr<Matrix::Representation>& l, 
                    std::unique_ptr<Matrix::Representation>& r) {

                if (l->num_cols() != r->num_rows()) {
                    throw std::length_error("Matrix A columns not equal to Matrix B rows.");
                }

                std::unique_ptr<Matrix::Representation> output = std::make_unique<Matrix::Representation>(l->num_rows(), r->num_cols());


                for (u_int64_t i = 0; i < l->num_rows(); i++) {
                    
                    for (u_int64_t j = 0; j < r->num_cols(); j++) {


                        float val = 0;

                        for (u_int64_t k = 0; k < l->num_cols(); k++) {
                            val += l->get(i, k) * r->get(k, j);
                        }

                        output->put(i, j, val);

                    }

                }



                return output;
            }

            /*
            Assume that n is an exact power of 2.
            
            
            void Rec_Mult(double *C, double *A, double *B,
                        int64_t n, int64_t rowsize) {
            if (n == 1)
                C[0] += A[0] * B[0];
            else {
                int64_t d11 = 0;
                int64_t d12 = n/2;
                int64_t d21 = (n/2) * rowsize;
                int64_t d22 = (n/2) * (rowsize+1);
                cilk_spawn Rec_Mult(C+d11, A+d11, B+d11, n/2, rowsize);
                cilk_spawn Rec_Mult(C+d21, A+d22, B+d21, n/2, rowsize);
                cilk_spawn Rec_Mult(C+d12, A+d11, B+d12, n/2, rowsize);
                Rec_Mult(C+d22, A+d22, B+d22, n/2, rowsize);
                cilk_sync;
                cilk_spawn Rec_Mult(C+d11, A+d12, B+d21, n/2, rowsize);
                cilk_spawn Rec_Mult(C+d21, A+d21, B+d11, n/2, rowsize);
                cilk_spawn Rec_Mult(C+d12, A+d12, B+d22, n/2, rowsize);
                Rec_Mult(C+d22, A+d21, B+d12, n/2, rowsize);
                cilk_sync;
            } }
            */
            std::unique_ptr<Matrix::Representation> Square::operator()(
                    std::unique_ptr<Matrix::Representation>& l, 
                    std::unique_ptr<Matrix::Representation>& r) {

                if (l->num_cols() != r->num_rows()) {
                    throw std::length_error("Matrix A columns not equal to Matrix B rows.");
                }

                std::unique_ptr<Matrix::Representation> output = std::make_unique<Matrix::Representation>(l->num_rows(), r->num_cols());


                cilk_for (u_int64_t i = 0; i < l->num_rows(); i++) {
                    
                    for (u_int64_t j = 0; j < r->num_cols(); j++) {


                        float val = 0;

                        for (u_int64_t k = 0; k < l->num_cols(); k++) {
                            val += l->get(i, k) * r->get(k, j);
                        }

                        output->put(i, j, val);

                    }

                }



                return output;
            }
        }

    }

}











