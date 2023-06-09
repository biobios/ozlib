COMPILER = clang++
CFLAGS = -O2 -Wall -g --target=x86_64-unknown-none-elf -g -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++20 -c
HEADERS = $(filter-out %spec, $(notdir $(wildcard $(SRCDIR)*)))
INCLUDEDIR = ./include/
SRCDIR = ./src/
TEMPDIR = ./temp/
SPECDIR = $(SRCDIR)spec/
IMPLES = $(wildcard $(SPECDIR)*.cpp)
OBJECTS = $(addprefix $(TEMPDIR), $(notdir $(IMPLES:.cpp=.o)))


build:./lib/liboz.a copyheaders

./lib/liboz.a: $(OBJECTS)
	ar rcs $@ $^

$(TEMPDIR)%.o: $(SPECDIR)%.cpp
	$(COMPILER) $(CFLAGS) -o $@ -c $<

copyheaders: $(addprefix $(INCLUDEDIR), $(HEADERS))

$(INCLUDEDIR)%: $(SRCDIR)%
	mkdir -p $(dir $@)
	cp $< $@

clean:
	rm $(TEMPDIR)*
	rm $(INCLUDEDIR)*
	rm ./lib/*

setup:
	mkdir -p include
	mkdir -p lib
	mkdir -p temp

.PHONY: setup copyheaders clean build
