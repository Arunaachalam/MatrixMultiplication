A Program for 2D Matrix Multiplication.

Problem:
	The computational complexity of a 2D matrix multiplication is in the order of O(n³). When using computers, improper memory utilization also increases computation time due to cache misses and data transfers. So a fast way of matrix multiplication is developed


Solutions:
Naive Matrix Multiplication - A common matrix multiplication algorithm, here cache misses are more due to data loading order [O(n³)]

Using the C++ memory storage algorithm - By considering how the elements are stored (row/column major), a simple transpose of one of the matrices multiplied will reduce cache misses. [O(n³)]

Loop Unrolling - By using the AVX architecture vectorization can be done to increase the speed of computation [O(n³)]

Blocking - By using the size of cache memory of a processor, the rate of cache misses can be reduced by using the technique of dividing and conquering, where the matrices are divided into smaller chunks with memory which are able to fit a cache usually 256 kB or 512 kB. [O(n³)]

Strassen-Winograd - By using an algorithm which reduces the order of computation, the computation time can be reduced. [O(pow(n,2.7))]

