#ifndef __T20_TYPE_H__
#define __T20_TYPE_H__

enum t20_soc_type {
	T20L,
	T20N,
	T20X,
	T20M,
	T20Z,
	T20_TYPE_END,
};

char *t20_soc_type_str[T20_TYPE_END] = {
	"T20L",
	"T20N",
	"T20X",
	"T20M",
	"T20Z",
};

int get_t20_type(void);

#endif
