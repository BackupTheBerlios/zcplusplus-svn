// Preprocess_macro_novar_concatenate.h

#define WELCOME(A) A te##st
#define WELCOME2 te##st

WELCOME(A)
WELCOME2
