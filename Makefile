
TARGET  := test

CFLAGS := -Iuthash/include -lpthread -g
LDFLAGS := 

SRC_PATH = .
OBJ_PATH = .
SOURCES += $(patsubst %.c,%.c,$(notdir $(wildcard $(SRC_PATH)/*.c)))
OBJS += $(addprefix $(OBJ_PATH)/, $(SOURCES:.c=.o))

.PHONY : clean

${TARGET}:${OBJS}
	${CC} ${CFLAGS} test.c $<  -o ${TARGET}

clean:
	rm -rf ${TARGET} ${OBJS} bintest

