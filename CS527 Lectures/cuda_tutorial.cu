
#include <iostream>
#include <conio.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <device_functions.h>
#include <ctime>
#include <stdlib.h>
#include <chrono>

#define BLOCK_SIZE 128

using namespace std;
using namespace std::chrono;

__global__ void PrefixSum(int* a, int n)
{
	for (int i = 1; i < n; i++) {
		a[i] += a[i - 1];
	}
}

int main()
{
	srand(time(NULL));
	for (int n = BLOCK_SIZE; n <= 128 * BLOCK_SIZE; n += BLOCK_SIZE) {
		//cout << "n is " << n << '\n';
		int* h_a = new int[n];
		for (int i = 0; i < n; i++)
			h_a[i] = 1;

		int* d_a;
		if (cudaMalloc(&d_a, sizeof(int) * n) != cudaSuccess) {
			cout << "Failed to allocate global memory in GPU.\n";
			delete[] h_a;
			return 0;
		}

		if (cudaMemcpy(d_a, h_a, sizeof(int) * n, cudaMemcpyHostToDevice) != cudaSuccess) {
			cout << "Failed to copy to global memory in GPU.\n";
			cudaFree(d_a);
			delete[] h_a;
			return 0;
		}

		auto start = high_resolution_clock::now();
		PrefixSum <<<1, 1>>> (d_a, n);
		cudaDeviceSynchronize();
		auto stop = high_resolution_clock::now();

		if (cudaMemcpy(h_a, d_a, sizeof(int) * n, cudaMemcpyDeviceToHost) != cudaSuccess) {
			cudaFree(d_a);
			delete[] h_a;
			cout << "Failed to copy to main memory in CPU.\n";
			return 0;
		}

		for (int i = 0; i < n; i++) {
			if (h_a[i] != i + 1) {
				cout << "FAIL";
				break;
			}
		}
		//cout << '\n';

		auto duration = duration_cast<microseconds>(stop - start);
		cout << "n = " << n << ", t = " << duration.count() << '\n';
	}
	return 0;

}




/*
#include <iostream>
#include <conio.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <device_functions.h>
#include <ctime>
#include <stdlib.h>
#include <chrono>

#define BLOCK_SIZE 128

using namespace std;
using namespace std::chrono;



__device__ void UpSweep(int* shared_memory, int i)
{
	// Traversing the prefix tree from bottom to top
	for (int d = 1; d < BLOCK_SIZE; d *= 2) {
		// Performing additions at depth d in parallel and synchronizing
		if (i % (2 * d) == 0)
			shared_memory[i + 2 * d - 1] = shared_memory[i + d - 1] + shared_memory[i + 2 * d - 1];
		__syncthreads();
	}
}

__device__ void DownSweep(int* shared_memory, int i)
{
	// Traversing the prefix tree from top to bottom
	for (int d = BLOCK_SIZE / 4; d >= 1; d /= 2) {
		// Performing additions at depth d in parallel and synchronizing
		if(i % (2 * d) == 0 && (i + 2 * d - 1 + d) < BLOCK_SIZE)
			shared_memory[i + 2 * d - 1 + d] += shared_memory[i + 2 * d - 1];
		__syncthreads();
	}
}


__global__ void Aggregate(int* a, int n)
{
	// Initializing shared memory half for array elements and half for last elements of each block
	__shared__ int shared_memory_2[2 * BLOCK_SIZE];
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	shared_memory_2[threadIdx.x] = 0;
	shared_memory_2[BLOCK_SIZE + threadIdx.x] = 0;
	__syncthreads();

	// Copying array elements to shared memory
	if (i < n)
		shared_memory_2[threadIdx.x] = a[i];
	__syncthreads();

	// Copying last element of each block to shared memory
	int j = (blockIdx.x + 1) * BLOCK_SIZE - 1;
	if(j < n)
		shared_memory_2[threadIdx.x + BLOCK_SIZE] = a[j];
	__syncthreads();

	// Performing upsweep and downsweep steps
	UpSweep(shared_memory_2 + BLOCK_SIZE, threadIdx.x);
	DownSweep(shared_memory_2 + BLOCK_SIZE, threadIdx.x);

	// Adding the resulting prefix sum to the respective array elements in shared memory
	if (blockIdx.x >= 1) {
		shared_memory_2[threadIdx.x] += shared_memory_2[BLOCK_SIZE + blockIdx.x - 1];
	}
	__syncthreads();
	
	// Copying the elements of shared memory back to array in global memory
	a[i] = shared_memory_2[threadIdx.x];
	__syncthreads();
}


__global__ void ParallelPrefixSum(int* a, int n)
{
	// Initializing shared memory of one block size section of the array
	__shared__ int shared_memory[BLOCK_SIZE];
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	shared_memory[threadIdx.x] = 0;
	__syncthreads();
	
	if (i < n) {
		// Copying the elements of array into shared memory
		shared_memory[threadIdx.x] = a[i];
		__syncthreads();

		// Performing the upsweep and downsweep steps
		UpSweep(shared_memory, threadIdx.x);
		DownSweep(shared_memory, threadIdx.x);
		
		// Copying the elements of shared memory back to array in global memory
		a[i] = shared_memory[threadIdx.x];
	}	
}

int main()
{
	srand(time(NULL));
	
	for (int n = BLOCK_SIZE; n <= 128 * BLOCK_SIZE; n += BLOCK_SIZE) {
		// Declaration and Allocation of memory in host
		int* h_a = new int[n];
		for (int i = 0; i < n; i++)
			h_a[i] = 1;

		// Declaration and Allocation of memory in device
		int* d_a;
		if (cudaMalloc(&d_a, sizeof(int) * n) != cudaSuccess) {
			cout << "Failed to allocate global memory in GPU.\n";
			delete[] h_a;
			return 0;
		}

		// Data transfer from host to device memory
		if (cudaMemcpy(d_a, h_a, sizeof(int) * n, cudaMemcpyHostToDevice) != cudaSuccess) {
			cout << "Failed to copy to global memory in GPU.\n";
			cudaFree(d_a);
			delete[] h_a;
			return 0;
		}

		// Kernels execute
		auto start = high_resolution_clock::now();
		ParallelPrefixSum <<<n / BLOCK_SIZE + 1, BLOCK_SIZE >>> (d_a, n);
		cudaDeviceSynchronize();
		Aggregate <<<n / BLOCK_SIZE + 1, BLOCK_SIZE >>> (d_a, n);
		cudaDeviceSynchronize();
		auto stop = high_resolution_clock::now();

		// Data transfers back to host
		if (cudaMemcpy(h_a, d_a, sizeof(int) * n, cudaMemcpyDeviceToHost) != cudaSuccess) {
			cudaFree(d_a);
			delete[] h_a;
			cout << "Failed to copy to main memory in CPU.\n";
			return 0;
		}

		// Checking for  correctness
		for (int i = 0; i < n; i++) {
			//cout << h_a[i] << ' ';
			if (h_a[i] != i + 1) {
				cout << "FAIL";
				break;
			}
		}
		cout << '\n';

		// Printing the output
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "n = " << n << ", t = " << duration.count() << '\n';
	}
	return 0;

}
*/