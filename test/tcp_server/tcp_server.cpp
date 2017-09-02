// tcp_server.cpp

#include <memory>
#include <iostream>
#include <portable/Packet.h>
#include <portable/StdFile.h>
#include <portable/BsdPacketServer.h>
#include <xxHash/xxhash.h>

using namespace std;

int main()
{   const int networkPort = 8000;
    cout << "Starting server on port " << networkPort << endl;
    std::unique_ptr<portable::BsdPacketServer> packetServer = make_unique<portable::BsdPacketServer>("1.0");
    const bool ok = packetServer->Start(networkPort, max_streams);


portable::PacketWriter& packet(packetServer->headerPacket);
portable::SoftLock softlock(packet.ownership);
if (!softlock)
{
    cout << "ERROR: Couldn't lock header packet" << endl;
    return;
}
const unsigned packetId = 0;
packet << packetId;

portable::PacketWriter& packet = packetServer->framePacket.GetDirty();
