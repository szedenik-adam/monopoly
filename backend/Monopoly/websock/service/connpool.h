/**
 *
 * filename: demo.h
 * summary:
 * author: caosiyang
 * email: csy3228@gmail.com
 *
 */
#ifndef CONNPOOL_H
#define CONNPOOL_H

#include "../websocket.h"
#include "../connection.h"
#include "ws_user.h"
#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <assert.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
using namespace std;

extern vector<WSAuth*> sessions;

evconnlistener* InitWebsocket(event_base* base, unsigned short port);

#endif
