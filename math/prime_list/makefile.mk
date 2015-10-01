PRIMEDIR := $(dir $(lastword $(MAKEFILE_LIST)))

PRIME := $(PRIMEDIR)generate_list
PRIMESRC := $(PRIMEDIR)generate_list.cpp $(PRIMEDIR)list.cpp

# We will only request compilation of generate_list.o.
# We must not request also the generation of generate_list
# because the main makefile makes every program
# depends on every object file in $(obj).
# Thus, $(PRIME) would depend on list.o,
# which in turn would depend on $(PRIME)
# --- a circular dependency.
#
# Thus, we need to manage the building of $(PRIME) directly.
src += $(PRIMESRC)
dep += $(PRIMESRC:.cpp=.dep.mk)
obj += $(PRIMEDIR)list.o

$(PRIMEDIR)list.cpp: $(PRIME)
	$(PRIME) > $@

# Make $(PRIME) depend on $(PRIME).o will automatically link $(PRIME)
# whenever the correspending object file is updated.
# Since the main makefile will manage $(PRIME).o's source dependencies,
# this will manage indirectly $(PRIME)'s source dependencies.
$(PRIME): $(PRIME).o
	$(link_main)

.PHONY: prime-list-clean prime-list-mostlyclean

mostlyclean: prime-list-mostlyclean

prime-list-mostlyclean:
	find $(TESTDIR) -name '*.o' -exec rm {} +

clean: prime-list-clean
prime-list-clean: prime-list-mostlyclean
	rm -f $(PRIME) $(PRIMEDIR)list.cpp
