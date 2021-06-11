Assignment: CMain.cpp CReadSHM.cpp CSHMStatistics.cpp CWriteSHM.cpp CParsePackets.cpp CDisplayStatisticsFileDump.cpp CReadPacketDump.cpp CQueue.cpp CGetPackets.cpp
	g++ $(DEBUG) -o Assignment CMain.cpp CReadSHM.cpp CSHMStatistics.cpp CWriteSHM.cpp CParsePackets.cpp CDisplayStatisticsFileDump.cpp CReadPacketDump.cpp CQueue.cpp CGetPackets.cpp -I ./ -L /usr/lib -lpcap
debug: DEBUG = -DDEBUG

debug: Assignment
