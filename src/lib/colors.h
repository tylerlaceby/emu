#ifndef _COLORS_
#define _COLORS_

/* FOREGROUND */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define F_RED(x) KRED x RST
#define F_GREEN(x) KGRN x RST
#define F_YELLOW(x) KYEL x RST
#define F_BLUE(x) KBLU x RST
#define F_MAGENTA(x) KMAG x RST
#define F_CYAN(x) KCYN x RST
#define F_WHITE(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

#endif  /* _COLORS_ */
