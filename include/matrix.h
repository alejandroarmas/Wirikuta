#ifndef MATRIX_REPRESENTATION_H
#define MATRIX_REPRESENTATION_H

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <memory>
#include <utility>

#include "assert.h"
#include "strong_types.h"



namespace Matrix {


    using Rows    = NamedType<u_int64_t, struct RowParameter>;
    using Columns = NamedType<u_int64_t, struct ColumnParameter>;

    



    class Representation {

        public:
            using matrix_iter = std::vector<float>::iterator;
            using const_matrix_iter = std::vector<float>::const_iterator;
            
            ~Representation() noexcept {};
            
            enum class Type : uint8_t {
                MATRIX,
                ROW_VECTOR,
                COLUMN_VECTOR,
                SCALAR,
            };


            Representation() noexcept : rows(0), columns(0) {}
            
            
                explicit Representation(Rows _l, Columns _w) noexcept  : 
                rows(_l.get()), 
                columns(_w.get()), 
                data(std::vector<float>(_l.get() * _w.get(), 0)) {}

            
            explicit Representation(const Matrix::Representation& _other) noexcept : 
                rows(_other.rows), 
                columns(_other.columns), 
                data(_other.data) {}
            
            
            explicit Representation(Matrix::Representation&& _other) noexcept : 
                rows(std::exchange(_other.rows, 0)), 
                columns(std::exchange(_other.columns, 0)), 
                data(std::move(_other.data)) {}
            

            Representation& operator=(const Matrix::Representation& _other) noexcept {
                rows    = _other.rows; 
                columns = _other.columns; 
                data    = _other.data;
                return *this;
            }

            Representation& operator+=(const Matrix::Representation& _other) noexcept {
                
                assert(rows    == _other.rows); 
                assert(columns == _other.columns);

                auto r = _other.constScanStart();
                for (auto l = scanStart(); l != scanEnd(); l++, r++) {
                    *l += *r;
                }

                return *this;
            }

            friend Representation operator*(double val, const Matrix::Representation& _other) noexcept {
                
                Representation output{_other};

                for (auto it = output.scanStart(); it != output.scanEnd(); it++) {
                    *it = val * *it;
                }

                return Representation{output};
            }


            Representation& operator=(Matrix::Representation&& _other) {
                rows = std::exchange(_other.rows, 0);
                columns = std::exchange(_other.columns, 0); 
                data = std::move(_other.data);
                return *this; 
            }

            Type get_type(void) const noexcept;
            std::string_view get_type_string(void) const noexcept;

            bool operator==(const Matrix::Representation _other) noexcept;
            bool operator!=(const Matrix::Representation _other) noexcept;

            constexpr u_int64_t num_rows() const noexcept { return rows; }
            constexpr u_int64_t num_cols() const noexcept { return columns; }
            
            
            float get(u_int64_t r, u_int64_t c) const noexcept;
            void put(u_int64_t r, u_int64_t c, float val) noexcept;


            constexpr matrix_iter scanStart() { return data.begin(); }
            constexpr matrix_iter scanEnd()   { return data.end(); }
            
            constexpr const_matrix_iter constScanStart() const { return data.cbegin(); }
            constexpr const_matrix_iter constScanEnd() const { return data.cend(); }


            // friend void swap(Representation& left, Representation& right) noexcept {
            //     std::swap(left.rows, right.rows);
            //     std::swap(left.columns, right.columns);
            //     std::swap(left.data, right.data);
            // }

        private:
            u_int64_t rows;
            u_int64_t columns;
            std::vector<float> data;
    };



    // class Matrix : public Representation {

    // };

    // class ColumnVector;

    // class RowVector : public Representation {
    //     RowVector() {}

    //     ColumnVector transpose(void) {

    //     }
    // };

    // class ColumnVector : public Representation {
        
    //     ColumnVector() {}

    //     RowVector transpose(void) {
    //     }

    // };

}



#endif