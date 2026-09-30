SHELL := bash

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -I.
BUILD_DIR := .build

.DEFAULT_GOAL := help

.PHONY: help q1 q1-extra q1-deep q2 q2-extra q2-deep q3 q3-extra q3-deep q4 q4-extra q4-deep q5 q5-extra q5-deep all all-deep clean check-q1 check-q2 check-q3 check-q4 check-q5

help:
	@echo "BKMove Assignment 1"
	@echo ""
	@echo "  make q1        Test Question 1 - SLinkedList"
	@echo "  make q1-extra  Stress & extra tests for Question 1"
	@echo "  make q2        Test Question 2 - CircularLinkedList"
	@echo "  make q2-extra  Stress & extra tests for Question 2"
	@echo "  make q3        Test Question 3 - BusRoute"
	@echo "  make q3-extra  Stress & extra tests for Question 3"
	@echo "  make q4        Test Question 4 - QuickSort"
	@echo "  make q4-extra  Stress & extra tests for Question 4"
	@echo "  make q5        Test Question 5 - BKMove"
	@echo "  make q5-extra  Stress & extra tests for Question 5"
	@echo "  make all       Run all available standard and extra tests"
	@echo "  make clean     Remove generated binaries"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# -----------------------------------------------------------------------------
# Basic source-rule checks. Hidden grading may enforce these rules more strictly.
# -----------------------------------------------------------------------------
check-q1:
	@if sed -E 's,//.*$$,,' SLinkedList.h | grep -En '#[[:space:]]*include[[:space:]]*<(vector|list|forward_list|deque)>|\bstd::(vector|list|forward_list|deque)\b|\b(vector|list|forward_list|deque)[[:space:]]*<' >/dev/null; then \
		echo "[RULE CHECK FAILED] Q1: STL sequence containers are not allowed in SLinkedList.h"; \
		exit 1; \
	fi

check-q2:
	@if sed -E 's,//.*$$,,' CircularLinkedList.h | grep -En '#[[:space:]]*include[[:space:]]*<(vector|list|forward_list|deque)>|\bstd::(vector|list|forward_list|deque)\b|\b(vector|list|forward_list|deque)[[:space:]]*<' >/dev/null; then \
		echo "[RULE CHECK FAILED] Q2: STL sequence containers are not allowed in CircularLinkedList.h"; \
		exit 1; \
	fi

check-q3:
	@if sed -E 's,//.*$$,,' BusRoute.cpp | grep -En '#[[:space:]]*include[[:space:]]*<(vector|list|forward_list|deque)>|\bstd::(vector|list|forward_list|deque)\b|\b(vector|list|forward_list|deque)[[:space:]]*<' >/dev/null; then \
		echo "[RULE CHECK FAILED] Q3: STL sequence containers are not allowed in BusRoute.cpp"; \
		exit 1; \
	fi

check-q4:
	@if sed -E 's,//.*$$,,' QuickSort.h | grep -En '\bstd::sort[[:space:]]*\(|\bstd::stable_sort[[:space:]]*\(|\bqsort[[:space:]]*\(' >/dev/null; then \
		echo "[RULE CHECK FAILED] Q4: standard sorting functions are not allowed in QuickSort.h"; \
		exit 1; \
	fi

check-q5:
	@if sed -E 's,//.*$$,,' BKMove.cpp | grep -En '\b(std::)?(vector|deque|list|forward_list|array)[[:space:]]*<[[:space:]]*BusStop\b' >/dev/null; then \
		echo "[RULE CHECK FAILED] Q5: do not copy route stops into an auxiliary BusStop STL container"; \
		exit 1; \
	fi
	@if sed -E 's,//.*$$,,' BKMove.cpp | grep -En '\bstd::sort[[:space:]]*\(|\bstd::stable_sort[[:space:]]*\(|\bqsort[[:space:]]*\(' >/dev/null; then \
		echo "[RULE CHECK FAILED] Q5: use your QuickSort from Q4, not a standard sorting function"; \
		exit 1; \
	fi
	@if ! sed -E 's,//.*$$,,' BKMove.cpp | grep -Eq '\bQuickSort[[:space:]]*<'; then \
		echo "[RULE CHECK FAILED] Q5: ranking must use QuickSort from Question 4"; \
		exit 1; \
	fi

# -----------------------------------------------------------------------------
# Public question checkpoints
# -----------------------------------------------------------------------------
q1: check-q1 | $(BUILD_DIR)
	@echo "== Q1: SLinkedList =="
	@$(CXX) $(CXXFLAGS) tests/q1/test_q1.cpp -o $(BUILD_DIR)/q1
	@$(BUILD_DIR)/q1 $(TEST)

q1-extra: check-q1 | $(BUILD_DIR)
	@echo "== Q1: SLinkedList (Extra Stress & Edge Tests) =="
	@$(CXX) $(CXXFLAGS) tests/q1/test_q1_extra.cpp -o $(BUILD_DIR)/q1_extra
	@$(BUILD_DIR)/q1_extra $(TEST)

q2: check-q2 | $(BUILD_DIR)
	@echo "== Q2: CircularLinkedList =="
	@$(CXX) $(CXXFLAGS) tests/q2/test_q2.cpp -o $(BUILD_DIR)/q2
	@$(BUILD_DIR)/q2 $(TEST)

q2-extra: check-q2 | $(BUILD_DIR)
	@echo "== Q2: CircularLinkedList (Extra Stress & Edge Tests) =="
	@$(CXX) $(CXXFLAGS) tests/q2/test_q2_extra.cpp -o $(BUILD_DIR)/q2_extra
	@$(BUILD_DIR)/q2_extra $(TEST)

q3: check-q1 check-q2 check-q3 | $(BUILD_DIR)
	@echo "== Q3: BusRoute =="
	@$(CXX) $(CXXFLAGS) tests/q3/test_q3.cpp BusRoute.cpp -o $(BUILD_DIR)/q3
	@$(BUILD_DIR)/q3 $(TEST)

q3-extra: check-q1 check-q2 check-q3 | $(BUILD_DIR)
	@echo "== Q3: BusRoute (Extra Stress & Edge Tests) =="
	@$(CXX) $(CXXFLAGS) tests/q3/test_q3_extra.cpp BusRoute.cpp -o $(BUILD_DIR)/q3_extra
	@$(BUILD_DIR)/q3_extra $(TEST)

q4: check-q4 | $(BUILD_DIR)
	@echo "== Q4: QuickSort =="
	@$(CXX) $(CXXFLAGS) tests/q4/test_q4.cpp -o $(BUILD_DIR)/q4
	@$(BUILD_DIR)/q4 $(TEST)

q4-extra: check-q4 | $(BUILD_DIR)
	@echo "== Q4: QuickSort (Extra Stress & Edge Tests) =="
	@$(CXX) $(CXXFLAGS) tests/q4/test_q4_extra.cpp -o $(BUILD_DIR)/q4_extra
	@$(BUILD_DIR)/q4_extra $(TEST)

q5: check-q1 check-q2 check-q3 check-q4 check-q5 | $(BUILD_DIR)
	@echo "== Q5: BKMove =="
	@$(CXX) $(CXXFLAGS) tests/q5/test_q5.cpp BusRoute.cpp BKMove.cpp -o $(BUILD_DIR)/q5
	@$(BUILD_DIR)/q5 $(TEST)

q5-extra: check-q1 check-q2 check-q3 check-q4 check-q5 | $(BUILD_DIR)
	@echo "== Q5: BKMove (Extra Stress & Edge Tests) =="
	@$(CXX) $(CXXFLAGS) tests/q5/test_q5_extra.cpp BusRoute.cpp BKMove.cpp -o $(BUILD_DIR)/q5_extra
	@$(BUILD_DIR)/q5_extra $(TEST)

all: q1 q1-extra q2 q2-extra q3 q3-extra q4 q4-extra q5 q5-extra
	@echo ""
	@echo "All checkpoints (Q1 - Q5, Standard + Extra) passed successfully!"

q1-deep: check-q1 | $(BUILD_DIR)
	@echo "== Q1: SLinkedList (Deep Hidden Tests) =="
	@$(CXX) $(CXXFLAGS) tests/q1/test_q1_deep.cpp -o $(BUILD_DIR)/q1_deep
	@$(BUILD_DIR)/q1_deep $(TEST)

q2-deep: check-q2 | $(BUILD_DIR)
	@echo "== Q2: CircularLinkedList (Deep Hidden Tests) =="
	@$(CXX) $(CXXFLAGS) tests/q2/test_q2_deep.cpp -o $(BUILD_DIR)/q2_deep
	@$(BUILD_DIR)/q2_deep $(TEST)

q3-deep: check-q1 check-q2 check-q3 | $(BUILD_DIR)
	@echo "== Q3: BusRoute (Deep Hidden Tests) =="
	@$(CXX) $(CXXFLAGS) tests/q3/test_q3_deep.cpp BusRoute.cpp -o $(BUILD_DIR)/q3_deep
	@$(BUILD_DIR)/q3_deep $(TEST)

q4-deep: check-q4 | $(BUILD_DIR)
	@echo "== Q4: QuickSort (Deep Hidden Tests) =="
	@$(CXX) $(CXXFLAGS) tests/q4/test_q4_deep.cpp -o $(BUILD_DIR)/q4_deep
	@$(BUILD_DIR)/q4_deep $(TEST)

q5-deep: check-q1 check-q2 check-q3 check-q4 check-q5 | $(BUILD_DIR)
	@echo "== Q5: BKMove (Deep Hidden Tests) =="
	@$(CXX) $(CXXFLAGS) tests/q5/test_q5_deep.cpp BusRoute.cpp BKMove.cpp -o $(BUILD_DIR)/q5_deep
	@$(BUILD_DIR)/q5_deep $(TEST)

all-deep: q1-deep q2-deep q3-deep q4-deep q5-deep
	@echo ""
	@echo "All deep tests (Q1 - Q5) passed successfully!"

clean:
	@rm -rf $(BUILD_DIR) .grader_build

