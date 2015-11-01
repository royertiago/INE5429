finaldir := $(dir $(lastword $(MAKEFILE_LIST)))

# Stripped slash directory
finalsdir := $(patsubst %/,%,$(finaldir))

tex_src := $(wildcard $(finaldir)e*.tex)
pdf := $(tex_src:.tex=.pdf)

.PHONY: final
all: final
final: $(pdf)

$(pdf): %.pdf : %.tex
	cd $(finaldir);\
	latexmk -pdf $(notdir $<) -pdflatex="pdflatex --shell-escape %O %S"

.PHONY: final-mostlyclean
mostlyclean: final-mostlyclean
final-mostlyclean:
	cd $(finaldir) && latexmk -c

.PHONY: final-clean
clean: final-clean
final-clean:
	cd $(finaldir) && latexmk -C
