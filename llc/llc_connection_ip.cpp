//#include "llc_connection_ip.h"
//
//LLC_USING_TYPEINT();
//
//llc::err_t		llc::connectionHandleCommandUDP	(SConnection & client, SCommand & command, au0_t & receiveBuffer)		{
//	au0_t				receiveBuffer;
//	return ::connectionHandleCommandCore(client, command, receive, receiveBuffer);
//}
//
//
//llc::err_t			llc::connectionHandleCommandTCP	(SConnection & client, SCommand & command, au0_t & receiveBuffer)		{
//	au0_t						receiveBuffer;
//	return ::connectionHandleCommand(client, command, receive, receiveBuffer);
//}
//sttc	llc::err_t	sendTo							(::llc::vcu0_t frame) {
//	sockaddr_in			remote							= {};
//	if_fail_fe(::llc::tcpipAddressToSockaddr(client.Address, remote));
//	return ::sendto(client.Socket, (const char*)frame.begin(), (int)frame.size(), 0, (sockaddr*)&remote, sizeof(remote)) == (int)frame.size() ? 0 : -1;
//};
//llc::err_t			llc::connectionSendQueueUDP		(SConnection & client, apobj<SMessage> & messageCacheSent, apobj<SMessage> & messageCacheSend, bool packPayloads)				{
//	return connectionSendQueue(client, messageCacheSent, messageCacheSend, sendTo, packPayloads);
//}
