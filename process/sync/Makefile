OBJECTS = producer_consumer_main.o
TARGET = producer_consumer_main
MAIN_HEADER = producer_consumer_main.h
## HEADER_FILE will handle the header file
HEADER_FILE = producer_consumer_01.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ 	
## [$@] : all targets
## [$^] : all depends

$(OBJECTS): producer_consumer_main.c $(MAIN_HEADER)
#$(CC) -c  producer_consumer_main.c $(MAIN_HEADER)
$(MAIN_HEADER): get_header

get_header:
	@cp $(HEADER_FILE)   producer_consumer_main.h
clean:
	rm -f $(OBJECTS) $(TARGET) producer_consumer_main.h
.PHONY: all clean
	
