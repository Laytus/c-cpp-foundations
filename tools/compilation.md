# Compilation

## C (baseline)
```bash
clang -std=c17 -Wall -Wextra -Werror -O0 -g file.c -o file
```

## Sanitizers (macOS)
```bash
clang -std=c17 -Wall -Wextra -Werror -O0 -g \
  -fsanitize=address,undefined file.c -o file
```