
## Environment and Dependency

This code is developed and tested on:

Centos 7.2
Tesla P100-PCIE
CUDA 9.1



## Build

To build this demo, you need to change the CUDA-related configuration, such as nvcc path, include path, compilation architecture, etc.
It is recommended to use `mkdir build` to create a new folder to place compiled files and so on, and then use `cmake .. && make` to compile

## Demo

`./Dyhash [size] [num_less] [num_more] [data_file_path]`

The size is the size of data set.
The data_file_path is the data set that store the key.

The operation flow is as follows:
1.Three copies of the data set of the size of the key-value pair are first used to perform insert, search and delete operations respectively. 
For each piece of data, it is divided into batches. For the data sets used to perform insert and search operations, each batch contains num_more key-value pairs.
For the data set used to perform the delete operation, each batch contains num_less key-value pairs. 

2.Then iterate through the batches, select the data in each batch in order to perform the insert operation, the search operation, and the delete operation.

3.The three data copied in the first step are still used. For each piece of data, it is divided into multiple batches. 
For the dataset used to perform the search and delete operations, each batch contains num_more key-value pairs.
For the dataset used to perform the insert operation, each batch contains num_less key-value pairs. 

4.Then iterate through the batches, select the data in each batch in order to perform the insert operation, the search operation, and the delete operation.
