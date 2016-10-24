#include <iostream>
#include <array>

using namespace std;

void sense(
    array<array<double, 5>, 4> &probDist,
    const array<array<char, 5>, 4> &colors,
    const char measurement,
    const double probSensorCorrect)
{
    //Update PD based on sensory input
    array<array<double, 5>, 4> newProbDist;
    int rows = probDist.size();
    int cols = probDist[0].size();
    double probPiece{0};
    double sum{0};

    //Update each position with its
    //non-normalized probability
    //and build the normalizer
    for (int row=0; row!=rows; ++row) {
        for (int col=0; col!=cols; ++col) {
            if (measurement == colors[row][col])
                probPiece = probSensorCorrect;
            else
                probPiece = 1.0 - probSensorCorrect;
            probPiece *= probDist[row][col];
            newProbDist[row][col] = probPiece;
            sum += probPiece;
        }
    }
    //Normalize each
    for (int row=0; row!=rows; ++row) {
        for (int col=0; col!=cols; ++col) {
            newProbDist[row][col] /= sum;
        }
    }
    probDist = newProbDist;
}

void shift(
    array<array<double, 5>, 4> &probDist,
    const array<int, 2> &motion,
    const double probMove)
{
    //Update PD based on move attempt
    array<array<double, 5>, 4> newProbDist;
    int rows = probDist.size();
    int cols = probDist[0].size();
    int rowCalc;
    int colCalc;

    //Update each position with its probability
    for (int row=0; row!=rows; ++row) {
        for (int col=0; col!=cols; ++col) {
            //Probability of not moving and staying
            newProbDist[row][col] =
                (1.0 - probMove)*probDist[row][col];
            //Plus probability of moving from another
            rowCalc = (((row - motion[0])%rows) + rows)%rows;
            colCalc = (((col - motion[1])%cols) + cols)%cols;
            newProbDist[row][col] +=
                probMove*probDist[rowCalc][colCalc];
        }
    }
    probDist = newProbDist;
}

array<array<double, 5>, 4> localize(
    const array<array<char, 5>, 4> &colors,
    const array<char, 5> &measurements,
    const array<array<int, 2>, 5> &motions,
    const double probSensorCorrect,
    const double probMove)
{
    //Localize the sensor among the 4x5 grid

    //Initialize Probability Distribution
    //as a Uniform Distribution
    array<array<double, 5>, 4> probDist;
    float rows = colors.size();
    float cols = colors[0].size();
    double probInit = 1.0/rows/cols; 
    for (uint row=0; row!=rows; ++row) {
        for (uint col=0; col!=cols; ++col) {
            probDist[row][col] = probInit;
        }
    }

    //Shift and sense n times
    for (int move=0; move!=measurements.size(); ++move) {
        shift(probDist, motions[move], probMove);
        sense(probDist, colors, measurements[move], probSensorCorrect);
    }

    return probDist;
}


int main()
{
    //Initialize Constants
    array<array<double, 5>, 4> probDist;
    const array<array<char, 5>, 4> colors{{
        {'R', 'G', 'G', 'R', 'R'},
        {'R', 'R', 'G', 'R', 'R'},
        {'R', 'R', 'G', 'G', 'R'},
        {'R', 'R', 'R', 'R', 'R'},
    }};
    const array<char , 5> measurements{
        'G', 'G', 'G', 'G', 'G',
    };
    const array<array<int, 2>, 5> motions{{
        {0, 0}, {0, 1}, {1, 0}, {1, 0}, {0, 1},
    }};
    const double probSensorCorrect = 0.7;
    const double probMove = 0.8;

    //Localize the sensor and print PD
    probDist = localize(colors, measurements,
        motions, probSensorCorrect, probMove);
    for (auto row: probDist) {
        for (double element: row) {
            cout << ' ' << element << ' ';
        }
        cout << endl;
    }
    return 0;
}
