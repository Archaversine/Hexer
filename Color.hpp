#ifndef __COLOR_H__
#define __COLOR_H__

#define RESET  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"

#define DIM "\x1B[2m"
#define INVERT "\x1B[7m"

#define FRED(x) KRED x RST
#define FGREEN(x) KGRN x RST
#define FYELLOW(x) KYEL x RST
#define FBLUE(x) KBLU x RST
#define FMAGENTA(x) KMAG x RST
#define FCYAN(x) KCYN x RST
#define FWHITE(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

#endif // __COLOR_H__
