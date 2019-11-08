ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

all: a.out

.PHONY: run

run: a.out
	./a.out $(RUN_ARGS)

test: a.out
	python test.py