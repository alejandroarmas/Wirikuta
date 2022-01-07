all: matrix_multiply run_unit_tests

CC = clang
# CFLAGS = -g -Werror -Wall -I include -I shared/include -I/usr/local/opt/openssl@1.1/include
CFLAGS = -g -Werror -Wall -I include -std=c++1z

# LDFLAGS = -L/usr/local/opt/openssl@1.1/lib -lssl -lcrypto -pthread -fopencilk -ldl 
LDFLAGS = -L/include -lstdc++ -lm
 

VPATH = shared

MAIN = main.o
OBJS = main.o mm.o generator.o matrix_printer.o functions.o network_layer.o m_algorithms.o
OBJS_FOR_UNIT_TEST = $(foreach obj, $(OBJS), $(filter-out $(MAIN), $(wildcard *.o))) 


UNIT_TEST_DIRS = ./unittests/
UNIT_TESTING_MAIN = ./unittests/test_all.cpp
UNIT_TESTS_CPP = $(foreach dir,$(UNIT_TEST_DIRS),$(filter-out $(UNIT_TESTING_MAIN), $(wildcard $(dir)*.cpp))) 
UNIT_TESTING_MAIN_OBJ = $(addprefix ./unittests/obj/, $(notdir $(UNIT_TESTING_MAIN:.cpp=.o)))

-include $(OBJS:.o=.d)



matrix_multiply: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS)

run_unit_tests: $(UNIT_TESTING_MAIN_OBJ) $(UNIT_TESTS_CPP) $(DEPS_OBJS_FOR_UNIT_TESTING) $(OBJS_FOR_UNIT_TEST)    
	$(CC) -static -o $@ $^ $(LDFLAGS) $(LIBS)
./unittests/obj/test_all.o: $(UNIT_TESTING_MAIN) 
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $<


%.d: %.c
	@set -e; gcc -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@;
	@[ -s $@ ] || rm -f $@

%.d: %.cpp
	@set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@;
	@[ -s $@ ] || rm -f $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f matrix_multiply *.o *~ core.* *.d run_unit_tests unittests/obj/*.o