shell-wrapper: ./source/shell-wrapper_func.c ./source/shell-wrapper_func.h ./source/main.c
	gcc ./source/shell-wrapper_func.c ./source/shell-wrapper_func.h ./source/main.c -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -o shell-wrapper.o

%.o:%.c
	gcc -c -o $@ $<