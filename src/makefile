sources 	= $(shell fd '.*\.cpp')
objs 		= $(patsubst %.cpp, %.o, $(sources))

.PHONY : all, clean

all : $(objs)

clean :
	rm -f $(objs)

%.o : %.cpp
	$(cc) $(cc_flags) -fPIC -o $@ -c $<