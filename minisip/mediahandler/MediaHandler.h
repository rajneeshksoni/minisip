/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* Copyright (C) 2004 
 *
 * Authors: Erik Eliasson <eliasson@it.kth.se>
 *          Johan Bilien <jobi@via.ecp.fr>
*/

#ifndef MEDIA_HANDLER_H
#define MEDIA_HANDLER_H

#include<libmutil/MemObject.h>

#include"Media.h"
#include"AudioMedia.h"
#include"MediaStream.h"
#include"Session.h"
#include"RtpReceiver.h"
#include"../minisip/ipprovider/IpProvider.h"
#include<libmutil/CommandString.h>


class SipSoftPhoneConfiguration;
class IpProvider;


class MediaHandler : public MObject, public SessionRegistry {

	public:
		/**
		 * Constructor, created on startup
		 * @param config reference to the softphone configuration
		 * @param ipProvider reference to the public IP provider, used
		 * for NAT traversal mechanisms
		 */
		MediaHandler( MRef<SipSoftPhoneConfiguration *> config, MRef<IpProvider *> ipProvider );
// 		~MediaHandler();
		
		/**
		 * Creates a new media session, for use in a new VoIP call
		 * @param config the call specific configuration
		 * @param callId identifier shared with the SIP stack
		 * @returns a reference to the session created
		 */
		MRef<Session *> createSession( SipDialogSecurityConfig &config, string callId = "" );
		
		/**
		 * Registers a new media type (audio or video
		 * @param media a reference to the representation of the
		 * medium to add
		 */
		void registerMedia( MRef<Media *> media );

		/**
		 * Handles a command sent by the user interface
		 * @param command the command to handle
		 */
		void handleCommand( CommandString command );
		
		/**
		 * Provides the IP address given as contact to the external
		 * peers
		 * @returns a string containing the IP address
		 */
		std::string getExtIP();
		
		
		virtual std::string getMemObjectType(){return "MediaHandler";}

#ifdef DEBUG_OUTPUT	
		virtual string getDebugString();
#endif

		/**
		True if all but one sender/media sessions are muted.
		If turned off, all ongoing sessions receive the audio from our mic.
		If turned on, only one source (the active one) will receive it.
		*/
		bool muteAllButOne;

	private:
		void init();

		std::list< MRef<Media *> > media;

		string ringtoneFile;

		MRef<AudioMedia *> audioMedia;
		MRef<IpProvider *> ipProvider;
		MRef<SipSoftPhoneConfiguration *> config;
		
		//cesc
		void setActiveSource( std::string callid );

};

#endif
