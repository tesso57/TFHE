# TFHE 

## BUILD
```
make 
```
C++20に対応したコンパイラが必要。
## HOM NAND RESULTS
 
1. [950d5200045791a64a96555a097318266ff93887](https://github.com/tesso57/TFHE/commit/950d5200045791a64a96555a097318266ff93887)
   - Naive implementation
   - generate keys : 49.907 sec
   - calculation time : 62.905 sec
2. [405ee68c79e81c61a7a815dbe0b161a3927b67c6](https://github.com/tesso57/TFHE/commit/405ee68c79e81c61a7a815dbe0b161a3927b67c6)
   - Implement FFT (radix-2)
     - [Boost](https://www.boost.org/) needs to be installed.
   - generate keys :  20.957 sec
   - Calculation :  4.491 sec
3. [20c6ffa174fe43c79c284ece3111d82e81d36902](https://github.com/tesso57/TFHE/commit/20c6ffa174fe43c79c284ece3111d82e81d36902)
   - Implementation of FFT without Boost
   - generate keys :  20.886 sec
   - Calculation : 4.494 sec
### MACHINE INFO 

- CPU : AMD Ryzen 7 3800XT 8-Core Processor 3.89 GHz
- MEM : 32.0 GB (3200 MHz)
- OS  : Ubuntu 20.04.2 LTS (WSL2)

### LICENSE
The source code is licensed MIT.
<!-- 
template 

1. commit ID
   - Comments
   - generate keys :  
   - Calculation :  
-->
