NAME = example
CFLAGS = -Wall -Wextra -pedantic -std=c11
OFLAG = -O2
DBFLAGS = -fsanitize=address,undefined

main:
	$(CC) example.c -o $(NAME) $(CFLAGS) $(OFLAG)


debug:
	$(CC) example.c -o $(NAME) $(CFLAGS) $(OFLAG) $(DBFLAGS)

clean:
	rm -rf $(NAME)
