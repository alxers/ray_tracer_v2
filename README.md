Compile: make main
Clean and compile: make clean && make main

Run and write to a file: ./main > test.ppm
Run and write to a console and file: ./main 2>&1 | tee test.ppm
