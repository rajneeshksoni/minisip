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

/*
 * Authors: Erik Eliasson <eliasson@it.kth.se>
 *          Johan Bilien <jobi@via.ecp.fr>
*/


#ifndef CERT_H
#define CERT_H


//#ifndef NO_SECURITY

extern "C"{
	#include<openssl/rsa.h>
	#include<openssl/evp.h>
	#include<openssl/objects.h>
	#include<openssl/x509.h>
	#include<openssl/err.h>
	#include<openssl/pem.h>
	#include<openssl/ssl.h>
}

#include<string>
#include<list>
#include<libmutil/Mutex.h>
#include<libmutil/MemObject.h>

#ifdef _MSC_VER
#ifdef LIBMUTIL_EXPORTS
#define LIBMUTIL_API __declspec(dllexport)
#else
#define LIBMUTIL_API __declspec(dllimport)
#endif
#else
#define LIBMUTIL_API
#endif

class certificate;


#define CERT_DB_ITEM_TYPE_OTHER  0
#define CERT_DB_ITEM_TYPE_FILE   1
#define CERT_DB_ITEM_TYPE_DIR    2

class LIBMUTIL_API ca_db_item{
	public:
		std::string item;
		int type;

		bool operator ==(const ca_db_item item2){ return (
				item2.item == item && 
				item2.type == type);};
};


class LIBMUTIL_API ca_db: public MObject{
	public:
		ca_db();
		~ca_db();
		
		X509_STORE * get_db();
		virtual std::string getMemObjectType(){return "ca_db";}
		void add_directory( std::string dir );
		void add_file( std::string file );
		void add_certificate( certificate * cert );
		std::list<ca_db_item *> &get_items();
		ca_db_item * get_next();
		void init_index();
		void lock();
		void unlock();

		void remove( ca_db_item * removedItem );

	private:
		X509_STORE * cert_db;

		std::list<ca_db_item *>::iterator items_index;

		std::list<ca_db_item *> items;

//		pthread_mutex_t mLock;
                Mutex mLock;
		
		
};

class LIBMUTIL_API certificate: public MObject{
	public:
		certificate();
		certificate( X509 * openssl_cert );
		certificate( const std::string cert_filename );
		certificate( const std::string cert_filename,
			     const std::string private_key_filename );
		certificate( unsigned char * der_cert, int length );
		~certificate();
		virtual std::string getMemObjectType(){return "certificate";}
		
		int control( ca_db * cert_db );

		int get_der_length();
		void get_der( unsigned char * output );
		int sign_data( unsigned char * data, int data_length, 
			       unsigned char * sign, int * sign_length );
		int verif_sign( unsigned char * sign, int sign_length,
				unsigned char * data, int data_length );

		std::string get_name();
		std::string get_cn();
		std::string get_issuer();
		std::string get_issuer_cn();

		std::string get_file();
		std::string get_pk_file();

		void set_pk( std::string file );

		EVP_PKEY * get_openssl_private_key(){return private_key;};
		X509 * get_openssl_certificate(){return cert;};
	private:
		EVP_PKEY * private_key;
		X509 * cert;

		std::string file;
		std::string pk_file;
};

class LIBMUTIL_API certificate_chain: public MObject{
	public:
		certificate_chain();
		certificate_chain( MRef<certificate *> cert );
		~certificate_chain();
		
		virtual std::string getMemObjectType(){return "certificate_chain";}
		
		void add_certificate( MRef<certificate *> cert );
		void remove_certificate( MRef<certificate *> cert );
		void remove_last();

		int control( MRef<ca_db *> cert_db );
		MRef<certificate *> get_next();
		MRef<certificate *> get_first();

		void clear();

		int length(){ return (int)cert_list.size(); };
		void lock();
		void unlock();

		bool is_empty();

		void init_index();
	private:
		std::list< MRef<certificate *> > cert_list;
		std::list< MRef<certificate *> >::iterator item;
//		pthread_mutex_t mLock;
                Mutex mLock;
};

class LIBMUTIL_API certificate_exception{
	public:
		certificate_exception(){};
		certificate_exception( std::string message ):message(message){};

		std::string get_message(){ return message; };
	protected:
		std::string message;
};

class LIBMUTIL_API certificate_exception_file : public certificate_exception{
	public:
		certificate_exception_file( std::string message ):certificate_exception(message){};
};

class LIBMUTIL_API certificate_exception_init : public certificate_exception{
	public:
		certificate_exception_init( std::string message ):certificate_exception(message){};
};

class LIBMUTIL_API certificate_exception_pkey : public certificate_exception{
	public:
		certificate_exception_pkey( std::string message ):certificate_exception(message){};
};

class LIBMUTIL_API certificate_exception_chain : public certificate_exception{
	public:
		certificate_exception_chain( std::string message ):certificate_exception(message){};
};
//#endif //NO_SECURITY

#endif
