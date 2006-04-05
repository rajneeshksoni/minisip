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
 *          Ignacio Sanchez Pardo <isp@kth.se>
*/

#ifndef FLOAT_RESAMPLER_H
#define FLOAT_RESAMPLER_H

#ifdef FLOAT_RESAMPLER

#include<libminisip/libminisip_config.h>

#include<libminisip/soundcard/Resampler.h>
#include<samplerate.h>

class FloatResampler : public Resampler {
	public: 

		FloatResampler( uint32_t inputFreq, uint32_t outputFreq, 
			        uint32_t duration, uint32_t nChannels );

		~FloatResampler();

		virtual void resample( short * input, short * output );

	private:
		uint32_t inputLength;
		uint32_t outputLength;

		SRC_DATA * src_data;
		SRC_STATE * src_state;

		int error;

};


#endif

#endif

