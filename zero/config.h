#ifndef CONFIG_H
#define CONFIG_H

class Config
{
public:
	Config(void);
	~Config(void);
	static const int MAX_POINT_NUM = 1000000;
	static const int MAX_EDGE_NUM = 20000000;
	static const double EPS;
	static const double INF;
};
#endif