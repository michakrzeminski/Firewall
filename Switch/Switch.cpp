#include "Switch.h"
#include "PythonAdapter.h"

Switch* Switch::instance = nullptr;

Switch* Switch::getInstance() {
    if (!instance) {
        instance = new Switch;
    }
    return instance;
}

Switch::Switch() {
    std::cout << "creating switch" << std::endl;
    init();
}

void Switch::init() {
    std::cout << "switch init" <<std::endl;
    std::thread(&Switch::sniff, this).detach();
    PythonAdapter::getInstance();
    Client * client = Client::getInstance();
    client->init();
    std::thread(&Client::read, client).detach();
    Protocol hello(HELLO);
    client->send(hello);

    std::thread(&Switch::bufferHandling, this).detach();
}

void Switch::sniff() {
    sniffer = new Tins::Sniffer("eth0");
    sniffer->sniff_loop(make_sniffer_handler(this, &Switch::callback));
}

bool Switch::callback(Tins::PDU &pdu) {
    const Tins::IP &ip = pdu.rfind_pdu<Tins::IP>();
    std::cout<< "from "<<ip.src_addr() <<" to "<<ip.dst_addr()<<" protocol "<<ip.protocol()<<std::endl;
	//miec jakos zebrana pule adresow czy cos
	//Tins::Ipv4Address lo("127.0.0.1");
	//Tins::HWAddress<6> hw_addr("01:de:22:01:09:af");
	//i porownywac to co dostajemy moze z pula zapisana
    if(!analyzePacket(ip)) {
        //do buffora
        if(packetBuffer.size() <= NUM_PACKETS) {
            packetBuffer.push_back(ip);
        }
    }
    return true;
    //zeby zakonczyc sniffowanie
    //return false
}

bool Switch::analyzePacket(const Tins::IP &ip) {
    for(auto i : packetMap) {
        if(i.ip.src_addr() == ip.src_addr() && i.ip.dst_addr() == ip.dst_addr() && i.ip.protocol() == ip.protocol())
            return true;
    }
    return false;
}

void Switch::bufferHandling() {
    if(packetBuffer.size() != 0 ) {
        auto tempPacket = packetBuffer.front();
        packetBuffer.pop_front();
        try {
            Protocol toCheck(CHECK);
            toCheck.packet = &tempPacket;
            Client::getInstance()->send(toCheck);
        }
        catch(std::exception &e) {
            std::cout<<e.what();
        }
    }
}
