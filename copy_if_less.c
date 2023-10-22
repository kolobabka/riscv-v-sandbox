#include <riscv_vector.h>

int copy_if_less_naive(int *dst, const int *src, int x, int n) {
  int k = 0;
  for (int i = 0; i < n; ++i)
    if (src[i] < x)
      dst[k++] = src[i];
  return k;
}

int copy_if_less_vectorized(int *dst, const int *src, int x, int n) {
  int k = 0;
  size_t vl = __riscv_vsetvl_e32m8(n); 
  vint32m8_t vpiv =  __riscv_vmv_v_x_i32m8(x, vl);
  
  for (; n > 0; n -= vl, src += vl) {
    vl = __riscv_vsetvl_e32m8(n);

    vint32m8_t vs = __riscv_vle32_v_i32m8(src, vl);
    vbool4_t mask = __riscv_vmslt_vv_i32m8_b4(vs, vpiv, vl);
    vint32m8_t vd = __riscv_vcompress_vm_i32m8(vs, mask, vl);
  
    unsigned long popcnt = __riscv_vcpop_m_b4(mask, vl);

    __riscv_vse32_v_i32m8(dst, vd, popcnt);
    k += popcnt;
    dst += popcnt;
  }

  return k;
}
