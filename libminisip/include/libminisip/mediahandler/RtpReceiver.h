/*
 Copyright (C) 2004-2006 the Minisip Team
 
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
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

/* Copyright (C) 2004, 2005 
 *
 * Authors: Erik Eliasson <eliasson@it.kth.se>
 *          Johan Bilien <jobi@via.ecp.fr>
*/

#ifndef RTPRECEIVER_H
#define RTPRECEIVER_H

#include<libminisip/libminisip_config.h>

#include<libmutil/Mutex.h>
#include<libmutil/MemObject.h>
#include<libmutil/Thread.h>

#include<libminisip/ipprovider/IpProvider.h>

class UDPSocket;
class MediaStreamReceiver;
class CryptoContext;

/**
 * The RtpReceiver is used to listen on a UDPSocket and demultiplex
 * several incoming streams, depending on their payload type.
 * MediaStreamReceiver objects register to it when they are ready
 * to receive a specific media type.
 */
class RtpReceiver : public Runnable{
	public:
		/**
		 * Constructor, called by the MediaHandler upon
		 * creation of a media Session.
		 * @param ipprovider reference to an IpProvider object,
		 * used to query the external IP address and UDP port
		 * on which the peer should send data (used by
		 * NAT traversal mechanisms).
		 */
		RtpReceiver( MRef<IpProvider *> ipProvider );

		/**
		 * Used for a MediaStreamReceiver to subscribe to data
		 * incoming on this RtpReceiver. If the payload type
		 * is handled by the MediaStreamReceiver, the data
		 * is sent to it.
		 * @param mediaStream a reference to the MediaStreamReceiver
		 * object to subscribe
		 */
		void registerMediaStream( MRef<MediaStreamReceiver *> mediaStream );

		/**
		 * Used to signify that a MediaStreamReceiver should no
		 * longer receive data from this RtpReceiver. Used
		 * when the media Session is stopped.
		 * @param mediaStream a reference to the MediaStreamReceiver
		 * object that should be unsubscribed
		 */
		void unregisterMediaStream( MRef<MediaStreamReceiver *> mediaStream);

		/**
		 * Listening thread main loop.
		 */
		virtual void run();

		/**
		 * Used to query the port that should be given as contact
		 * information to the peer in the session description (SDP).
		 * It may be different from the local port of the UDPSocket,
		 * in case of NAT traversal mechanisms.
		 * @returns the port number to use as contact information
		 */
		uint16_t getPort();

		/**
		 * Destructor. Will wait for the running thread
		 * to be terminated.
		 */
		~RtpReceiver();

		/**
		 * Used to query the UDPSocket on which the RtpReceiver is
		 * listening.
		 * @returns a reference to the UDPSocket object
		 */
		MRef<UDPSocket *> getSocket();

		virtual std::string getMemObjectType(){return "RtpReceiver";}

	private:
		MRef<UDPSocket *> socket;
		uint16_t externalPort;
		bool kill;

		std::list< MRef<MediaStreamReceiver *> > mediaStreams;

		Mutex mediaStreamsLock;

		Thread * thread;
};

#endif
