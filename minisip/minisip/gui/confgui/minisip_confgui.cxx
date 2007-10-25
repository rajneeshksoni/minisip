#include<libminisip/Minisip.h>
#include<libminisip/gui/Gui.h>
#include<libmsip/SipCommandString.h>
#include<libmutil/TextUI.h>
#include<iostream>

#include<libminisip/media/MediaCommandString.h>
#include<libminisip/signaling/conference/ConferenceControl.h>
#include<libminisip/signaling/conference/ConfMessageRouter.h>



//cleanup, how to get username and domain separately
//set_session_sound_settings also in incoming_available?
//audio_forwarding_enable is done ok in invite_ok?
//should it be done also in incoming_available?



using namespace std;

class MyGui : public Gui{
		std::string 			   currentconfname;
		MRef<SipSoftPhoneConfiguration *>  config;
		ConferenceControl 		  *currentconf;
		MRef<ConfMessageRouter *> 	   confCallback;
		std::list<MRef<ContactEntry *> >   contactEntries;
	
	public:
		/***********************************
                the method that handles the commands
                ***********************************/
                void handleCommand(const CommandString &command){
			std::string callId;
			std::string description;
			list<MRef<ContactEntry *> >::iterator entryIter;

                        cerr << "****MyGui: handleCommand called: " << endl;
                        
			if(command.getOp() == "incoming_available"){
				//accepts a call only if it is in the list of contacts
				for( entryIter = contactEntries.begin(); entryIter != contactEntries.end(); entryIter++ ){
                                        if( SipUri((*entryIter)->getUri()) == SipUri(command.getParam()) ){                 
					        cerr << "MyGui: incoming call from: " << command.getParam() << endl;
                	                        CommandString resp(command.getDestinationId(), "accept_invite");
                                		sendCommand("sip", resp);

						callId = command.getDestinationId();
						CommandString cmdstr(callId,
		                                        MediaCommandString::audio_forwarding_enable/*,
                		                        "senders", "ON"*/);
                                		sendCommand("media", cmdstr);

                                                cerr << "***** INCOMING *** AVAILABLE ***** " << description << endl;
					}
                                }
                        }

			//registered suceessfully with ser
                        if(command.getOp() == "register_ok"){ 
			        inviteAllContacts();
                        }

			//the user accepted the call
			if(command.getOp() == "invite_ok"){
				callId = command.getDestinationId();
                		CommandString cmdstr1( callId,
                                	MediaCommandString::set_session_sound_settings,
                                	"senders", "ON");
                		sendCommand("media", cmdstr1);
				
				CommandString cmdstr2( callId,
                                        MediaCommandString::audio_forwarding_enable/*,
                                        "senders", "ON"*/);
                                sendCommand("media", cmdstr2);

				description = "ACCEPT_CALL " + callId;

				cerr << "EEEE: number of entries: " << contactEntries.size()<<endl; 
				for( entryIter = contactEntries.begin(); entryIter != contactEntries.end(); entryIter++ ){
					if( SipUri((*entryIter)->getUri()) == SipUri(command.getParam()) ){
						cerr << "***** ACCEPTED *** CALL ***** " << description << endl;
						(*entryIter)->setDesc(description);
					}
				}
			}
			//the user ended the call
			// id=asdflskfjlklserjldkj, op=remote_hang_up
                	if(command.getOp() == "remote_hang_up"){
				cerr << "*****HANGUP*****: " << endl;
	
				for( entryIter = contactEntries.begin(); entryIter != contactEntries.end(); entryIter++ ){
					description = (*entryIter)->getDesc().substr(12);
					cerr << "*****DESCRIPTION*****: " << description << endl;
					if(description == command.getDestinationId()){
						description = "HUNGUP_CALL " + command.getDestinationId();
						cerr << "***** HUNGUP *** CALL ***** " << description << endl;
						(*entryIter)->setDesc(description);
						break;
					}
				}
			}
		}

		/*************
		the run method
		**************/
		//we need to add code here for the program
		//to terminate at some point when
		//the connected users have disconnected
		void run(){
			int waiting = 1;
			list<MRef<ContactEntry *> >::iterator entryIter;
		
			//sleep(DURATION_TIME);
			while(waiting){
				sleep(10);

				waiting = 0;
				for( entryIter = contactEntries.begin(); entryIter != contactEntries.end(); entryIter++ ){
					//at least one user is still in a call
		       			if((*entryIter)->getDesc().substr(0,11) == "ACCEPT_CALL"){
						waiting = 1;
						break;
		              		}
		      		}
			}
			cleanup();
		}

		/*********************************
		goes through all the contacts
		and invites them to the conference
		***********************************/
		void inviteAllContacts(){
			string uri;
			std::list<MRef<PhoneBook *> > 		phonebooks;
			std::list<MRef<PhoneBookPerson *> > 	persons;
			std::list<MRef<ContactEntry *> > 	entries;

			list< MRef< PhoneBook * > >::iterator 	    phonebookIter;
			list< MRef< PhoneBookPerson * > >::iterator personIter;
			list< MRef< ContactEntry * > >::iterator    entryIter;

			phonebooks = config->phonebooks;
			for( phonebookIter = phonebooks.begin(); phonebookIter != phonebooks.end(); phonebookIter++ ){
                		persons = (*phonebookIter)->getPersons();
				for( personIter = persons.begin(); personIter != persons.end(); personIter++ ){
			                entries = (*personIter)->getEntries();
					//contactEntries = entries;
					for( entryIter = entries.begin(); entryIter != entries.end(); entryIter++ ){
						contactEntries.push_back(*entryIter);
						
						uri = (*entryIter)->getUri();
						(*entryIter)->setDesc("THIS STRING NEEDS TO BE LONGER THAN 12 LETTERS");
						callUser(uri);
						//cerr << "OTHER CONTACT: " << uri << endl;
					}	
                		}
                	}
		}
		
		/**********************************
		for making a regular call to a user
		***********************************/
		void callUser(string user){
                        CommandString invite("", SipCommandString::invite, user);
                        CommandString resp = callback->handleCommandResp("sip", invite);
		}

                /*******************************
                this is for disconnecting a user
                ********************************/
                void hangupUser(){
                        CommandString hup("", SipCommandString::hang_up);
                        hup.setParam3(currentconfname);
                        confCallback->guicb_handleConfCommand(hup);
                }

		/**********************
		remove the ser user
		remove the config file
		remove the contact file
		***********************/
		void cleanup(){
			SipUri 	    myIdentity;
			std::string rmSerUser_cmd;
                        std::string rmConfigFile_cmd;
                        std::string rmContactFile_cmd;

			myIdentity = 	    config->defaultIdentity->getSipUri();
			rmSerUser_cmd =     "./script rm " + myIdentity.getUserIpString() + " " + myIdentity.getIp(); // domain_name
			rmConfigFile_cmd =  "rm ";
			rmContactFile_cmd = "rm ";

		/*	system(rmSerUser_cmd.c_str());
			system(rmConfigFile_cmd.c_str());
			system(rmContactFile_cmd.c_str());
		*/
		}

		/*****************
		initializes config
		******************/
		void setSipSoftPhoneConfiguration(MRef<SipSoftPhoneConfiguration *> sipphoneconfig){
			config = sipphoneconfig;
			cerr << "MyGui: setSipSoftPhoneConfiguration called"<<endl;
		}
		
		/******************************************************
		method needed because defined as virtual in super class	
		*******************************************************/
		void setContactDb(MRef<ContactDb *> contactDb){
			cerr << "MyGui: setContactDb called"<<endl;
		}

		/******************************************************
                method needed because defined as virtual in super class
                *******************************************************/
		bool configDialog(MRef<SipSoftPhoneConfiguration *> conf){
			cerr << "MyGui: configDialog called"<<endl;return false;
		}

	private: 
};


//********************************
//********************************
int main( int argc, char *argv[] )
{
        merr.setPrintStreamName(true);
        mout.setPrintStreamName(true);
        mdbg.setPrintStreamName(true);

#if defined(DEBUG_OUTPUT) || !defined(WIN32)
        cerr << endl << "Starting MiniSIP GTK ... welcome!" << endl << endl;
#endif

        setupDefaultSignalHandling(); //Signal handlers are created for all
                                      //threads created with libmutil/Thread.h
                                      //For the main thread we have to
                                      //install them
////#ifndef DEBUG_OUTPUT
////        redirectOutput( UserConfig::getFileName( "minisip.log" ).c_str() );
////#endif

        cerr << "Creating GTK GUI"<< endl;

	MRef<MyGui *> gui = new MyGui();


        Minisip minisip( *gui, argc, argv );


        if( minisip.startSip() > 0 ) {
#ifdef DEBUG_OUTPUT
                minisip.startDebugger();
#else
                //in non-debug mode, send merr to the gui
                merr.setExternalHandler( dynamic_cast<DbgHandler *>( *gui ) );
                mout.setExternalHandler( dynamic_cast<DbgHandler *>( *gui ) );
                mdbg.setExternalHandler( dynamic_cast<DbgHandler *>( *gui ) );
#endif  // DEBUG_OUTPUT

                minisip.runGui();

#ifndef DEBUG_OUTPUT
                merr.setExternalHandler( NULL );
                mout.setExternalHandler( NULL );
                mdbg.setExternalHandler( NULL );
#endif
        } else {
                cerr << endl << "ERROR while starting SIP!" << endl << endl;
        }

	gui->cleanup();
        minisip.exit();
}
                                