1. Build Z3 with clang
https://github.com/Z3Prover/z3/blob/master/README.md
$ git clone https://github.com/Z3Prover/z3.git
$ cd z3
$ CXX=clang++ CC=clang python scripts/mk_make.py
$ cd build/
$ make -j 4


2. build clang3.8
Don't add compiler-rt, the llvm3.8 sanitizer runtime code cannot build in ubuntu17.10
$ wget http://releases.llvm.org/3.8.0/llvm-3.8.0.src.tar.xz
$ tar -xvf llvm-3.8.0.src.tar.xz
$ cd llvm-3.8.0.src/tools/
$ wget http://releases.llvm.org/3.8.0/cfe-3.8.0.src.tar.xz
$ tar -xvf cfe-3.8.0.src.tar.xz
$ mv cfe-3.8.0.src clang
$ mkdir ~/llvmreleasebuild3.8
$ cd ~/llvmreleasebuild3.8/
$ ~/llvmreleasebuild3.8$ cmake ../llvm-3.8.0.src -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Release" -DLLVM_TARGETS_TO_BUILD="X86" -DCMAKE_VERBOSE_MAKEFILE=ON -DLLVM_OPTIMIZED_TABLEGEN=ON -DLLVM_BINUTILS_INCDIR=/home/jshi19/binutils-2.29/include -DLLVM_INSTALL_UTILS=ON -DLLVM_ENABLE_ASSERTIONS=OFF -DCMAKE_C_FLAGS=-DLLVM_ENABLE_DUMP -DCMAKE_CXX_FLAGS=-DLLVM_ENABLE_DUMP
$ make -j 4

3. build Klee
Please refer the http://klee.github.io/build-llvm38/ to build and install necessary libs
In the step 8, when merge the pull 729, please refer to the below branch to solve the conflict
https://github.com/shijunjing/klee/tree/edk2_enabling
or directly use the edk2_enabling branch:
$ git clone https://github.com/shijunjing/klee.git klee-fork
$ git checkout edk2_enabling
$ mkdir klee_build_dir
$ cd klee_build_dir
$ CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=0" CXXFLAGS="-fno-rtti" cmake \
  -DENABLE_SOLVER_Z3=ON \
  -DENABLE_SOLVER_STP=OFF \
  -DENABLE_POSIX_RUNTIME=ON \
  -DENABLE_KLEE_UCLIBC=ON \
  -DKLEE_UCLIBC_PATH=/home/jshi19/symbolicexecution/klee-uclibc/ \
  -DGTEST_SRC_DIR=/home/jshi19/symbolicexecution/googletest-release-1.7.0/ \
  -DENABLE_SYSTEM_TESTS=ON \
  -DENABLE_UNIT_TESTS=ON \
  -DLLVM_CONFIG_BINARY=/home/jshi19/llvmreleasebuild3.8/bin/llvm-config \
  -DLLVMCC=/home/jshi19/llvmreleasebuild3.8/bin/clang \
  -DLLVMCXX=/home/jshi19/llvmreleasebuild3.8/bin/clang++ \
  -DCMAKE_VERBOSE_MAKEFILE=ON \
  -DCMAKE_BUILD_TYPE="Debug" \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  /home/jshi19/symbolicexecution/klee-fork

In ~/llvm-3.8.0.src/include/llvm/IR/ValueMap.h, change the line 102:
from
  bool hasMD() const { return MDMap; }
to
  bool hasMD() const { return bool(MDMap); }

$ make
$ sudo make install
