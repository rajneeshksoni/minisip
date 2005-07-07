/*
  Copyright (C) 2005, 2004 Erik Eliasson, Johan Bilien
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


/* Copyright (C) 2004 
 *
 * Authors: Erik Eliasson <eliasson@it.kth.se>
 *          Johan Bilien <jobi@via.ecp.fr>
*/

#ifndef _CONFERENCECONTROL_H
#define _CONFERENCECONTROL_H

#ifdef _MSC_VER
#ifdef LIBMINISIP_EXPORTS
#define LIBMINISIP_API __declspec(dllexport)
#else
#define LIBMINISIP_API __declspec(dllimport)
#endif
#else
#define LIBMINISIP_API
#endif


#include<string>
#include<libmutil/CommandString.h>
#include<libmutil/minilist.h>
#include<libminisip/ConfCallback.h>
#include<libminisip/ConfMember.h>


using namespace std;
/**
 * A text user interface. 
 * The interface interacts with the user via the terminal, and with the <code>Sip</code> class
 * with <code>SipSMCommands</code> via the <code>MessageRouter</code>.
 */
typedef struct CCList {
    string uris[10];
    string callids[10];
    int numUser;
} cclist;

class LIBMINISIP_API ConferenceControl{
    public:
        ConferenceControl();
	ConferenceControl(string configUri, string cid, bool islocal);
	void handleGuiCommand(string cmd);
	void handleGuiCommand(CommandString &command);
	void handleGuiDoInviteCommand(string sip_url);
	
	void handleSipCommand(CommandString &cmd);
	void setCallback(ConfCallback *cb);
	void setPendingList(string user);
	void setConnectedList(string user);
	ConfCallback* getCallback();
	
	
	/**
	* Moves a member from pending to connected and look for new members
	*/
	void handleOkAck(string callid, minilist<ConfMember> *list);
	
	/**
	* Print a list of conference members
	*/
	void printList(minilist<ConfMember> *list);
        minilist<ConfMember> * getConnectedList();
        
	string confId;
    private:
	void sendUpdatesToGui();
	/**
	* Move a member from pending to connected status
	*/
	void pendingToConnected(string memberid);
	/**
	* Check for new members to connect to
	*/
	void updateLists(minilist<ConfMember> *list);
	void removeMember(string memberid); 
	string addDomainToPrefix(string remoteUri);
	bool incoming;
        string input;
        string callId;
        string state;
	ConfCallback *callback;
	int numConnected;
	int numPending;
	
	minilist<ConfMember> connectedList;
	minilist<ConfMember> pendingList;
	string myUri;
	string myDomain;
	
};

#endif