CLEANUP = rm -f
MKDIR = mkdir -p
TARGET_EXTENSION=out

.PHONY: clean
.PHONY: test

SRC_PATH = src/
TEST_PATH = tests/
INCLUDES_PATH = includes/
UNITY_PATH = $(TEST_PATH)unity/src/
BUILD_PATH = $(TEST_PATH)build/
DEPENDS_PATH = $(BUILD_PATH)depends/
OBJECTS_PATH = $(BUILD_PATH)objs/
RESULTS_PATH = $(BUILD_PATH)results/

BUILD_SRC_PATH = $(BUILD_PATH) $(DEPENDS_PATH) $(OBJECTS_PATH) $(RESULTS_PATH)

TEST_FILES = $(wildcard $(TEST_PATH)*.c)
TEST_SRC_FILES = $(SRC_PATH)floatUtility.c $(SRC_PATH)trigUtility.c

COMPILE=gcc -c
LINK=gcc
DEPEND=gcc -MM -MG -MF
MATH_LIB = -lm
CFLAGS=-I. -I$(UNITY_PATH) -I$(SRC_PATH) -I$(TEST_PATH) -I$(INCLUDES_PATH) -DTEST

RESULTS = $(patsubst $(TEST_PATH)Test%.c,$(RESULTS_PATH)Test%.txt,$(TEST_FILES) )
OBJS    = $(patsubst $(SRC_PATH)%.c,$(OBJECTS_PATH)%.o,$(TEST_SRC_FILES))

PASSED = `grep -s PASS $(RESULTS_PATH)*.txt`
FAIL = `grep -s FAIL $(RESULTS_PATH)*.txt`
IGNORE = `grep -s IGNORE $(RESULTS_PATH)*.txt`

# all: $(BUILD_PATH)/TestInjector.$(TARGET_EXTENSION)

test: $(BUILD_SRC_PATH) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

$(RESULTS_PATH)Test%.txt: $(BUILD_PATH)Test%.$(TARGET_EXTENSION)
	-./$< > $@ 2>&1

$(BUILD_PATH)Test%.$(TARGET_EXTENSION): $(OBJECTS_PATH)Test%.o $(OBJS) $(OBJECTS_PATH)unity.o
	$(LINK) -o $@ $^ $(CFLAGS)

$(OBJECTS_PATH)%.o:: $(TEST_PATH)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(OBJECTS_PATH)%.o:: $(SRC_PATH)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(OBJECTS_PATH)%.o:: $(UNITY_PATH)%.c $(UNITY_PATH)%.h
	$(COMPILE) $(CFLAGS) $< -o $@

$(DEPENDS_PATH)%.d:: $(TEST_PATH)%.c | $(DEPENDS_PATH)
	$(DEPEND) $@ $<

$(DEPENDS_PATH):
	$(MKDIR) $(DEPENDS_PATH)

$(OBJECTS_PATH): 
	$(MKDIR) $(OBJECTS_PATH)

$(RESULTS_PATH):
	$(MKDIR) $(RESULTS_PATH)

$(BUILD_PATH):
	$(MKDIR) $(BUILD_PATH)

clean:
	$(CLEANUP) $(OBJECTS_PATH)*.o
	$(CLEANUP) $(BUILD_PATH)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(RESULTS_PATH)*.txt
testprint:
	@make test
	# cat tests/build/results/TestFloatUtility.txt
.PRECIOUS: $(BUILD_PATH)Test%.$(TARGET_EXTENSION)
.PRECIOUS: $(DEPENDS_PATH)%.d
.PRECIOUS: $(OBJECTS_PATH)%.o
.PRECIOUS: $(RESULTS_PATH)%.txt
.PRECIOUS: testprint
.PRECIOUS: test