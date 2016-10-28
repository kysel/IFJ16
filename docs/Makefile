# Interpretr jazyka IFJ16

# Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
# Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
# Korček Juraj <xkorce01@stud.fit.vutbr.cz>
# Kubica Jan <xkubic39@stud.fit.vutbr.cz>
# Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>

file = dokumentace

make: $(file).tex
	 latex $(file).tex
	 latex $(file).tex
	 dvips $(file).dvi
	 ps2pdf $(file).ps
clean:
	 rm -f ${file}.{ps,pdf,log,aux,out,dvi,bbl,blg,toc,nav,snm}
