all: logger producer

logger: logger.c
	gcc -o logger utility-func.c logger.c

producer: logProducer.c
	gcc -o producer utility-func.c logProducer.c 

clean:
	rm logger
	rm producer