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
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 * Copyright (C) 2004-2006
 *
 * Authors: Erik Eliasson <eliasson@it.kth.se>
 *          Johan Bilien <jobi@via.ecp.fr>
 *          Mikael Magnusson <mikma@users.sourceforge.net>
*/

#include<config.h>
#include<libmutil/Library.h>
#include<libmutil/massert.h>

#include<windows.h>

#include<iostream>


using namespace std;

int Library::refCount=0;

Library::Library(const string &path_):path(path_){
	refCount++;
	handle = new HMODULE;
	*((HMODULE*)handle) = LoadLibrary( path.c_str() );
	if ( (*((HMODULE*)handle))==NULL){
		delete handle;
		handle = NULL;
	}	
}

Library::~Library(){
	if (handle){
		BOOL ok = FreeLibrary( *((HMODULE*)handle) );
		massert(ok);
		handle = NULL;
	}
	refCount--;
}

void *Library::getFunctionPtr(string name){
	return (void*)GetProcAddress( *((HMODULE*)handle), name.c_str() );
}


MRef<Library *> Library::open(const string &path){
	MRef<Library *> ret = new Library(path);
	if(ret->handle){
		ret->path = path;
		return ret;
	}
	ret = NULL;
	return ret;
}

const string &Library::getPath(){
	return path;
}
