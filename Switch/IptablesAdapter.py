import iptc

class IptablesAdapter:
	def __init__(self):
		self.printAllRules()
	#def addRule(self, rule):
	#def deleteRule(self, rule):
	def printAllRules(self):
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

	#def resetToDefault():
