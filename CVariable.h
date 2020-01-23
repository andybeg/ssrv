/*
 * CVariable.h
 *
 * Copyright 2014 kon <kon@kononov>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */


#ifndef CVARIABLE_H
#define CVARIABLE_H

template <typename T>
class CVariable
{
    public:
        CVariable() { setted = false; };
        void Set(T mVar)
        {
            Var = mVar;
            setted = true;
        }
        T Get(T newVar)
        {
            if(setted)
                return Var;
            else
                return newVar;
        }
        bool IsSet()
        {
            return setted;
        }

    private:
        T Var;
        bool setted;
        /* add your private declarations */
};

#endif /* CVARIABLE_H */
