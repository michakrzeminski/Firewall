import iptc
import libPythonAdapter

def addRule(object):
	print "Py: Adding rule: "
	print object
	list = object.split('/')
	list_size = len(list)
	#print list_size
	
	#creating new rule
	rule = iptc.Rule()
	chain = iptc.Chain(iptc.Table(iptc.Table.FILTER), list[0])
	rule.target = iptc.Target(rule, list[1])
	if list[2] != 255:
		rule.protocol = list[2]
	if list[3] != "None":
		rule.src = list[3]
	if list[4] != "None":
		rule.dst = list[4]
	if list[5] != "None":
		rule.in_interface = list[5]
	if list[6] != "None":
		rule.out_interface = list[6]
	
	#TODO	
	#dalej sa juz match'e	
	#counter = 0
	#for item in list:
	#	counter += 1
	#	print counter
	#	if item != "None":
	#		print "do smth"
	#	else:
	#		print "None"
	try:
		chain.append_rule(rule)
		return True
	except:
		return False

def deleteRule(object):
	print "Py: Deleting rule: "
	print object
	list = object.split('/')
	rule = iptc.Rule()
	chain = iptc.Chain(iptc.Table(iptc.Table.FILTER), list[0])
	rule.target = iptc.Target(rule, list[1])
        if list[2] != "None":
                rule.protocol = list[2]
        if list[3] != "None":
                rule.src = list[3]
        if list[4] != "None":
                rule.dst = list[4]
        if list[5] != "None":
                rule.in_interface = list[5]
        if list[6] != "None":
                rule.out_interface = list[6]
	#TODO match'e
	try:
		chain.delete_rule(rule)
		return True
	except:
		return False

def printAllRules(object):
	print object
	table = iptc.Table(iptc.Table.FILTER)
	for chain in table.chains:
		print "Chain ", chain.name
		for rule in chain.rules:
			print "Rule", "proto:", rule.protocol, "src:", rule.src, "dst:", \
				rule.dst, "in:", rule.in_interface, "out:", rule.out_interface,
			print "Target:", rule.target.name,
			print "Matches:",
			for match in rule.matches:
				print match.name, match.state, match.mark, match.sport, match.dport, match.src_range, match.dst_range, match.mac_source,
			print(" ")
	return True

def getRules():
	list = []
	table = iptc.Table(iptc.Table.FILTER)
	for chain in table.chains:
		for rule in chain.rules:
			list.append(toString(rule))
	return list

def toString(rule):
	temp = ""
	temp = rule.chain.name + "/" + rule.target.name
	if rule.protocol:
		temp = temp + "/" + rule.protocol
	if rule.src:
		temp = temp + "/" + rule.src
	if rule.dst:
		temp = temp + "/" + rule.dst
	if rule.in_interface:
		temp = temp + "/" + rule.in_interface
	if rule.out_interface:
		temp = temp + "/" + rule.out_interface
	for match in rule.matches:
		temp = temp + "/" + str(match.name) + "/" + str(match.state) + "/" + str(match.sport) +"/" + str(match.dport) + "/" + str(match.src_range) + "/" + str(match.dst_range) + "/" + str(match.mac_source)
	return temp
