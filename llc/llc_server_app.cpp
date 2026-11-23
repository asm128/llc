#include "llc_server_app.h"
#include "llc_parse.h"
#include "llc_gui_control_list.h"
#include "llc_json_expression.h"
#include "llc_chrono.h"

LLC_USING_TYPEINT();

llc::err_t			llc::loadServerConfig	(cnst SJSONReader & jsonConfig, s2_t parentNode, vcst_t & port, vcst_t & adapter) {
	if_fail_w(jsonExpressionResolve("listen_port", jsonConfig, parentNode, port));
	if_fail_w(jsonExpressionResolve("adapter", jsonConfig, parentNode, adapter));
	info_printf("\nListen port: %.*s.\nAdapter: %.*s.'", LLC_VIEW_ARGS(port), LLC_VIEW_ARGS(adapter));
	return 0;
}

llc::err_t			llc::setupGUI		(SServerUI & serverUI, SGUI & gui) {
	if_fail_fe(serverUI.Root = createScreenLayout(gui));

	stxp	n2u1_t		CONTROL_SIZE		= {192, 20};
	if_fail_fe(llc::setupControlListWithRoot<UI_SERVER_INPUT>(gui, serverUI.Input, serverUI.Root, CONTROL_SIZE, ALIGN_BOTTOM_RIGHT));
	if_fail_fe(llc::setupControlListWithRoot<UI_SERVER_INFO> (gui, serverUI.Info , serverUI.Root, CONTROL_SIZE, ALIGN_TOP_LEFT));
	return 0;
}

llc::err_t			llc::serverStart	(SServer & server, SGUI & gui) {
	if_fail_fe(llc::tcpipAddress({}, server.Port, server.SocketServer.Address));
	if_fail_fe(llc::parseIntegerDecimal(server.Adapter, server.SocketServer.AdapterIndex));
	if_fail_fe(llc::serverStart(server.SocketServer, server.SocketServer.Address.Port, server.SocketServer.AdapterIndex)); 
	server.ListeningSince	= timeCurrentInMs() / 1000.0;
	time_t						timeInt				= time(NULL);
	tm							timeptr				= {};
#if defined(LLC_WINDOWS)
	localtime_s(&timeptr, &timeInt);
#else
	localtime_r(&timeInt, &timeptr);
#endif
	sc_t						buf [64]			= {};
	strftime(buf, 32, "%Y-%m-%d %H:%M:%S", &timeptr);	
	server.UI.TextTimeStarted	= vcst_t(buf);
	return controlTextSet(gui, server.UI.Info[UI_SERVER_INFO_TimeStarted], server.UI.TextTimeStarted);
}


sttc	::llc::err_t	updateGUI		(::llc::SServer & server, ::llc::SGUI & gui)		{
	::llc::SSocketServer		& udpServer		= server.SocketServer;
	if_fail_fe(llc::controlTextSet(gui, server.UI.Input[::llc::UI_SERVER_INPUT_Stop		], udpServer.Listen ? ::llc::vcst_t{"Stop"} : ::llc::vcst_t{"Start"})); 
	if_fail_fe(llc::controlTextSet(gui, server.UI.Info [::llc::UI_SERVER_INFO_Listening	], udpServer.Listen ? ::llc::vcst_t{"Listening"} : ::llc::vcst_t{"Stopped"})); 
	sc_t						buf [128]		= {};
	{	
		u2_t					clientCount		= 0;
		for(u2_t iClient = 0; iClient < udpServer.Clients.size(); ++iClient) 
			if(udpServer.Clients[iClient]->State == ::llc::CONNECTION_STATE_IDLE)
				++clientCount;

		::llc::sprintf_s(buf, "Clients: %i", clientCount);	
		server.UI.TextClientCount	= ::llc::vcst_t(buf);
		if_fail_fe(llc::controlTextSet(gui, server.UI.Info[::llc::UI_SERVER_INFO_ClientCount], server.UI.TextClientCount));
	}
	{ // update datetime field
		time_t						timeInt			= time(0);
		tm							timeptr			= {};
	#if defined(LLC_WINDOWS)
		localtime_s(&timeptr, &timeInt);
	#else
		localtime_r(&timeInt, &timeptr);
	#endif
		strftime(buf, 32, "%Y-%m-%d %H:%M:%S", &timeptr);	
		server.UI.TextDateTime	= ::llc::vcst_t(buf);
		if_fail_fe(llc::controlTextSet(gui, server.UI.Info[::llc::UI_SERVER_INFO_DateTime], server.UI.TextDateTime)); 
	}

	return 0;
}

llc::err_t			llc::serverUpdate		(SServer & server, SGUI & gui) {
	server.QueueReceived.clear();
	if_fail_e(serverPayloadCollect(server.SocketServer, server.QueueReceived));
	
	server.SocketServer.Clients.enumerate([&server](u2_t & iClient, pobj<SConnection> & client){
		if(server.QueueToSend.size() <= iClient)
			iClient					= server.SocketServer.Clients.size();
		else {
			if_fail_e(server.QueueToSend[iClient]->for_each([&client](pau0_t & payload) {
				if(payload && payload->size())
					if_fail_fe(llc::connectionPushData(*client, client->Queue, *payload));

				payload.clear();
 				return 0;
			}));
			server.QueueToSend[iClient].clear();
		}
		return 0;
	});
	return ::updateGUI(server, gui);
}

llc::err_t			llc::processGUIEvent	(SServer & server, SGUI & gui, cnst SEventView<EVENT_GUI_CONTROL> & screenEvent) { 
	SSocketServer			& socketServer				= server.SocketServer;
	switch(screenEvent.Type) {
	default: break;
	case EVENT_GUI_CONTROL_StateChange: {
		cnst SChangeControlState	stateChange		= *(cnst SChangeControlState*)screenEvent.Data.begin();
		if(0 == (stateChange.Set & GUI_CONTROL_FLAG_Action))
			return 0;

		for(u2_t iInput = 0; iInput < server.UI.Input.size(); ++iInput) {
			if(server.UI.Input[iInput] != stateChange.Id) 
				continue;

			switch(iInput) {
			default: break;
			case UI_SERVER_INPUT_Exit		: return 1;
			case UI_SERVER_INPUT_Disconnect	: return 0;
			case UI_SERVER_INPUT_Restart		: 
				if(socketServer.Listen) {
					if_fail_e(serverStop(socketServer)); 
				}
				if_fail_e(llc::serverStart(server, gui));
				return 0;
			case UI_SERVER_INPUT_Stop			: 
				if(socketServer.Listen) {
					if_fail_e(serverStop(socketServer)); 
				}
				else {
					if_fail_e(serverStart(server, gui));
				}

				return 0;
			}
		}
	}
	}
	return 0;
}
