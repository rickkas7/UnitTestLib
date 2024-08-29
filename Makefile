

CFLAGS=-std=c++17

libwiringgcc.a : helpers.o spark_wiring_json.o jsmn.o spark_wiring_print.o spark_wiring_string.o spark_wiring_time.o spark_wiring_variant.o time_compat.o
	ar rcs $@ $^
	
	
test1 : libwiringgcc.a
	gcc test1.cpp $(CFLAGS) libwiringgcc.a -lc++ -o test1
	 
%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $<

clean :
	rm *.o *.a test1 libwiringcc.a || set status 0