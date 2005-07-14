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


#include<config.h>

#include<libmutil/MemObject.h>
#include<libmutil/Mutex.h>
#include<string>

using namespace std;

#ifdef MDEBUG
#include<libmutil/itoa.h>
Mutex global;
minilist<MObject *> objs;
int ocount=0;
bool outputOnDestructor=false;
#endif

MObject::MObject() : refCount(0){
#ifdef MDEBUG
	global.lock();
	ocount++;
	objs.push_back(this);
	global.unlock();
#endif
}

MObject::~MObject(){
#ifdef MDEBUG	
	global.lock();
	for (int i=0; i<objs.size(); i++){
		if (this == objs[i]){
			objs.remove(i);
			ocount--;
			break;
		}
	}
	global.unlock();
#endif
}

int MObject::decRefCount(){
	int ref;
#ifdef MDEBUG
	global.lock();
#else
	refLock.lock();
#endif
	
	ref=--refCount;
	
#ifdef MDEBUG
	global.unlock();
	if (ref==0 && outputOnDestructor){
		string output = "~MO:"+getMemObjectType()+"\n";
		cerr << output << endl;
	}
#else
	refLock.unlock();
#endif
	return ref;
}

void MObject::incRefCount(){
#ifdef MDEBUG
	global.lock();
#else
	refLock.lock();
#endif
	
	refCount++;
	
#ifdef MDEBUG
	global.unlock();
#else
	refLock.unlock();
#endif
}

int MObject::getRefCount(){
	return refCount;
}

minilist<string> getMemObjectNames(){
#ifdef MDEBUG
	minilist<string> ret;
	global.lock();
	for (int i=0; i< objs.size(); i++){
		int count = objs[i]->getRefCount();
		string countstr = count?itoa(count):"on stack"; 
		ret.push_back(objs[i]->getMemObjectType()+"("+countstr+")");
	}
	global.unlock();
	return ret;
#else
	minilist<string> ret;
	return ret;
#endif
}

int getMemObjectCount(){
#ifdef MDEBUG
	return ocount;
#else
	return -1;
#endif
}

bool setDebugOutput(bool on){
#ifdef MDEBUG
	outputOnDestructor=on;
	return true;
#else
	return false;
#endif
}

bool getDebugOutputEnabled(){
#ifdef MDEBUG
	return outputOnDestructor;
#else
	return false;
#endif
}

