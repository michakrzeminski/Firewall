#ifndef RULE_H
#define RULE_H
#include <list>
#include "Match.h"

class Rule {
public:
	Rule(){}
	std::string chain = "None";
	std::string target = "None";
	std::string protocol = "None";
	std::string src = "None";
	std::string dst = "None";
	std::string in_interface = "None";
	std::string out_interface = "None";
	std::list<Match> matches;
	std::string toString() {
		std::string ret;
		ret+=chain+"/"+target+"/"+protocol+"/"+src+"/"+dst+"/"+in_interface+"/"+out_interface;
		for (auto match : matches) {
			ret+="/"+match.name+"/"+match.state+"/"+match.sport+"/"+match.dport+"/"+match.src_range+"/"+match.dst_range+"/"+match.mac_source;
		}
		return ret;
	}
};
#endif
