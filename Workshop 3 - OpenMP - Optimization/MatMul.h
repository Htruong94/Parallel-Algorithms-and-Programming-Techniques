// Workshop 3 - Matrix Multiplication - Auto Vectorization
// MatMul.h
// 2020.09.30
// Chris Szalwinski
#pragma once

constexpr size_t N{ 2048 };
using row = double[N];

void initialize(row a[]);
void matmul(const row a[], const row b[], row c[], int n);
double checksum(const row x[]);