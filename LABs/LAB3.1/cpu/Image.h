/*
 * Copyright (C) 2020 Universitat Autonoma de Barcelona - David Castells-Rufas <david.castells@uab.cat>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DATATYPES_H_INCLUDED_
#define DATATYPES_H_INCLUDED_

class Image
{
public:
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;

        // not compliant with java.awt.Image
        virtual int getRGB(int x, int y) = 0;
        virtual void getRGB(int x, int y, int* r, int* g, int* b) = 0;
        
};



#endif
