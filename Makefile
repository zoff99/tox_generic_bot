TARGET = tox_generic_bot

all: $(TARGET)

$(TARGET):
	$(CC) -o $(TARGET) -O3 \
                tox_generic_bot.c \
		-g -fPIC -lsodium -pthread

static:
	$(CC) -o $(TARGET)_static -O3 \
                tox_generic_bot.c \
		-g -fPIC -Wl,-Bstatic -lsodium -Wl,-Bdynamic -pthread

clean:
	rm -f $(TARGET) $(TARGET)_static
