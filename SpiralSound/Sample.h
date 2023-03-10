/* SpiralSound
 *     - Copyleft (C) 2016 Andy Preston <edgeeffect@gmail.com>
 * based on SpiralSynthModular
 *     - Copyleft (C) 2002 David Griffiths <dave@pawfal.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef SAMPLE
#define SAMPLE

#include <iostream>
#include <limits.h>
#include <cstring>

inline float Linear(float bot,float top,float pos,float val1,float val2)
{
    float t=(pos-bot)/(top-bot);
    return val1*t + val2*(1.0f-t);
}

inline bool feq(float a, float b, float tol)
{
	return (a>b-tol && a<b+tol);
}

class Sample
{
public:
	Sample(int Len=0);
	Sample(const Sample &rhs);
	Sample(const float *S, int Len);
	~Sample();

	bool Allocate(int Size);
	void Clear();
	void Zero();
	void Set(float Val);
	void Insert(const Sample &S, int Pos);
	void Add(const Sample &S);
	void Mix(const Sample &S, int Pos);
	void Remove(int Start, int End);
	void Reverse(int Start, int End);
	void Move(int Dist);
	void GetRegion(Sample &S, int Start, int End) const;
	const float *GetBuffer() const {return m_Data;}
	float *GetNonConstBuffer() {return m_Data;}
	int  GetLength() const {return m_Length;}
	int  GetLengthInBytes() const {return m_Length*sizeof(float);}
	void Expand(int Length);
	void Shrink(int Length);
	void CropTo(int NewLength);
	bool IsEmpty() const { return m_IsEmpty; }

	inline float &operator[](int i) const
	{
		return m_Data[i];
	}

	// Linear interpolated
	inline float operator[](float i) const
	{
		int ii=(int)i;
		if (ii==m_Length-1) return m_Data[ii];
		float t=i-ii;
		return ((m_Data[ii]*(1-t))+(m_Data[ii+1])*t);
	}


	inline void Set(int i, float v)
	{
		m_IsEmpty=false;
		m_Data[i]=v;
	}

	inline Sample &operator=(const Sample &rhs)
	{
		Allocate(rhs.GetLength());
		memcpy(m_Data,rhs.GetBuffer(),GetLengthInBytes());
		m_IsEmpty=rhs.m_IsEmpty;
		return *this;
	}

	void SetDataGranularity(int s) { m_DataGranularity=s; }

    //void setSpecificData(void *ptr) { m_PluginSpecificData = ptr; }
    //void *getSpecificData() { return m_PluginSpecificData; }
protected:
	bool m_IsEmpty;

private:
	int m_DataGranularity;
	float *m_Data;
	long  int  m_Length;
	//void *m_PluginSpecificData;
};

#endif