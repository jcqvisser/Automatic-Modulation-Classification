# LaTeX Makefile
FILE=prelim




$(FILE).pdf: $(FILE).tex $(FILE).bib
	rm $(FILE).bbl
	rm $(FILE).aux
	rm $(FILE).blg
	rm $(FILE).log
	rm $(FILE).out
	pdflatex $(FILE)
	pdflatex $(FILE)
	bibtex $(FILE)
	pdflatex $(FILE)
	pdflatex $(FILE)
