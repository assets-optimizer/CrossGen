#include "fmain.hpp"


MainFrame::MainFrame( wxWindow* parent): VMainFrame(parent) {
    // Hack for better background
    #ifdef __WINDOWS__
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    #endif
    _isDictLoaded = false;
    srand(time(NULL));
    auto *config = wxConfigBase::Get();
    wxSize sz;
    sz.SetHeight(config->Read(SETTINGS_KEY_FMAIN_HEIGHT, SETTINGS_DEFAULT_FMAIN_HEIGHT));
    sz.SetWidth(config->Read(SETTINGS_KEY_FMAIN_WIDTH, SETTINGS_DEFAULT_FMAIN_WIDTH));
    SetSize(sz);
}

void MainFrame::procDict(wxString path){
    _dict.clear();
    _allWords.clear();
    _transType.clear();
    readDict(path, _dict);
    generateAllWords(_dict, _allWords, _transType);
    _isDictLoaded = true;
}

void MainFrame::onOpenGridClick(wxCommandEvent &event) {
    wxFileDialog dlgOpen(this, _("Open crossword file"), wxEmptyString, wxEmptyString,
    _("Files of crossword (*.cross)|*.cross"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    
    if ( dlgOpen.ShowModal() == wxID_CANCEL )
        return;
    wxFileInputStream input_stream(dlgOpen.GetPath());
    if ( !input_stream.IsOk() ) {
        wxMessageBox(_("Cannot open file ") + dlgOpen.GetPath(), _("Error"), wxICON_ERROR );
        return;
    }
    tPath->SetValue(dlgOpen.GetPath());
    
    // Clearing
    _grid.clear();
    _ques.clear();
    _ans.clear();
    tOutput->Clear();
    
    readGrid(tPath->GetValue(), _grid);
    
    SetGridImage(_grid);
}

void MainFrame::SetGridImage(GridType &grid, size_t w) {
    size_t h = static_cast<float>(w) / grid.size() * grid.at(0).size();
    wxBitmap bmp(w, h);
    wxMemoryDC dc;
    dc.SelectObject(bmp);
    dc.Clear();
    
    std::vector<WordInfo> winfos;
    generateWordInfo(grid, winfos);
    
    float sq_w = static_cast<int>(static_cast<float>(w) / grid.size());
    float sq_h = static_cast<int>(static_cast<float>(h) / grid.at(0).size());
    
    wxFont cur_f = dc.GetFont();
    cur_f.SetPointSize(sq_h/3);
    
    dc.SetBrush(wxBrush(wxColour(217,231,200)));
    dc.SetFont(cur_f);
    
    for (size_t i = 0; i < grid.size(); ++i){
        for (size_t j = 0; j < grid.at(0).size(); ++j){
            if ( grid.at(i).at(j) == CELL_CLEAR )
                dc.DrawRectangle(sq_w*i,sq_h*j,sq_w+1,sq_h+1);
        }
    }
    
    for (size_t i = 0; i < winfos.size(); ++i) {
        dc.DrawText(wxString::Format(wxT("%d"),winfos.at(i).ind), sq_w*winfos.at(i).x, sq_h*winfos.at(i).y);
    }
    
    if ( _ans.size() > 0 ) {
        using std::vector;
        vector< vector< bool > > usedCells(
            grid.size(), 
            vector<bool>(grid.at(0).size(), false)
        );
        cur_f = dc.GetFont();
        cur_f.SetPointSize(sq_h*0.6);
        dc.SetFont(cur_f);
        for ( size_t i = 0; i < winfos.size(); ++i ) {
            if ( winfos.at(i).direct == true ) {
                for (size_t j = 0; j < winfos.at(i).len; ++j)
                    if ( !usedCells.at(winfos.at(i).x+j).at(winfos.at(i).y) ) {
                        dc.DrawText(
                            _ans.at(i).at(j),
                            sq_w*(winfos.at(i).x+j) + sq_w*0.24,
                            sq_h*winfos.at(i).y
                        );
                        usedCells.at(winfos.at(i).x+j).at(winfos.at(i).y) = true;
                    }
            } else {
                for (size_t j = 0; j < winfos.at(i).len; ++j)
                    if ( !usedCells.at(winfos.at(i).x).at(winfos.at(i).y+j) ) {
                        dc.DrawText(
                            _ans.at(i).at(j),
                            sq_w*winfos.at(i).x + sq_w*0.24,
                            sq_h*(winfos.at(i).y+j)
                        );
                        usedCells.at(winfos.at(i).x).at(winfos.at(i).y+j) = true;
                    }
            }
        }
    }
    
    bPreview->SetBitmap(bmp);
    bPreview->Show();
    
    dc.SelectObject( wxNullBitmap );
    
    this->Refresh();
    this->GetSizer()->RecalcSizes();
}

void MainFrame::onGenerateClick(wxCommandEvent &event) {
    auto config = wxConfigBase::Get();
    if ( !_isDictLoaded ) {
        procDict(config->Read(SETTINGS_KEY_DICT_PATH, SETTINGS_DEFAULT_DICTPATH));
    }
    
    std::vector<wxString> words_out;
    if ( _grid.empty() ){
        wxMessageBox( _("Crossword grid isn't loaded!"), _("Warning"), wxICON_WARNING);
        return;
    }
    try {
        generateCross(_grid, _allWords, _transType, words_out);
        
        _ans = words_out;
        
        std::vector<WordInfo> winfos;
        generateWordInfo(_grid, winfos);
        
        if ( winfos.size() == 0 ) 
            throw 42;
        
        _ques.clear();
        for (size_t i = 0; i < words_out.size(); ++i)
            _ques.push_back(getRandInterval(
                _dict.lower_bound(words_out.at(i)),
                _dict.upper_bound(words_out.at(i))
            )->second);
        
        tOutput->Clear();
        
        FilledCrossword t_cross;
        t_cross.words = winfos;
        t_cross.grid  = _grid;
        t_cross.ans   = _ans;
        t_cross.ques  = _ques;
        
        tOutput->AppendText(getQuesString(t_cross));
        
        SetGridImage(_grid);
    }
    catch ( ... ){
        tOutput->Clear();
        wxMessageBox( _("Cannot generate crossword"), _("Error"), wxICON_ERROR );
    }
    this->Refresh();
}

void MainFrame::onExportClick(wxCommandEvent& event) {
    if ( _grid.empty() ) {
        wxMessageBox( _("Grid isn't loaded now"), _("Info"), wxICON_WARNING );
        return;
    }
     wxFileDialog dlgSave(this, _("Exporting crossword"), wxEmptyString, wxEmptyString,
        _("txt files (*.txt)|*.txt"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if ( dlgSave.ShowModal() == wxID_CANCEL )
        return;
    FilledCrossword t_cross;
    t_cross.grid = _grid;
    t_cross.ans  = _ans;
    t_cross.ques = _ques;
    generateWordInfo(_grid, t_cross.words);
    if ( !exportToFile(t_cross, dlgSave.GetPath()) ){
        wxLogError(_("Cannot save current contents to file '%s'."), dlgSave.GetPath().GetData());
        return;
    }
    wxLogDebug(wxT("Exporting to ") + dlgSave.GetPath() + wxT(" is complete"));
}

void MainFrame::onSettingsClick( wxCommandEvent& event ){
    SettingsDialog fSettings(this);
    auto *config = wxConfigBase::Get();
    fSettings.setDictPath(config->Read(SETTINGS_KEY_DICT_PATH, SETTINGS_DEFAULT_DICTPATH));
    if ( fSettings.ShowModal() == wxID_OK ){
        if ( config->Read(SETTINGS_KEY_DICT_PATH, SETTINGS_DEFAULT_DICTPATH) != fSettings.getDictPath() ) {
            config->Write(SETTINGS_KEY_DICT_PATH, fSettings.getDictPath());
            procDict(fSettings.getDictPath());
        }
    }
}

void MainFrame::onWindowClose( wxCloseEvent& event ){
    saveConfig();
    event.Skip();
}

void MainFrame::onExitClick( wxCommandEvent& event ){
    this->Close();
}

void MainFrame::saveConfig(){
    auto *config = wxConfigBase::Get();
    auto sz = GetSize();
    config->Write(SETTINGS_KEY_FMAIN_HEIGHT, sz.GetHeight());
    config->Write(SETTINGS_KEY_FMAIN_WIDTH, sz.GetWidth());
}
