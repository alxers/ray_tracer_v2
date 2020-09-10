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
continue (c)

Can also print expressions (and functions, f.e. p foo(1))
call foo(1)

Debug included files:
(gdb) b box.h:42
Breakpoint 1 at 0x16b9: file box.h, line 43.

Links:
http://tavianator.com/2011/ray_box.html     
https://www.kevinbeason.com/smallpt/     
https://nim-lang.org/blog/2020/06/30/ray-tracing-in-nim.html     
https://blog.johnnovak.net/2016/10/22/the-nim-raytracer-project-part-4-calculating-box-normals/     
http://jcgt.org/published/0002/02/02/paper.pdf (BVH)     
