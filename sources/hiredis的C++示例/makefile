BIN = demo2

redis: $(BIN).cpp redis.h
	g++ $(BIN).cpp -o $(BIN) -L/usr/local/lib -lhiredis

clean:
	rm $(BIN).o $(BIN)
