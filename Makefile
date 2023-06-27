COMPILER = clang++
CFLAGS = -O2 -Wall -g --target=x86_64-unknown-none-elf -g -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++20 -I $(SRCDIR)
HEADERS = $(filter-out %common, $(filter-out %spec, $(notdir $(wildcard $(SRCDIR)*))))
INCLUDEDIR = ./include/
SRCDIR = ./src/
TEMPDIR = ./temp/
SPECDIR = spec/
COMMONDIR = common/
SPECSRC = $(wildcard $(SRCDIR)$(SPECDIR)*.cpp)
COMMONSRC = $(wildcard $(SRCDIR)$(COMMONDIR)*.cpp)
SPECOBJECTS = $(addprefix $(TEMPDIR)$(SPECDIR), $(notdir $(SPECSRC:.cpp=.o)))
COMMONOBJECTS = $(addprefix $(TEMPDIR)$(COMMONDIR), $(notdir $(COMMONSRC:.cpp=.o)))


build:./lib/libozstd.a ./lib/libcommonstd.a copyheaders

./lib/libozstd.a: $(SPECOBJECTS)
	ar rcs $@ $^

./lib/libcommonstd.a: $(COMMONOBJECTS)
	ar rcs $@ $^

$(TEMPDIR)%.o: $(SRCDIR)%.cpp
	mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) -c -o $@ -c $<

copyheaders: $(addprefix $(INCLUDEDIR), $(HEADERS))

$(INCLUDEDIR)%: $(SRCDIR)%
	mkdir -p $(dir $@)
	cp $< $@

test: ./test/test.out

./test/test.out: ./test/test.cpp
	$(COMPILER) $(CFLAGS) -o $@ -c $<

clean:
	rm $(TEMPDIR)*
	rm $(INCLUDEDIR)*
	rm ./lib/*

setup:
	mkdir -p include
	mkdir -p lib
	mkdir -p temp

.PHONY: setup copyheaders clean build test
