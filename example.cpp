#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/artprov.h>

// Include all animated class headers
#include "include/wxStaticTextAnimated.h"
#include "include/wxButtonAnimated.h"
#include "include/wxImageAnimated.h"

class MainFrame : public wxFrame {
public:
    MainFrame() : wxFrame(nullptr, wxID_ANY, "wxAnimator Example - All Classes", 
                         wxDefaultPosition, wxSize(800, 600)) {
        
        CreateControls();
        SetupLayout();
        BindEvents();
        
        SetMinSize(wxSize(600, 400));
    }

private:
    // Controls
    wxStaticTextAnimated* m_animatedText;
    wxButtonAnimated* m_animatedButton1;
    wxButtonAnimated* m_animatedButton2;
    wxImageAnimated* m_animatedImage;
    wxStaticText* m_infoText;
    
    void CreateControls() {
        // Create main panel
        wxPanel* mainPanel = new wxPanel(this);
        
        // Create animated static text
        m_animatedText = new wxStaticTextAnimated(mainPanel, wxID_ANY, 
            "Click buttons to see animations!", 
            wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
        m_animatedText->SetBackgroundColour(wxColour(240, 240, 240));
        
        // Create animated buttons
        m_animatedButton1 = new wxButtonAnimated(mainPanel, wxID_ANY, 
            "Animate Text");
        m_animatedButton2 = new wxButtonAnimated(mainPanel, wxID_ANY, 
            "Animate Image");
        
        // Create animated image with a sample image
        wxBitmap bitmap = wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(64, 64));
        wxImage image = bitmap.ConvertToImage();
        m_animatedImage = new wxImageAnimated(mainPanel, image, wxID_ANY, 
                                             wxDefaultPosition, wxSize(64, 64));
        
        // Create info text
        m_infoText = new wxStaticText(mainPanel, wxID_ANY, 
            "This example demonstrates all animated classes:\n"
            "• wxStaticTextAnimated - Animated text with font size and color changes\n"
            "• wxButtonAnimated - Animated buttons with click effects\n"
            "• wxImageAnimated - Animated images with scaling and rotation effects",
            wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_WORDWRAP);
        m_infoText->SetBackgroundColour(wxColour(250, 250, 220));
        
        // Set up the main panel as the window's content
        mainPanel->SetSizerAndFit(SetupMainPanelLayout(mainPanel, 
            m_animatedText, m_animatedButton1, m_animatedButton2, 
            m_animatedImage, m_infoText));
    }
    
    wxSizer* SetupMainPanelLayout(wxPanel* panel, 
                                  wxStaticTextAnimated* text,
                                  wxButtonAnimated* btn1,
                                  wxButtonAnimated* btn2,
                                  wxImageAnimated* img,
                                  wxStaticText* info) {
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
        
        // Title section
        mainSizer->AddSpacer(10);
        wxStaticText* title = new wxStaticText(panel, wxID_ANY, 
            "wxAnimator Library Example", 
            wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
        wxFont titleFont = title->GetFont();
        titleFont.SetPointSize(16);
        titleFont.SetWeight(wxFONTWEIGHT_BOLD);
        title->SetFont(titleFont);
        mainSizer->Add(title, 0, wxALL | wxCENTER, 10);
        
        // Animated text section
        mainSizer->AddSpacer(10);
        wxStaticText* textLabel = new wxStaticText(panel, wxID_ANY, "Animated Text:");
        mainSizer->Add(textLabel, 0, wxLEFT | wxRIGHT, 20);
        mainSizer->Add(text, 0, wxALL | wxEXPAND, 20);
        
        // Buttons section
        mainSizer->AddSpacer(10);
        wxStaticText* btnLabel = new wxStaticText(panel, wxID_ANY, "Animated Buttons:");
        mainSizer->Add(btnLabel, 0, wxLEFT | wxRIGHT, 20);
        
        wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
        btnSizer->Add(btn1, 1, wxALL, 10);
        btnSizer->Add(btn2, 1, wxALL, 10);
        mainSizer->Add(btnSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 20);
        
        // Image section
        mainSizer->AddSpacer(10);
        wxStaticText* imgLabel = new wxStaticText(panel, wxID_ANY, "Animated Image:");
        mainSizer->Add(imgLabel, 0, wxLEFT | wxRIGHT, 20);
        mainSizer->Add(img, 0, wxALL | wxCENTER, 20);
        
        // Info section
        mainSizer->AddSpacer(20);
        wxStaticText* infoLabel = new wxStaticText(panel, wxID_ANY, "Information:");
        mainSizer->Add(infoLabel, 0, wxLEFT | wxRIGHT, 20);
        mainSizer->Add(info, 1, wxALL | wxEXPAND, 20);
        
        return mainSizer;
    }
    
    void SetupLayout() {
        // Layout is handled in CreateControls
    }
    
    void BindEvents() {
        // Bind button click events
        m_animatedButton1->Bind(wxEVT_STATIC_BTN_LEFT_DOWN, [this](wxCommandEvent& event) {
            // Change the animated text when button 1 is clicked
            static int textIndex = 0;
            wxString texts[] = {
                "Text animation in progress!",
                "Notice the font size change!",
                "And the color transition!",
                "wxStaticTextAnimated working!",
                "Click the other button!"
            };
            
            m_animatedText->SetLabelText(texts[textIndex]);
            textIndex = (textIndex + 1) % 5;
            
            // Trigger the animation
            m_animatedText->doAnimations();
        });
        
        m_animatedButton2->Bind(wxEVT_STATIC_BTN_LEFT_DOWN, [this](wxCommandEvent& event) {
            // Animate the image when button 2 is clicked
            m_animatedImage->doAnimations();
        });
        
        // Bind window close event
        this->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& event) {
            Destroy();
        });
    }
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() override {
        MainFrame* frame = new MainFrame();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
