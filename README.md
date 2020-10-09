Compile: cc x11.cpp -I /usr/local/include -L /usr/lib/x86_64-linux-gnu/X11 -l X11 -lm -lstdc++ -ggdb    


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
