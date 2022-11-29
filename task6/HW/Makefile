CFLAGS = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

ss:
	gcc ./src/ss_errors.c ./src/ss_config.c ./src/ss_log.c ./src/ss_main.c ./src/ss_start.c ./headers/ss_errors.h ./headers/ss_config.h ./headers/ss_log.h ./headers/ss_main.h ./headers/ss_start.h $(CFLAGS) -o ss.out -lrt