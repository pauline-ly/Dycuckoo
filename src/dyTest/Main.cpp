//
// Created by jing on 2018/7/1.
//

#include "../dyhash/cuckoo.h"
#include "api.h"
#include "../../include/mt19937ar.h"
#include <stdlib.h>
#include <cuda_profiler_api.h>

using namespace std;
#define NUM_DATA 100000000


unsigned int *read_data(char *filename, int size) {
  FILE *fid;
  fid = fopen(filename, "rb");
  auto data = (unsigned int *) malloc(sizeof(unsigned int) * size);//申请内存空间，大小为n个int长度

  if (fid == NULL) {
    printf("the data file is unavailable.\n");
    exit(1);
  }

  fread(data, sizeof(unsigned int), size, fid);
  fclose(fid);
  return data;
}


bool test_size_new_api(int pool_size, int num_less, int num_more, char *filename) {
  int batch_num = pool_size / BATCH_SIZE;
  int batch_every_time = batch_num / max(num_less, num_more);
  int num_search = 10;

  cudaDeviceReset();
  checkCudaErrors(cudaGetLastError());

  TYPE *key = read_data(filename, pool_size);
  auto *value = new TYPE[pool_size];

  for (TYPE i = 0; i < pool_size; i++) {
    value[i] = i + 1;
  }

  hashAPI h(TABLE_NUM * CHANGE * 10);

  /// pool init
  h.set_data_to_gpu(key, value, pool_size);

  int i;
  GpuTimer time;
  time.Start();
  for (i = 0; i < batch_every_time / 2 + 1; i++) {
    for (int j = 0; j < num_more; j++)
      h.hash_insert_batch();
    for (int j = 0; j < num_search; j++)
      h.hash_search_batch();

    for (int j = 0; j < num_less; j++)
      h.hash_delete_batch();
  }

  int t_batch = i;

  h.batch_reset();
  for (i = 0; i < batch_every_time / 2 - 1; i++) {
    for (int j = 0; j < num_less; j++)
      h.hash_insert_batch();
    for (int j = 0; j < num_search; j++)
      h.hash_search_batch();
    for (int j = 0; j < num_more; j++)
      h.hash_delete_batch();
  }
  t_batch += i;
  time.Stop();
  double diff = time.Elapsed() * 1000000;
  auto mops = (double) (BATCH_SIZE * (num_more + num_search + num_less) *
                        (batch_every_time / 2 + 1 + batch_every_time / 2 - 1)) / diff;
  printf("%d %.2lf %.2f ", num_less, (double) diff, mops);
  checkCudaErrors(cudaGetLastError());

  delete[] key;
  delete[] value;
}

// size num_less num_more data
int main(int argc, char **argv) {
  if (argc < 2) {
    printf("run using : ./linear file_neme");
  }

  int size = atoi(argv[1]);
  int num_less = atoi(argv[2]);
  int num_more = atoi(argv[3]);
  char *file_name = argv[4];
  test_size_new_api(size, num_less, num_more, file_name);

  return 0;
}



