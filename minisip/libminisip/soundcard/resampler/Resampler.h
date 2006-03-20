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

/* Copyright (C) 2004, 2005
 *
 * Authors: Erik Eliasson <eliasson@it.kth.se>
 *          Johan Bilien <jobi@via.ecp.fr>
 *          Ignacio Sanchez Pardo <isp@kth.se>
*/

#ifndef RESAMPLER_H
#define RESAMPLER_H

#include<libmutil/MemObject.h>

class Resampler : public MObject{
	public: 
		static MRef<Resampler *> create(
				    uint32_t inputFreq, uint32_t outputFreq,
		                    uint32_t duration, uint32_t nChannels );

		virtual void resample( short * input, short * output )=0;

		virtual std::string getMemObjectType(){return "Resampler";};


};

#endif
