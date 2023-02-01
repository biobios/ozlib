HEADERS = cstdint cstddef new
INCLUDEDIR = ./include/
SRCDIR = ./src/

PHONY: setup copyheaders

copyheaders: $(addprefix $(INCLUDEDIR), $(HEADERS))

$(INCLUDEDIR)%: $(SRCDIR)%
	cp $< $@


setup:
	mkdir -p include
	mkdir -p lib

