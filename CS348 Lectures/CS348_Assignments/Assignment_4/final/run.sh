lex ans.l
yacc -d ans.y
cc lex.yy.c y.tab.c
