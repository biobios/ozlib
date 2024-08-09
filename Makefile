COMPILER = clang++
CFLAGS = -O2 -Wall -g --target=x86_64-unknown-none-elf -g -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++20 -I $(INCLUDEDIR)
HEADERS = $(filter-out %bits, $(filter-out %common, $(filter-out %spec, $(notdir $(wildcard $(SRCDIR)*)))))
INCLUDEDIR = ./include/
SRCDIR = ./src/
TEMPDIR = ./temp/
TESTDIR = ./test/
SPECDIR = spec/
COMMONDIR = common/
BITSDIR = bits/
BITS = $(notdir $(wildcard $(SRCDIR)$(BITSDIR)*.hpp))
SPECSRC = $(wildcard $(SRCDIR)$(SPECDIR)*.cpp)
COMMONSRC = $(wildcard $(SRCDIR)$(COMMONDIR)*.cpp)
TESTSRC = $(wildcard $(TESTDIR)*.cpp)
SPECOBJECTS = $(addprefix $(TEMPDIR)$(SPECDIR), $(notdir $(SPECSRC:.cpp=.o)))
COMMONOBJECTS = $(addprefix $(TEMPDIR)$(COMMONDIR), $(notdir $(COMMONSRC:.cpp=.o)))
TESTOBJECTS = $(addprefix $(TEMPDIR)test/, $(notdir $(TESTSRC:.cpp=.o)))

define add_compile_commands
	@echo "{" >> compile_commands.json
	@echo "\"directory\": \"$(shell pwd)\"," >> compile_commands.json
	@echo "\"command\": \"$(COMPILER) $(CFLAGS) -o $(1:.cpp=.o) -c $(1)\"," >> compile_commands.json
	@echo "\"file\": \"$(1)\"" >> compile_commands.json
	@echo "}," >> compile_commands.json

endef

build:copyheaders copybits ./lib/libozstd.a ./lib/libcommonstd.a

./lib/libozstd.a: $(SPECOBJECTS)
	ar rcs $@ $^

./lib/libcommonstd.a: $(COMMONOBJECTS)
	ar rcs $@ $^

$(TEMPDIR)%.o: $(SRCDIR)%.cpp
	mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) -o $@ -c $<

copyheaders: $(addprefix $(INCLUDEDIR), $(HEADERS))

copybits: $(addprefix $(INCLUDEDIR)$(BITSDIR), $(BITS))

$(INCLUDEDIR)%: $(SRCDIR)%
	mkdir -p $(dir $@)
	cp $< $@

$(TEMPDIR)test/%.o: $(TESTDIR)%.cpp
	mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) -o $@ -c $<

test: $(TESTOBJECTS)
	$(foreach file, $(TESTSRC), clang-tidy -p ./ -header-filter=.* -system-headers $(file);)

clean:
	rm -rf $(TEMPDIR)*
	rm -rf $(INCLUDEDIR)*
	rm ./lib/*

setup: compile_commands.json

compile_commands.json:
	@echo "[" > compile_commands.json
	$(foreach file, $(SPECSRC), $(call add_compile_commands, $(file)))
	$(foreach file, $(COMMONSRC), $(call add_compile_commands, $(file)))
	$(foreach file, $(TESTSRC), $(call add_compile_commands, $(file)))
	@echo "{" >> compile_commands.json
	@echo "\"directory\": \"placeholder\"," >> compile_commands.json
	@echo "\"command\": \"placeholder\"," >> compile_commands.json
	@echo "\"file\": \"placeholder\"" >> compile_commands.json
	@echo "}" >> compile_commands.json
	@echo "]" >> compile_commands.json

lint:
	$(foreach file, $(SPECSRC), clang-tidy -p ./ $(file);)
	$(foreach file, $(COMMONSRC), clang-tidy -p ./ $(file);)

.PHONY: setup copyheaders copybits clean build test lint
