Compile: make main    
Clean and compile: make clean && make main    

Run and write to a file: ./main > test.ppm    
Run and write to a console and file: ./main 2>&1 | tee test.ppm   

make clean && make main && ./main 2>&1 | tee test.ppm     


GDB:     
gdb main     
break line_number (or b)     
run     

print var (or p)     
bt (for backtrace)     
next (n)     
step (s)     

Can also print expressions (and functions, f.e. p foo(1))
call foo(1)
