SUBDIR=tool hardware

all : $(SUBDIR)

$(SUBDIR):ECHO
	make -C $@

ECHO:
	@echo $@


CLEANDIR:
	@for dir in $$(echo $(SUBDIR)); \
	do make -C $$dir clean ;\
	done

.PHONY : clean
clean :CLEANDIR