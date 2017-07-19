#ifndef MATCH_H
#define MATCH_H

class Match {
public:
	Match(){}
	std::string name = "None";
	std::string state = "None";
	std::string sport = "None";
	std::string dport = "None";
	std::string src_range = "None";
	std::string dst_range = "None";
	std::string mac_source = "None";
};
#endif
