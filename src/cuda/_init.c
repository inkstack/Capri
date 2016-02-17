/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 inkstack.info
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/*
 * _init.c
 *
 *  Created on: 2016-2-17
 *      Author: Halo9Pan
 */

#include "_init.h"

hpc_framework blas_framework;

int
hpc_handle_initialize (hpc_handle* blas_handle)
{
  cl_int err = CL_SUCCESS;
  cl_command_queue command_queue = clCreateCommandQueue (blas_framework.context.ocl, blas_framework.device.ocl, 0, &err);
  if (err != CL_SUCCESS)
	{
	  printf ("clCreateCommandQueue() failed with %d\n", err);
	  return err;
	}
  blas_handle->command_queue.ocl = command_queue;
  for (uint8_t i = 0; i < blas_handle->memory_length; ++i) {
	  cl_mem buffer = clCreateBuffer (blas_framework.context.ocl, CL_MEM_READ_WRITE, blas_handle->memory_size[i], NULL, &err);
	  if (err != CL_SUCCESS)
		{
		  printf ("clCreateBuffer() failed with %d\n", err);
		  return err;
		}
	  blas_handle->memory[i].ocl = buffer;
  }
  return err;
}

clblasStatus
hpc_blas_initialize (void)
{
  /* Setup clblas. */
  clblasStatus status = clblasSetup ();
  return status;
}

/* Release OpenCL working objects. */
cl_int
hpc_handle_finalize (hpc_handle* blas_handle)
{
  cl_int err = clReleaseCommandQueue (blas_handle->command_queue.ocl);
  for (uint8_t i = 0; i < blas_handle->memory_length; ++i) {
	  /* Release OpenCL memory objects. */
	  err = clReleaseMemObject(blas_handle->memory[i].ocl);
	  if (err != CL_SUCCESS)
		{
		  printf ("clReleaseMemObject() failed with %d\n", err);
		  return err;
		}
  }
  return err;
}

void
hpc_blas_finalize (void)
{
  /* Finalize work with clblas. */
  clblasTeardown ();
}