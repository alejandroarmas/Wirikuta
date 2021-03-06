#include <iostream>
#include <time.h>
#include <chrono>

#include <cilk/cilk.h>
#include <cilk/cilkscale.h>

#include "matrix_benchmark.h"


namespace Matrix {


    // template <Operations::UnaryMatrixOperatable Operator>
    // Representation Operations::Timer<Operator>::operator()(const Matrix::Representation& l, 
    //         const Matrix::Representation& r) {

    //     start = std::chrono::steady_clock::now();                
    //     Matrix::Representation mc = matrix_operation(l);
    //     end   = std::chrono::steady_clock::now();

        
    //     return Matrix::Representation{mc};
    // }


    // template <Operations::BinaryMatrixOperatable Operator>
    // Representation Operations::Timer<Operator>::operator()(const Matrix::Representation& l, 
    //         const Matrix::Representation& r) {

    //     start = std::chrono::steady_clock::now();                
    //     Matrix::Representation mc = matrix_operation(l, r);
    //     end   = std::chrono::steady_clock::now();

        
    //     return Matrix::Representation{mc};
    // }

    
    // template class Operations::Timer<Operations::Unary::ReLU>;
    // template class Operations::Timer<Operations::Unary::SoftMax>;
    // template class Operations::Timer<Operations::Unary::Transpose>;
    // template class Operations::Timer<Operations::Binary::HadamardProduct::Std>;
    // template class Operations::Timer<Operations::Binary::Multiplication::ParallelDNC>;
    // template class Operations::Timer<Operations::Binary::Multiplication::Naive>;
    // template class Operations::Timer<Operations::Binary::Multiplication::Square>;
    // template class Operations::Timer<Operations::Binary::Addition::Std>;
    // template class Operations::Timer<Operations::Binary::OuterProduct::Naive>;
    // template class Operations::Timer<Operations::Metric::CrossEntropy>;



// #ifdef CILKSCALE
    /*
    Cilkscale's command-line output includes work and span measurements for the Cilk program in terms of empirically measured times.
    Parallelism measurements are derived from these times.
    A simple struct wsp_t contains the number of nanoseconds for work and span. This data is collected immediately before and after 
    the wrapped function's execution. Then these two measurements are subtracted and dumped to stdout in CSV format, with the first 
    column being the label of the measurement. At the end, the same measurements are output for the program as a whole with an 
    empty label. The final measurement includes all the setup and teardown code, which pollutes the measurement we are interested in.
    Because the dump to stdout can interleave with other program output, you might want to set the environment variable 
    CILKSCALE_OUT="filename.csv" to redirect Cilkscale output to a specific file (you will only be able to access that file when 
    running Cilkscale instrumented programs locally --- awsrun.py currently doesn't return output files).

    In addition to a span column, you are also seeing a "burdened span" column. Burdened span accounts for the worst possible 
    migration overhead, which can come from work-stealing and other factors.
    */
    // std::unique_ptr<Representation> Operations::ParallelMeasurer::operator()(std::unique_ptr<Matrix::Representation> l, 
    //         std::unique_ptr<Matrix::Representation> r) {

    //             wsp_t start_wsp, stop_wsp;
                
    //             start_wsp = wsp_getworkspan();
    //             std::unique_ptr<Matrix::Representation> mc = this->matrix_operation->operator()(l, r);
    //             stop_wsp = wsp_getworkspan();

    //             wsp_dump(wsp_sub(stop_wsp, start_wsp), "Cilkscale Parallel Measurement:");
                
    //             return mc;
    //         }
// #endif

}
