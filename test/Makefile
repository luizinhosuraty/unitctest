# Makefile for tests
#
# Copyright (c) 2022 Luiz Henrique Suraty Filho <luiz-dev@suraty.com>
#
# SPDX-License-Identifier: MIT

KBLUE := "\e[1;36m"
KNORM := "\e[0m"

unitctest_framework := ../include/

test_build_dir := build/
test_dir := ./
test_includes := $(addprefix -I, $(unitctest_framework))
test_cflags := -g -Wall $(test_includes)
test_depfiles = $(addprefix $(test_build_dir), $(targets:=.d))

targets := assert_true_false\
	   assert_int\
	   expect_true_false\
	   expect_int\
	   fixture\
	   test_skip

all : $(targets)

$(test_build_dir)%.o : %.c Makefile | $(test_build_dir)
	@printf $(KBLUE)"---- building $@ ----\n"$(KNORM)
	$(CC) $(test_cflags) -c -MMD -MP $< -o $@

$(targets): %: $(test_build_dir)%.o
	@printf $(KBLUE)"---- linking $@----\n"$(KNORM)
	@printf "$(test_depfiles)"
	$(CC) -o $@.test $<

$(test_build_dir):
	@printf $(KBLUE)"---- create $@ dir ----\n"$(KNORM)
	mkdir -p $(test_build_dir)

.PHONY: clean
clean:
	@printf $(KBLUE)"---- cleaning ----\n"$(KNORM)
	rm -rf $(addsuffix .test, $(targets))
	rm -rf build

.PHONY: test
test: $(targets)
	@printf $(KBLUE)"---- running tests ----\n"$(KNORM)
	@for f in $^ ; do echo $$f; ./$$f.test ; done

-include $(test_depfiles)
