TARGET_EXEC ?= seamer.bin

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# $(info sources are $(SRCS))
# $(info objs are $(OBJS))
# $(info deps are $(DEPS))

CC = /usr/bin/gcc
CFLAGS ?= -lm -g

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	# $(info from linker $(@))

	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(CFLAGS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	# $(info from c source $(<), $(@))


	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(info CLEANING)
	find ./build -type f \( -name '*.o' -o -name "*.bin" -o -name "*.dis" -o -name ".out" -o -name "*.c.d" -o -name "*.out" \) -delete


-include $(DEPS)

MKDIR_P ?= mkdir -p
