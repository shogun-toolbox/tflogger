# TFLogger
This is a simple library that has been carved out of [TensorFlow](https://www.tensorflow.org/)'s codebase for being able to generate the required output format for [TensorBoard](https://www.tensorflow.org/get_started/summaries_and_tensorboard).

![](https://github.com/shogun-toolbox/tflogger/workflows/C/C++%20CI/badge.svg)

## Building

In order to build from source the following packages are required:
  - cmake
  - protobuf

To build the library simply run the following commands:
```
git clone https://github.com/shogun-toolbox/tflogger.git
cd tflogger
mkdir build
cd build
cmake ..
make
```

### Testing
In order to enable the unit tests add the `-DBUILD-TESTS=ON` cmake option to the cmake command. This will automatically create a target for building the unit tests and adds it to the ctest targets.

In other words to run the unit tests:
```
cmake -DBUILD-TESTS=ON [other cmake flags] ..
make
ctest
```

### Python interface
Currently only the python wrapper is available, but further targets could be specified using SWIG.

For building the python wrapper you need to add the `-DINTERFACE_PYTHON=ON` cmake flag to the cmake command:
```
cmake -DINTERFACE_PYTHON=ON ..
```

## Using
Examples of how to use the library are available under [example](examples) directory.
