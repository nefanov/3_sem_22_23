duplex-pipe:
	gcc ../source/duplex-pipe.c ../source/duplex-pipe.h ../source/main.c -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -o duplex-pipe.out 

%.o:%.c
	gcc -c -o $@ $<