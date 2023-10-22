#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int copy_if_less_naive(int *dst, const int *src, int x, int n);
int copy_if_less_vectorized(int *dst, const int *src, int x, int n);
/* 
  allocate memory for src. Need to delete!
  return -1 if some happens wrongm 0 -- otherwise
*/
static int init_data_from_file(const char *filename, unsigned *n, int *piv, int **src) {
  assert(filename && n && piv && src);
  FILE *input;
  int err;

  if (!(input = fopen (filename, "r"))) {
    fprintf (stderr, "Can't open %s test\n", filename);
    fclose(input);
    return -1;
  }      
  
  err = fscanf(input, "%u%d", n, piv);
  if (err != 2) {
    fprintf(stderr, "wrong input\n");
    fclose(input);
    return -1;
  }

#ifdef DUMP
  printf("n : %u\n", *n);
  printf("piv : %d\n", *piv);
#endif

  *src = (int *) calloc(*n, sizeof(int));
  assert(*src && "can't allocate enough memory for src");

  for (unsigned i = 0; i < *n; ++i) {
    err = fscanf(input, "%d", *src + i);
    if (err != 1) {
      fprintf(stderr, "wrong data input\n");
      free(*src);
      fclose(input);
      return -1;
    } 
  }
  fclose(input);
  return 0;
}

/* 
  check vectorized implementation is correct. 
  returns 0, if correct, -1 -- otherwise
*/ 
#ifdef VALIDATE
static int validate(const int *to_validate_dst, int to_validate_k, const int *src, int piv, int n) {
  // we suppose that the naive algorithm is correct
  int k_naive;
  int *dst_naive = (int *) calloc(n, sizeof(int));
  assert(dst_naive && "can't allocate enough memory for validation");
  
  k_naive = copy_if_less_naive(dst_naive, src, piv, n);
  printf("--- start to validate...\n");
  
  if (k_naive != to_validate_k) {
    fprintf(stderr, "Error: total copied sizes differ:\n"
                    "\tnaive : %d\n"
                    "\tvectorized: %d\n", k_naive, to_validate_k);
    return -1;
  }

  assert(k_naive == to_validate_k);
  for (int i = 0; i < to_validate_k; ++i) 
    if (to_validate_dst[i] != dst_naive[i]) {
      fprintf(stderr, "copied arrays differ\n"
              "the first mismatch on %d-th position\n", i);
      return -1;
    }

  printf("correct\n");
  return 0;
}
#endif

#ifdef DUMP
static void dump(const int *arr, int size) {
  for (int i = 0; i < size; ++i)
    printf("%d ", arr[i]);
  printf("\n");
}
#endif 

int main(int argc, char **argv) {
  unsigned n;
  int piv, k;
  int *src, *dst;

  if (argc != 2) {
    fprintf (stderr, "Usage: ./copy_if test_file\n");
    return -1;
  }

  if (init_data_from_file(argv[1], &n, &piv, &src))
    return -1;

  dst = (int *) calloc(n, sizeof(int));
  assert(dst && "can't allocate enough memory for dst");
  
#ifdef NAIVE
  k = copy_if_less_naive(dst, src, piv, n);
#else
  k = copy_if_less_vectorized(dst, src, piv, n);

#ifdef VALIDATE
  if (validate(dst, k, src, piv, n)) {
    free(src);
    free(dst);
    return -1;
  }
#endif

#ifdef DUMP  
  printf("total copied size : %d\n", k);
  dump(dst, k);
#endif

#endif  
  free(src);
  free(dst);
}
