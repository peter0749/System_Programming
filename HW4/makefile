all: addGrade
addGrade: addGrade.c
	gcc addGrade.c -o addGrade -Wall
critical: addGrade.c
	gcc addGrade.c -o addGrade -DWATCH_LOCK -Wall
clean:
	rm -f addGrade

