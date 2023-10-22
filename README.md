# riscv-v-spec
Today there is only `copy_if_less` implementation.

# How to build 
```
make CC=$PATH_TO_RISCV_COMPILER CFLAGS="-march=rv64gcv -O3 -DNDEBUG"
make clean
```

You have to modes: naive and vectorized
to build program with navie copy_if_less implementation use -DNAIVE in CFLAGS

the driver also allow three modes:
* silent (default building) 
* with validation of vectorized implementation (pass -DVALIDATE to CFLAGS)
* with dump of results (pass -DDUMP to CFLAGS)


# How to execute
```
$PATH_TO_SPIKE/spike --isa=rv64gcv $PATH_TO_PK ./copy_if_less $YOUR_TEST
```

# Current results
For `tests/003.dat` results (we collected executed PC histogram with -g spike option and gather only `copy_if_*` instrs) for `O3` optimization level

naive copy_if:
```
17484309 instructions
```

vectorized copy_if:
```
1000006 instructions
```

As you can see, we have ~ 17,5x improvements for sew = 32 and lmul = 8

**Note:** for sew = 32 and lmul = 1 the result for vectorized copy_if would be `8000006`
