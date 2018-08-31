//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
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
//    for (any comment, suggestion or feature request, please contact the manager of
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

// for (compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/xrc/xmlres.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>

#include "dlg_compile_book.h"

enum {
    lmID_SOURCE = 2600,
    lmID_BTN_BROWSE_SRC,
    lmID_DESTINATION,
    lmID_BTN_BROWSE_DEST,
    lmID_CHK_DUMP_TREE,
    lmID_CHK_LOG_TREE,
    lmID_LANG,
};

//table must be ordered by language name (in English) to
//ensure correspondence with table in dlg_compile_book.h
//English MUST BE first. All other in alphabetical order
const lmLangData g_tLanguages[lmNUM_LANGUAGES] = {
    { _T("en"),     _T("English") },
    { _T("eu"),     _T("Basque") },
    { _T("zh_CN"),  _T("Chinese (simplified)") },
    { _T("nl"),     _T("Dutch") },
    { _T("fr"),     _T("French") },
    { _T("gl_ES"),  _T("Galician") },
    { _T("de"),     _T("German") },
    { _T("el"),     _T("Greek") },
    { _T("it"),     _T("Italian") },
    { _T("ru"),     _T("Russian") },
    { _T("es"),     _T("Spanish") },
    { _T("tr"),     _T("Turkish") },
};


//-----------------------------------------------------------------------------
// Event table: connect the events to the handler functions to process them
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(lmDlgCompileBook, wxDialog)
    EVT_BUTTON(wxID_OK, lmDlgCompileBook::OnAcceptClicked )
    EVT_BUTTON(wxID_CANCEL, lmDlgCompileBook::OnCancelClicked )

    // Language check boxes
    EVT_COMMAND_RANGE(lmID_LANG, lmID_LANG+eLangLast, wxEVT_COMMAND_CHECKBOX_CLICKED , lmDlgCompileBook::OnDataChanged )
    EVT_CHECKBOX(lmID_CHK_LOG_TREE, lmDlgCompileBook::OnDataChanged )
    EVT_CHECKBOX(lmID_CHK_DUMP_TREE, lmDlgCompileBook::OnDataChanged )

    EVT_BUTTON(lmID_BTN_BROWSE_SRC, lmDlgCompileBook::OnBrowseSrc )
    EVT_BUTTON(lmID_BTN_BROWSE_DEST, lmDlgCompileBook::OnBrowseDest )

END_EVENT_TABLE()



lmDlgCompileBook::lmDlgCompileBook(wxWindow* parent, lmCompileBookOptions* pOptions)
    : wxDialog(parent, wxID_ANY, wxT("eMusicBook compilation"), wxDefaultPosition,
               wxSize(530,600), wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX )
    , m_pOptions(pOptions)
{
    // create the dialog controls
    Create();

    // initialize all controls with current data
    for(int i=0; i < eLangLast; i++)
    {
        m_pOptions->fLanguage[i] = false;
        m_pChkLang[i]->SetValue(false);
    }
    m_pChkLang[eLangLast]->SetValue(false);

    m_pTxtSrcPath->SetValue( m_pOptions->sSrcPath );
    m_pTxtDestPath->SetValue( m_pOptions->sDestPath );

    //center dialog on screen
    CentreOnScreen();
}

lmDlgCompileBook::~lmDlgCompileBook()
{
}

void lmDlgCompileBook::Create()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetExtraStyle( wxWS_EX_BLOCK_EVENTS );

	wxBoxSizer* pMainSizer;
	pMainSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* pSourceSizer;
	pSourceSizer = new wxBoxSizer( wxHORIZONTAL );

	pLblSrcPath = new wxStaticText( this, wxID_ANY, wxT("Source xml file"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	pLblSrcPath->Wrap( -1 );
	pSourceSizer->Add( pLblSrcPath, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5 );

	m_pTxtSrcPath = new wxTextCtrl( this, lmID_SOURCE, wxEmptyString, wxDefaultPosition, wxSize( 300,-1 ), 0 );
	pSourceSizer->Add( m_pTxtSrcPath, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	pBtBrowseSrc = new wxButton( this, lmID_BTN_BROWSE_SRC, wxT("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
	pBtBrowseSrc->SetDefault();
	pSourceSizer->Add( pBtBrowseSrc, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	pMainSizer->Add( pSourceSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	wxBoxSizer* pDestSizer;
	pDestSizer = new wxBoxSizer( wxHORIZONTAL );

	pLblDestPath = new wxStaticText( this, wxID_ANY, wxT("Destination folder"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	pLblDestPath->Wrap( -1 );
	pDestSizer->Add( pLblDestPath, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5 );

	m_pTxtDestPath = new wxTextCtrl( this, lmID_DESTINATION, wxEmptyString, wxDefaultPosition, wxSize( 300,-1 ), 0 );
	pDestSizer->Add( m_pTxtDestPath, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	pBtBrowseDest = new wxButton( this, lmID_BTN_BROWSE_DEST, wxT("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
	pBtBrowseDest->SetDefault();
	pDestSizer->Add( pBtBrowseDest, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	pMainSizer->Add( pDestSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	wxBoxSizer* pOptionsSizer;
	pOptionsSizer = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* pLangsSizer;
	pLangsSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Languages") ), wxVERTICAL );



    //Create languages check boxes
    for (int i=0; i < eLangLast; i++)
    {
        m_pChkLang[i] = new wxCheckBox( this, lmID_LANG+i, g_tLanguages[i].sLangName, wxDefaultPosition, wxDefaultSize, 0 );
	    pLangsSizer->Add( m_pChkLang[i], 0, wxALIGN_LEFT, 5 );
    }
	pLangsSizer->Add( 5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	m_pChkLang[eLangLast] = new wxCheckBox( this, lmID_LANG+eLangLast, wxT("All languages"), wxDefaultPosition, wxDefaultSize, 0 );
	pLangsSizer->Add( m_pChkLang[eLangLast], 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );



	pOptionsSizer->Add( pLangsSizer, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxStaticBoxSizer* pDbgSizer;
	pDbgSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Debug options") ), wxVERTICAL );

	m_pChkDumpTree = new wxCheckBox( this, lmID_CHK_DUMP_TREE, wxT("Dump tree"), wxDefaultPosition, wxDefaultSize, 0 );

	pDbgSizer->Add( m_pChkDumpTree, 0, wxALIGN_LEFT|wxALL, 5 );

	m_pChkLogTree = new wxCheckBox( this, lmID_CHK_LOG_TREE, wxT("Log tree analysis"), wxDefaultPosition, wxDefaultSize, 0 );

	pDbgSizer->Add( m_pChkLogTree, 0, wxALIGN_LEFT|wxALL, 5 );

	pOptionsSizer->Add( pDbgSizer, 0, wxALIGN_TOP|wxALL, 5 );

	pMainSizer->Add( pOptionsSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	btOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	btOK->SetDefault();
	bSizer5->Add( btOK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    btOK->Enable(false);

	btCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	btCancel->SetDefault();
	bSizer5->Add( btCancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	pMainSizer->Add( bSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	this->SetSizer( pMainSizer );
	this->Layout();

	this->Centre( wxBOTH );
}

void lmDlgCompileBook::OnDataChanged(wxCommandEvent& WXUNUSED(event))
{
    VerifyData();
}

void lmDlgCompileBook::OnCancelClicked(wxCommandEvent& WXUNUSED(event))
{
    EndDialog(wxID_CANCEL);
}

void lmDlgCompileBook::OnAcceptClicked(wxCommandEvent& WXUNUSED(event))
{
    // Accept button will be enabled only if all data have been validated and is Ok. So
    // when accept button is clicked we can proceed to save data.

    //move language options
    if (m_pChkLang[eLangLast]->GetValue()) {
        for(int i=0; i < eLangLast; i++) {
             m_pOptions->fLanguage[i] = true;
        }
    }
    else {
        for(int i=0; i < eLangLast; i++) {
            m_pOptions->fLanguage[i] = m_pChkLang[i]->GetValue();
        }
    }

    //move debug options
    m_pOptions->fLogTree = m_pChkLogTree->GetValue();
    m_pOptions->fDump = m_pChkDumpTree->GetValue();

    //terminate the dialog
    EndModal(wxID_OK);
}

bool lmDlgCompileBook::VerifyData()
{
    // Returns a boolean to enable or not the Accept button: Returns true if there are
    // errors preventing to continue processing.


    // check that at least one language is selected
    int i;
    bool fError = false;
    bool fAtLeastOne = false;
    for (i=0; i <= eLangLast; i++) {
        if (m_pChkLang[i]->GetValue()) {
            fAtLeastOne = true;
            break;
        }
    }
    fError = !fAtLeastOne;


    //enable / disable accept button
    btOK->Enable(!fError);

    return fError;

}

void lmDlgCompileBook::OnBrowseSrc(wxCommandEvent& WXUNUSED(event))
{
    // ask for the file to covert
    wxString sFilter = wxT("*.xml");
    wxString sPath = ::wxFileSelector(_T("Choose the file to convert"),
                                     m_pOptions->sSrcPath,    //default path
                                        wxT(""),    //default filename
                                        wxT("xml"),    //default_extension
                                        sFilter,
                                        wxFD_OPEN,        //flags
                                        this);
    if ( sPath.IsEmpty() ) return;
    m_pOptions->sSrcPath = sPath;
    m_pTxtSrcPath->SetValue(sPath);

}

void lmDlgCompileBook::OnBrowseDest(wxCommandEvent& WXUNUSED(event))
{
    const wxString& dir = ::wxDirSelector(_T("Choose destination folder (lenmus/locale)"),
                                          m_pOptions->sDestPath,    //default path
                                          0,         //style
                                          wxDefaultPosition,
                                          this);
    if ( dir.empty() )
        return;
    m_pOptions->sDestPath = dir;
    m_pTxtDestPath->SetValue(dir);

}

