# lab_gtk3
GTK 3.0 development demo

## What
Tis a interest rate calculator

## Contains
1. High performance/quality bond price simulation engine based on 2-factor gaussian model (G2++)
2. A calculator UI powered by GTK 3.0

## Dependencies:
* cmake
* gnu make
* GTK 3.0
* GTK 3.0 runtime (for windows)

## How to build:
* update submodule
```
git submodule update --init --recursive
```
* compile
```
mkdir -p build && cd build && cmake .. && make build
```
