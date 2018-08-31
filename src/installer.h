//--------------------------------------------------------------------------------------
//    LenMus langtool: A tool for compiling and translating LenMus eBooks
//    Copyright (c) 2002-2016
//
//    This program is free software; you can redistribute it and/or modify it under the
//    terms of the GNU General Public License as published by the Free Software Foundation;
//    either version 3 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this
//    program. If not, see <http://www.gnu.org/licenses/>.
//
//
//    For any comment, suggestion or feature request, please contact the manager of
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------
#ifdef __GNUG__
// #pragma interface
#endif

#ifndef __INSTALLER_H__        //to avoid nested includes
#define __INSTALLER_H__


class lmInstaller
{
public:
    lmInstaller();
    ~lmInstaller();

    static wxString GetInstallerStrings(wxString sLangCode, wxString sLangName);

private:

};


#endif    // __INSTALLER_H__
