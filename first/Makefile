X = first

all: $(X)

$(X): $(X).c
	gcc -Wall -Werror -fsanitize=address $(X).c -o $(X)

clean:
	rm -rf $(X)
