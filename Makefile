CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -Wpedantic

TEST_SRCS := $(wildcard tests/*_test.cpp)

ifeq ($(TEST),)
    SELECTED_SRCS := $(TEST_SRCS)
else
    SELECTED_SRCS := tests/$(TEST).cpp
endif

TEST_BINS := $(patsubst tests/%.cpp,build/%,$(SELECTED_SRCS))

.PHONY: tests clean

tests: $(TEST_BINS)
	@failed=""; \
	total=0; \
	passed=0; \
	for bin in $(TEST_BINS); do \
		total=$$((total+1)); \
		echo "========== $$bin =========="; \
		./$$bin; \
		if [ $$? -eq 0 ]; then \
			passed=$$((passed+1)); \
		else \
			name=$$(basename $$bin); \
			failed="$$failed $$name.cpp"; \
		fi; \
	done; \
	echo "Summary: $$passed/$$total tests passed."; \
	if [ -z "$$failed" ]; then \
		echo "All tests passed."; \
	else \
		echo "Failed test files:"; \
		for f in $$failed; do \
			echo "  - $$f"; \
		done; \
		exit 1; \
	fi

build/%: tests/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf build *.out *.o *.exe