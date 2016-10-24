#include <iostream>
#include <array>
#include <string>

/**
 * A template class designed to take 2D Matrices of any size greater
 * than zero. The matrices can be added, subtracted, multiplied,
 * transposed, inversed, and shown.
 */

template <const unsigned _xDim, const unsigned _yDim>
class Matrix {

    private:

        // The dimensions and values of the Matrix
        const unsigned xDim = _xDim;
        const unsigned yDim = _yDim;
        std::array<double, _xDim*_yDim> values;

    public:

        // A constructor for diagonal matrices with DiagVal
        Matrix(double diagVal) {
            values.fill(0);
            const unsigned diagCount = (xDim < yDim) ? xDim : yDim;
            for (unsigned index = 0; index != diagCount; index++) {
                values[index*yDim + index] = diagVal;
            }
        }

        // A constructor for input of one array
        Matrix(std::array<double, _xDim*_yDim> iValues): values(iValues) {}
        
        // Getter in X Dimension
        unsigned getXDim() const {
            return xDim;
        }

        // Getter in Y Dimension
        unsigned getYDim() const {
            return yDim;
        }

        // Show the Matrix as a String
        std::string show() const {
            std::string display;
            for (unsigned iX = 0; iX != xDim; ++iX) {
                for (unsigned iY = 0; iY != yDim; ++iY) {
                    display += ' ';
                    display += std::to_string(values[iX*yDim + iY]);
                    display += ' ';
                }
                display += '\n';
            }
            return display;
        }

        // Add two matrices
        Matrix<_xDim, _yDim>
        operator+(const Matrix<_xDim, _yDim> &rhM) {
            Matrix<_xDim, _yDim> sum(0);
            for (unsigned index = 0; index != xDim*yDim; ++index) {
                sum.values[index] = values[index] + rhM.values[index];
            }
            return sum;
        }
};


/*
 * Example Exectution
 */

int main()
{
    Matrix<2, 3> m(1);
    Matrix<2, 3> n(2);
    std::cout << (m+n).show() << std::endl;
    return 0;
}
