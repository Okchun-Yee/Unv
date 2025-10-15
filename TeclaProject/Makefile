TARGET = GpioFile
OBJS = main.o sensor.o segment.o motor.o

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) -lpigpio -lpthread -lrt -lm

main.o: main.c sensor.h segment.h motor.h
	gcc -c main.c

sensor.o: sensor.c sensor.h
	gcc -c sensor.c

segment.o: segment.c segment.h
	gcc -c segment.c

motor.o: motor.c motor.h
	gcc -c motor.c

clean:
	rm -f *.o $(TARGET)
