#include "Switch.h"
#include "PythonAdapter.h"
#include <boost/algorithm/string.hpp>

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
    std::thread(&Switch::sniffEth, this).detach();
    std::thread(&Switch::sniffWlan, this).detach();
    PythonAdapter::getInstance();
    fillinPacketmap();
    //Client * client = Client::getInstance();
    client->init();
    std::thread(&Client::read, client).detach();
    client->send(Protocol(HELLO));
    std::thread(&Switch::bufferHandling, this).detach();
}

void Switch::sniffEth() {
    eth_sniffer = new Tins::Sniffer("eth0");
    eth_sniffer->sniff_loop(make_sniffer_handler(this, &Switch::ethCallback));
}

void Switch::sniffWlan() {
    wlan_sniffer = new Tins::Sniffer("wlan0");
    wlan_sniffer->sniff_loop(make_sniffer_handler(this, &Switch::wlanCallback));
}

bool Switch::ethCallback(Tins::PDU &pdu) {
    const Tins::IP &ip = pdu.rfind_pdu<Tins::IP>();
    //std::cout<< "from "<<ip.src_addr() <<" to "<<ip.dst_addr()<<" protocol "<<ip.protocol()<<std::endl;

    if(!analyzePacket(ip)) {
        //do bufora
        if(packetBuffer.size() <= NUM_PACKETS) {
            //std::cout<<"do bufora"<<std::endl;
            packetBuffer.push_back(std::make_tuple(getChain(ip),(int)ip.protocol(),ip.src_addr().to_string(),ip.dst_addr().to_string()));
        }
    }
    return true;
    //zeby zakonczyc sniffowanie
    //return false
}

bool Switch::wlanCallback(Tins::PDU &pdu) {

    const Tins::IP &ip = pdu.rfind_pdu<Tins::IP>();
    //std::cout<< "from "<<ip.src_addr() <<" to "<<ip.dst_addr()<<" protocol "<<ip.protocol()<<std::endl;

    if(!analyzePacket(ip)) {
        //do bufora
        if(packetBuffer.size() <= NUM_PACKETS) {
            //std::cout<<"do bufora"<<std::endl;
            packetBuffer.push_back(std::make_tuple(getChain(ip),(int)ip.protocol(),ip.src_addr().to_string(),ip.dst_addr().to_string()));
        }
    }
    return true;
    //zeby zakonczyc sniffowanie
    //return false
}

char Switch::getChain(const Tins::IP &ip) {
    if(ip.src_addr().to_string() == SWITCH_IP) {
        return 'O';
    }
    else if(ip.dst_addr().to_string() == SWITCH_IP) {
        return 'I';
    }
    else {
        return 'F';
    }
}

bool Switch::analyzePacket(const Tins::IP &ip) {
    for(auto i : packetMap) {
        if((i.ip.src_addr() == ip.src_addr()) && (i.ip.dst_addr() == ip.dst_addr()) && (i.ip.protocol() == ip.protocol())) {
            //std::cout<<"jest juz"<<std::endl;
            return true;
        }
    }
    //std::cout<<"false wiec dodajemy do mapy ze waiting"<<std::endl;
    PacketDecision temp;
    temp.ip = ip;
    temp.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    packetMap.push_back(temp);
    return false;
}

void Switch::bufferHandling() {
    for(;;) { //TODO delete busy waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if(packetBuffer.size() != 0 ) {
        auto tempPacket = packetBuffer.front();
        packetBuffer.pop_front();
        try {
            Protocol toCheck(CHECK);
            toCheck.id = client->name;
            toCheck.chain = std::get<0>(tempPacket);
            toCheck.packet_prot = std::get<1>(tempPacket);
            toCheck.packet_src = std::get<2>(tempPacket);
            toCheck.packet_dst = std::get<3>(tempPacket);
            client->send(toCheck);
        }
        catch(std::exception &e) {
            std::cout<<"Handling: "<<e.what()<<std::endl;
        }
    }
    }
}

void Switch::fillinPacketmap() {
    std::vector<std::string> rules = PythonAdapter::getInstance()->getRules();
    for(auto i : rules) {
        //wyciagam src i dst i prot i tworze IP
        std::vector<std::string> splitted;
        boost::split(splitted, i, [](char c){return c == '/';});
        Tins::IP packet(splitted[5],splitted[3]);
        packet.protocol(convertProtocol(splitted[2]));
        PacketDecision temp;
        temp.ip = packet;
        temp.timestamp =  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());;
        packetMap.push_back(temp);
    }
    //std::cout<<"D: "<<packetMap.size()<<std::endl;
    //for(auto a : packetMap) {
        //std::cout<<"D: "<<a.ip.src_addr()<<" "<<a.ip.dst_addr()<<" "<<std::to_string(a.ip.protocol())<<std::endl;
    //}
}

int Switch::convertProtocol(std::string prot) {
    if(prot.compare("tcp") == 0)
        return 6;
    else if(prot.compare("ip") == 0)
        return 4;
    else if(prot.compare("udp") == 0)
        return 17;
    else
        return 255;
}
