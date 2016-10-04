CC ?= gcc
CFLAGS_common ?= -Wall -std=gnu99
CFLAGS_orig = -O0
CFLAGS_opt  = -O0 -pthread -g 

ifeq ($(strip $(COMPARE)),1)
CFLAGS_opt += -DCOMPARE
endif

EXEC = phonebook_orig phonebook_opt
all: $(EXEC)

SRCS_common = main.c

file_align: file_align.c
	$(CC) $(CFLAGS_common) $^ -o $@

phonebook_orig: $(SRCS_common) phonebook_orig.c phonebook_orig.h 
	$(CC) $(CFLAGS_common) $(CFLAGS_orig) \
		-DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c

phonebook_opt: $(SRCS_common) phonebook_opt.c phonebook_opt.h  
	$(CC) $(CFLAGS_common) $(CFLAGS_opt) \
		-DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c threadpool.c

run: $(EXEC)
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	watch -d -t "./phonebook_orig && echo 3 | sudo tee /proc/sys/vm/drop_caches"

cache-test: $(EXEC)
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_orig
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_opt

output.txt: cache-test calculate
	./calculate

plot: output.txt
	gnuplot scripts/runtime.gp

calculate: calculate.c
	$(CC) $(CFLAGS_common) $^ -o $@

compare: phonebook_opt
	./phonebook_opt > compare_result

.PHONY: clean
clean:
	$(RM) $(EXEC) *.o perf.* \
	    calculate orig.txt opt.txt output.txt runtime.png \
		aligned.txt compare_result entry_words.txt
	    
