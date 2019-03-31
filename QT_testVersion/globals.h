#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iterator>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/LU>
#include <unsupported/Eigen/MatrixFunctions>

// TYPES
typedef Eigen::Matrix<float, Eigen::Dynamic, 1> mat_D1f;
typedef Eigen::Matrix<float, Eigen::Dynamic, 2> mat_D2f;
typedef Eigen::Matrix<float, Eigen::Dynamic, 3> mat_D3f;
typedef Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> mat_DDi;
//typedef Eigen::Matrix<T, N, 1> V; // vertical
//typedef Eigen::Matrix<T, 1, N> V_horizontal; // horizontal
//typedef Eigen::Matrix<T, Dynamic, Dynamic> DM; // dynamic matrix
typedef float T;

/// CONSTANTS
constexpr T EPSILON = 1e-6;
constexpr T PI = 3.141592653589793238462643383279;

///// TYPE METHODS
//T dot_horizontal(V_horizontal a, V_horizontal b) {
//    return (a * b.transpose())(0, 0);
//}
//T dot_vertical(V a, V b) {
//    return (a.transpose() * b)(0, 0);
//}
static T clamp(T a, T minVal, T maxVal){
    return std::max(minVal, std::min(maxVal, a));
}
