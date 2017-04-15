CC = xelatex
FLAGS = -shell-escape

all: rapport.pdf mostlyclean

rapport.pdf: rapport.tex ../workspace/ghostbuster_02/src/*.c
	$(CC) $(FLAGS) rapport.tex

mostlyclean:
	rm -rf _minted-rapport *.log *.aux

clean:
	rm -rf rapport.pdf _minted-rapport *.log *.aux
