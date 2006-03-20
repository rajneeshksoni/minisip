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

/* Copyright (C) 2005
 *
 * Authors: Erik Eliasson <eliasson@it.kth.se>
 *          Johan Bilien <jobi@via.ecp.fr>
*/


#include<config.h>

#include"ConfBackend.h"

class XMLFileParser;

class MXmlConfBackend : public ConfBackend {
	public:
		MXmlConfBackend();
		~MXmlConfBackend();
		virtual void save( const std::string &key, 
				const std::string &value );
		virtual void save( const std::string &key, 
				const int32_t value );

		virtual std::string loadString( const std::string &key, 
				const std::string &defaultValue="" );
		virtual int32_t loadInt( const std::string &key, 
				const int32_t defaultValue=0 );

		virtual void commit();


		 std::string getMemObjectType(){return "MXmlConfBackend";}
	private:
		std::string getDefaultConfigFilename();
		std::string fileName;
		XMLFileParser * parser;
};
