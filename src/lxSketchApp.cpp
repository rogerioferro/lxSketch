
#include "lxSketchApp.h"
#include "lxSketchFrame.h"

#include <wx/image.h>

IMPLEMENT_APP(lxSketchApp);

bool lxSketchApp::OnInit()
{
    wxInitAllImageHandlers();
    lxSketchFrame* frame = new lxSketchFrame(0L, _("wxWidgets Application Template"));
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();

    return true;
}
