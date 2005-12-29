/*
  Copyright (C) 2005 Mikael Magnusson, Erik Eliasson
  
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

/*
 * Author(s): Mikael Magnusson <mikma@users.sourceforge.net>
 *            Erik Eliasson <eliasson@it.kth.se>
*/


/* Name
 * 	SipRequest.cxx
 * Author
 *      Mikael Magnusson, mikma@users.sourceforge.net
 *      Erik Eliasson, eliasson@it.kth.se 
 * Purpose
 *      Any SIP request request to libmsip
*/

#include<config.h>

#include<libmsip/SipRequest.h>
#include<libmsip/SipException.h>
#include<libmsip/SipMessageContentIM.h>
#include<libmsip/SipHeader.h>
#include<libmsip/SipHeaderRoute.h>
#include<libmsip/SipHeaderEvent.h>
#include<libmsip/SipHeaderFrom.h>
#include<libmsip/SipHeaderTo.h>
#include<libmsip/SipHeaderCallID.h>
#include<libmsip/SipHeaderCSeq.h>
#include<libmsip/SipHeaderMaxForwards.h>
#include<libmsip/SipHeaderUserAgent.h>
#include<libmsip/SipHeaderProxyAuthorization.h>
#include<libmsip/SipHeaderAccept.h>
#include<libmsip/SipHeaderContact.h>
#include<libmsip/SipHeaderReferTo.h>

MRef<SipRequest*> SipRequest::createSipMessageAck(string branch,
		MRef<SipMessage*> pack,
		string to_tel_no)
{
	MRef<SipRequest*> req = new SipRequest(branch, "ACK");
	req->setUri(to_tel_no);

	req->addHeader(new SipHeader(new SipHeaderValueMaxForwards(70)));
	
	int noHeaders = pack->getNoHeaders();
	for (int32_t i=0; i< noHeaders; i++){			//FIX: deep copy?
		MRef<SipHeader *> header = pack->getHeaderNo(i);
		int headerType = header->getType();
		switch (headerType){
			case SIP_HEADER_TYPE_CSEQ:
				((SipHeaderValueCSeq*) *(header->getHeaderValue(0)))->setMethod("ACK");
			case SIP_HEADER_TYPE_FROM:
			case SIP_HEADER_TYPE_TO:
			case SIP_HEADER_TYPE_CALLID:
				req->addHeader(header);
				break;
		}
	}
	
	return req;
}


MRef<SipRequest*> SipRequest::createSipMessageBye(string branch,
                MRef<SipRequest*> inv,
                string to_uri,
                string from_uri,
                string proxyAddr,
                int32_t seq_no)
{
	MRef<SipRequest*> req = new SipRequest(branch, "BYE");
	
	req->setUri(to_uri);
	
        req->addHeader(new SipHeader(new SipHeaderValueMaxForwards(70)));

	int noHeaders = inv->getNoHeaders();
	for (int32_t i=0; i < noHeaders; i++){
		MRef<SipHeader *> header = inv->getHeaderNo(i);
		int headerType = header->getType();
		bool add = false;
		switch (headerType){
			case SIP_HEADER_TYPE_FROM:
				((SipHeaderValueFrom*)*(header->getHeaderValue(0)))->removeParameter("tag");
				add = true;
				break;
			case SIP_HEADER_TYPE_TO:
				((SipHeaderValueTo*)*(header->getHeaderValue(0)))->removeParameter("tag");
				add = true;
				break;
	
			case SIP_HEADER_TYPE_CSEQ:
				((SipHeaderValueCSeq*)*(header->getHeaderValue(0)))->setMethod("BYE");
				((SipHeaderValueCSeq*)*(header->getHeaderValue(0)))->setCSeq(seq_no);
				add=true;
				break;
			case SIP_HEADER_TYPE_CALLID:
				add=true;
				break;
			
		}
		if (add){
			req->addHeader(header);
		}
	}	

	return req;

}

MRef<SipRequest*> SipRequest::createSipMessageCancel(string branch,
                MRef<SipRequest*> inv,
                string to_uri,
                string from_uri,
                string proxy
                )
{
	MRef<SipRequest*> req = new SipRequest(branch, "CANCEL");
	req->setUri(to_uri);

	req->addHeader(new SipHeader( new SipHeaderValueMaxForwards(70)));
	
	MRef<SipHeader *> header;
	int noHeaders = inv->getNoHeaders();
	for (int32_t i=0; i< noHeaders; i++){
		header = inv->getHeaderNo(i);
		int type = header->getType();
		bool add=false;
		switch (type){
			case SIP_HEADER_TYPE_FROM:
				add=true;
				break;
			case SIP_HEADER_TYPE_TO:
				add=true;
				break;
			case SIP_HEADER_TYPE_CSEQ:
				((SipHeaderValueCSeq*)*(header->getHeaderValue(0)))->setCSeq(  ((SipHeaderValueCSeq *)*(header->getHeaderValue(0)))->getCSeq() );
				((SipHeaderValueCSeq*)*(header->getHeaderValue(0)))->setMethod("CANCEL");
				add=true;
				break;
			case SIP_HEADER_TYPE_CALLID:
				add=true;
				break;
		}
		if (add){
			req->addHeader(header);
		}
	}
	
	
	return req;
}


MRef<SipRequest*> SipRequest::createSipMessageIMMessage(string branch,
                                                string callId,
                                                std::string toUri,
                                                //MRef<SipIdentity*> fromIdentity,
						const SipURI& fromUri,
                                                int32_t seqNo,
                                                string msg)
{
	MRef<SipRequest*> req = new SipRequest(branch, "MESSAGE");
	req->setUri(toUri);
	req->addDefaultHeaders(fromUri,toUri,"MESSAGE",seqNo,callId);
	req->addHeader(new SipHeader(new SipHeaderValueUserAgent(HEADER_USER_AGENT_DEFAULT)));
	req->setContent(new SipMessageContentIM(msg));
	return req;
}

static void addHeaders( MRef<SipRequest*> req,
		const string &call_id,
		const string &tel_no,	//FIXME: Send uris as const SipURI&
		const string &proxyAddr,
		int32_t proxyPort,
		const string &localAddr,
		int32_t localSipPort,
		const string &from_tel_no,
		int32_t seq_no,
		const string &username,
		const string &nonce,
		const string &realm,
		const string &password,
		const string &transport
		)
{

	req->setUri(tel_no);

	MRef<SipHeader*> hdr;
	
	SipURI fromUri(from_tel_no);
	req->addHeader(new SipHeader( new SipHeaderValueFrom(fromUri) ) );

	SipURI toUri(tel_no);
	req->addHeader(new SipHeader( new SipHeaderValueTo(toUri) ));
	
	req->addHeader(new SipHeader(new SipHeaderValueCallID(call_id)) );
        

	SipURI uri;
	uri.setParams(tel_no,proxyAddr,"",proxyPort);
	
	if ( username.length()>0 || nonce.length()>0 || realm.length()>0 ){
		req->addHeader(new SipHeader( 
				new SipHeaderValueProxyAuthorization("INVITE",tel_no,realm, nonce, uri, username, password,"DIGEST") )
				);
	}

	req->addHeader(new SipHeader(new SipHeaderValueCSeq("INVITE",seq_no)));
	req->addHeader(new SipHeader(new SipHeaderValueContact(from_tel_no, localAddr, localSipPort,"",transport)));
	req->addHeader(new SipHeader(new SipHeaderValueUserAgent(HEADER_USER_AGENT_DEFAULT)));
}





MRef<SipRequest*> SipRequest::createSipMessageInvite(const string &branch,
                const string &call_id,
                const string &tel_no,
                const string &proxyAddr,
                int32_t proxyPort,
                const string &localAddr,
                int32_t localSipPort,
                const string &from_tel_no,
                int32_t seq_no,
                const string &transport
                )
{
	MRef<SipRequest*> req = new SipRequest(branch,"INVITE");
	addHeaders(req, call_id, tel_no, 
			proxyAddr, proxyPort, 
			localAddr, localSipPort, 
			from_tel_no, seq_no, 
			"","","","",transport);
	
	
	
	return req;
}

MRef<SipRequest*> SipRequest::createSipMessageInvite(const string &branch,
                const string &call_id,
                const string &tel_no,
                const string &proxyAddr,
                int32_t proxyPort,
                const string &localAddr,
                int32_t localSipPort,
                const string &from_tel_no,
                int32_t seq_no,
                const string &username,
                const string &nonce,
                const string &realm,
                const string &password,
                const string &transport)
{
	MRef<SipRequest*> req = new SipRequest(branch, "INVITE");
	
	addHeaders(req, call_id, tel_no, 
			proxyAddr, proxyPort, 
			localAddr, localSipPort, 
			from_tel_no, seq_no, 
			username, nonce, realm, password, transport);
	return req;
}





MRef<SipRequest*> SipRequest::createSipMessageNotify(string branch,
                string callId,
                //MRef<SipIdentity*> toIdentity,
		const SipURI& toUri,
                //MRef<SipIdentity*> fromId,
		const SipURI& fromUri,
                int32_t seqNo
                )
{
	MRef<SipRequest*> req = new SipRequest(branch, "NOTIFY");
	req->setUri(toUri.getString());
	req->addDefaultHeaders(fromUri, toUri,"NOTIFY",seqNo,callId);
	req->addHeader(new SipHeader(new SipHeaderValueUserAgent(HEADER_USER_AGENT_DEFAULT)));
	req->addHeader(new SipHeader(new SipHeaderValueEvent("presence")));
	return req;
}



MRef<SipRequest*> SipRequest::createSipMessageRefer(string branch,
		MRef<SipRequest*> inv,
		string to_uri,
		string from_uri,
		string proxy,
		string referredUri,
		int cSeqNo)
{
	MRef<SipRequest*> req = new SipRequest(branch, "REFER");
	req->setUri(to_uri);

	req->addHeader(new SipHeader( new SipHeaderValueMaxForwards(70)));
	
	MRef<SipHeader *> header;
	int noHeaders = inv->getNoHeaders();
	for (int32_t i=0; i< noHeaders; i++){
		header = inv->getHeaderNo(i);
		int type = header->getType();
		bool add=false;
		switch (type){
			case SIP_HEADER_TYPE_FROM:
				((SipHeaderValueFrom*)*(header->getHeaderValue(0)))->getUri().setUser(from_uri);//This line can be removed?
				add=true;
				break;
			case SIP_HEADER_TYPE_TO:
				add=true;
				break;
			case SIP_HEADER_TYPE_CALLID:
				add=true;
				break;
		}
		
		if (add){
			req->addHeader(header);
		}
	}

	/* Add the CSeq: header */
	req->addHeader(new SipHeader(new SipHeaderValueCSeq("REFER", cSeqNo)));
	
	/* Add the Refer-To: header */
	req->addHeader(new SipHeader(new SipHeaderValueReferTo(referredUri)));
	return req;
}


MRef<SipRequest*> SipRequest::createSipMessageRegister(string branch,
                string call_id,
                string domain,
                string localIp,
                int32_t sip_listen_port,
                string from_tel_no,	//FIXME: use SipURI
                int32_t seq_no,
                string transport,
		int expires,
                string auth_id,
                string realm,
                string nonce,
                string password)
{
	MRef<SipRequest*> req = new SipRequest(branch, "REGISTER","sip:"+domain);

	req->setUri("sip:" + domain);


	SipURI fromUri(from_tel_no);
	SipURI toUri(from_tel_no);
	req->addDefaultHeaders(fromUri,toUri,"REGISTER",seq_no,call_id);
	
	 
	req->addHeader(new SipHeader(new SipHeaderValueContact(from_tel_no, localIp, sip_listen_port,"",transport, expires)));
	req->addHeader(new SipHeader(new SipHeaderValueUserAgent(HEADER_USER_AGENT_DEFAULT)));
	

	SipURI uri;
	uri.setParams("", localIp,"", sip_listen_port);
	if (auth_id!=""){
		MRef<SipHeaderValue*> authp = 
			new SipHeaderValueAuthorization(
					"REGISTER",
					from_tel_no, 
					realm, 
					nonce, 
					uri, 
					auth_id, 
					password,
					"Digest");
		req->addHeader(new SipHeader(*authp));
	}
	
	req->setContent(NULL);

	return req;
}


MRef<SipRequest*> SipRequest::createSipMessageSubscribe(string branch,
                string call_id,
		const SipURI &toUri,
		const SipURI &fromUri,
                int32_t seq_no)
{
	MRef<SipRequest*> req = new SipRequest(branch, "SUBSCRIBE", toUri.getString() );

	req->setUri(toUri.getString());

	req->addDefaultHeaders(fromUri, toUri,"SUBSCRIBE",seq_no, call_id);
	
	req->addHeader(new SipHeader(new SipHeaderValueEvent("presence")));
	req->addHeader(new SipHeader(new SipHeaderValueAccept("application/xpidf+xml")));
						
	return req;
}

void SipRequest::addDefaultHeaders(const SipURI& fromUri,
		const SipURI& toUri,
		const string& method,
		int seqNo,
		const string& callId)
{
        addHeader(new SipHeader(new SipHeaderValueFrom(fromUri)));
        addHeader(new SipHeader(new SipHeaderValueTo(toUri)));
        addHeader(new SipHeader(new SipHeaderValueCallID(callId)));
        addHeader(new SipHeader(new SipHeaderValueCSeq(method, seqNo)));
	addHeader(new SipHeader(new SipHeaderValueMaxForwards(70)));
}


SipRequest::SipRequest(string branch, const string &method,
		       const string &uri) :
		SipMessage(branch), method(method),
		uri(uri)
{
	if( this->uri == "" )
		this->uri = "sip:";
}

SipRequest::SipRequest(string &build_from): SipMessage(-1, build_from){
	init(build_from);
}

void SipRequest::init(string &build_from){
	int start = 0;
	int pos;
	int pos2;
	int end = 0;
	int length = build_from.length();
	string requestLine;

	// Skip white space
	start = build_from.find_first_not_of( ' ', start );
	if( start == string::npos ){
		throw SipExceptionInvalidMessage("SipRequest malformed - first line did not contain any non whitespace character");
	}

	end = build_from.find_first_of( "\r\n", start );
	if( end == string::npos ){
		throw SipExceptionInvalidMessage("SipRequest malformed - only one line");
	}

	requestLine = build_from.substr( start, end - start );
	start = 0;
	end = requestLine.length();

	// Parse method
	pos = requestLine.find( ' ', start );
	if( pos == string::npos ){
		throw SipExceptionInvalidMessage("SipRequest malformed - could not find method");
	}

	method = build_from.substr( start, pos - start );


	// Parse version
	pos2 = requestLine.rfind( ' ', end - 1 );
	if( pos2 == string::npos ){
		throw SipExceptionInvalidMessage("SipRequest malformed - request line did not contain space between method and version");
	}

	string version = requestLine.substr( pos2 + 1, end - pos2 );

	if( version != "SIP/2.0" ){
		throw SipExceptionInvalidMessage("SipRequest malformed - unknown version");
	}	  

	uri = requestLine.substr( pos + 1, pos2 - pos );
}

SipRequest::~SipRequest(){
}

const string& SipRequest::getType(){
	return method;
}

string SipRequest::getString(){
	return getMethod() + " " + getUri() + " SIP/2.0\r\n"
		+ getHeadersAndContent();
}


void SipRequest::setMethod(const string &method){
	this->method = method;
}

string SipRequest::getMethod(){
	return method;
}

static string buildUri(const string &name)
{
	string ret ="";
	
	//FIXME sanitize the request uri ... if we used a SipURI object, this would not be needed
	string username; //hide the class::username ... carefull
	size_t pos;
	username = name;
	
	pos = username.find('<');
	if( pos != string::npos ) {
		username.erase( 0, pos + 1 ); //erase the part in front of the '<'
		pos = username.find('>');
		username.erase( pos );
	}

	if (username.length()>4 && username.substr(0,4)=="sip:")
		ret = username;
	else
		ret = "sip:"+username;

#if 0
	if (username.find("@")==string::npos)
		ret = ret+"@"+ip;
#endif

	return ret;
}

void SipRequest::setUri(const string &uri){
	this->uri = buildUri(uri);
}

string SipRequest::getUri(){
	return uri;
}

void SipRequest::addRoute(const string &route)
{
	MRef<SipHeaderValue*> routeValue = (SipHeaderValue*)new SipHeaderValueRoute( route );
	MRef<SipHeader*> routeHdr = new SipHeader( routeValue );
	int i;
	int pos = 0;
	
	for( i = 0; i < headers.size(); i++ ) {
		if( headers[i]->getType() == SIP_HEADER_TYPE_ROUTE ) {
			pos = i;
			break;
		}
	}

	headers.insert( pos, routeHdr );	
}

void SipRequest::addRoute(const string &addr, int32_t port,
			  const string &transport)
{
	string uri = "<sip:" + addr;

	if( port ){
		char buf[20];
		sprintf(buf, "%d", port);
		uri = uri + ":" + buf;
	}

	if( transport != "" ){
		uri = uri + ";transport=" + transport;
	}

	uri = uri + ";lr>";
	
	addRoute( uri );
}
