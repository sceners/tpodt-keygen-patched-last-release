/*
Broadcast:
  Sym : 4B11D8BC
  Chk : 7CE60D89
 Size : 4F
 Diff : 4C4
  MD5 : 380C21D0
Templ : 35
 Orig : 1480609348,5369488552660434430332817877220007,973349376941954201377298114029911
 Repl : 3543939100,423459548883516196853882930608375,8484360843179587455308070336071028
 Prvt : 4487453935223399054398123700273797

Studio:
  Sym : 8205FBB9
  Chk : 931D0076
 Size : 50
 Diff : 13B
  MD5 : 4E3418B7
Templ : 3
 Orig : 1568656911,2520785939399452688547335135189161,8194240765473853550166226692686387
 Repl : 2127088620,2888770628539854944085964743629587,3569353393377585389378801225448248
 Prvt : 931675772215283508885431996874350

Professional:
  Sym : E4FC7DE9
  Chk : EDA8A5ED
 Size : 4F
 Diff : 293
  MD5 : 60F74FE9
Templ : 16
 Orig : 558811559,1709701285150553392500395103846903,4732987328429883871863841480767479
 Repl : 2962705863,2986569122504981496443632156461035,808620215554464990791518453439232
 Prvt : 3696558672603767903274371819635632

Home:
  Sym : 868F63BD
  Chk : DAB8E5DD
 Size : 4F
 Diff : E6
  MD5 : 28887F94
Templ : 1
 Orig : 1464764738,2370657907885437965311543086728048,713247189102816759459019477615894
 Repl : 943901380,5852893500978533104296112875516146,6976869334719132365903529482995757
 Prvt : 2446684864137654178351500875025920
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include "keygen\keygen_main.h"
#include "lib\minifmod.h"
#include "Music\loadmusic.h"

HINSTANCE hInst;
FMUSIC_MODULE* mod;
char file[256]="";
char FormatTextHex_format[1024]=""; //String for hex format
unsigned int symmetric_selected=0x4B11D8BC;
char selected_private[256]="4487453935223399054398123700273797";
char selected_public[256]="3543939100,423459548883516196853882930608375,8484360843179587455308070336071028";

void CopyToClipboard(const char* text)
{
    HGLOBAL hText;
    char *pText;

    hText = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, strlen(text)+1);
    pText = (char*)GlobalLock(hText);
    strcpy(pText, text);

    OpenClipboard(0);
    EmptyClipboard();
    if(!SetClipboardData(CF_OEMTEXT, hText))
    {
        MessageBeep(MB_ICONERROR);
    }
    MessageBeep(MB_ICONINFORMATION);
    CloseClipboard();
}

char* FormatTextHex(const char* text)
{
    int len=strlen(text);
    FormatTextHex_format[0]=0;
    for(int i=0; i<len; i++)
    {
        if((text[i]>64 and text[i]<71) or(text[i]>47 and text[i]<58))
            sprintf(FormatTextHex_format, "%s%c", FormatTextHex_format, text[i]);
    }
    return FormatTextHex_format;
}

bool fileExsists(const char* filename)
{
    if(GetFileAttributes(filename)==0xFFFFFFFF)
        return false;
    return true;
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        loadmusic();
        mod=FMUSIC_LoadSong(NULL, NULL);
        FMUSIC_PlaySong(mod);
        SetDlgItemTextA(hwndDlg, IDC_EDT_HWID, "0000-0000");
        SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)));
        SendDlgItemMessageA(hwndDlg, IDC_COMBO_PRODUCT, CB_ADDSTRING, 0, (LPARAM)"VidBlaster Broadcast");
        SendDlgItemMessageA(hwndDlg, IDC_COMBO_PRODUCT, CB_ADDSTRING, 0, (LPARAM)"VidBlaster Studio");
        SendDlgItemMessageA(hwndDlg, IDC_COMBO_PRODUCT, CB_ADDSTRING, 0, (LPARAM)"VidBlaster Professional");
        SendDlgItemMessageA(hwndDlg, IDC_COMBO_PRODUCT, CB_ADDSTRING, 0, (LPARAM)"VidBlaster Home");
        SendDlgItemMessageA(hwndDlg, IDC_COMBO_PRODUCT, CB_SETCURSEL, 0, 0);
        GetCurrentDirectoryA(256, file);
        sprintf(file, "%s\\VidBlaster.exe", file);
        EnableWindow(GetDlgItem(hwndDlg, IDC_BTN_REGISTER), fileExsists(file));
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDC_BTN_REGISTER:
        {
            SetFocus(GetDlgItem(hwndDlg, IDC_EDT_SERIAL));
            char commandline[256]="";
            sprintf(commandline, "%s REGISTER", file);
            STARTUPINFO si= {0};
            si.cb=sizeof(STARTUPINFO);
            PROCESS_INFORMATION pi= {0};
            if(!CreateProcessA(0, commandline, 0, 0, 0, CREATE_DEFAULT_ERROR_MODE, 0, 0, &si, &pi))
            {
                MessageBoxA(hwndDlg, "Failed to create process...", "Error", MB_ICONERROR);
                return TRUE;
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        return TRUE;

        case IDC_EDT_HWID:
        {
            SendMessageA(hwndDlg, WM_COMMAND, IDC_BTN_GENERATE, 0);
        }
        return TRUE;

        case IDC_BTN_GENERATE:
        {
            if(lParam)
                SetFocus(GetDlgItem(hwndDlg, IDC_EDT_SERIAL));
            char name[256]="";
            SYSTEMTIME st= {0};
            GetLocalTime(&st);
            short today=MakeDate(st.wYear, st.wMonth, st.wDay);
            unsigned int hwid=0;
            char hwid_text[20]="";
            GetDlgItemTextA(hwndDlg, IDC_EDT_HWID, hwid_text, 20);
            sscanf(FormatTextHex(hwid_text), "%X", &hwid);
            SetDlgItemTextA(hwndDlg, IDC_EDT_SERIAL, CreateSignedKey(29, symmetric_selected, 0, selected_private, selected_public, 0, today, name, hwid, 0, 0, 0, 0, 0, 0));
        }
        return TRUE;

        case IDC_BTN_COPY:
        {
            SetFocus(GetDlgItem(hwndDlg, IDC_EDT_SERIAL));
            char serial[256]="";
            if(GetDlgItemTextA(hwndDlg, IDC_EDT_SERIAL, serial, 256))
                CopyToClipboard(serial);
        }
        return TRUE;

        case IDC_COMBO_PRODUCT:
        {
            switch(HIWORD(wParam))
            {
            case CBN_SELCHANGE:
            {
                SetFocus(GetDlgItem(hwndDlg, IDC_EDT_SERIAL));
                int selection=SendDlgItemMessageA(hwndDlg, LOWORD(wParam), CB_GETCURSEL, 0, 0);
                switch(selection)
                {
                case 0: //Broadcast
                    symmetric_selected=0x4B11D8BC;
                    strcpy(selected_private, "4487453935223399054398123700273797");
                    strcpy(selected_public, "3543939100,423459548883516196853882930608375,8484360843179587455308070336071028");
                    SendMessageA(hwndDlg, WM_COMMAND, IDC_BTN_GENERATE, 0);
                    break;
                case 1: //Studio
                    symmetric_selected=0x8205FBB9;
                    strcpy(selected_private, "931675772215283508885431996874350");
                    strcpy(selected_public, "2127088620,2888770628539854944085964743629587,3569353393377585389378801225448248");
                    SendMessageA(hwndDlg, WM_COMMAND, IDC_BTN_GENERATE, 0);
                    break;
                case 2: //Professional
                    symmetric_selected=0xE4FC7DE9;
                    strcpy(selected_private, "3696558672603767903274371819635632");
                    strcpy(selected_public, "2962705863,2986569122504981496443632156461035,808620215554464990791518453439232");
                    SendMessageA(hwndDlg, WM_COMMAND, IDC_BTN_GENERATE, 0);
                    break;
                case 3: //Home
                    symmetric_selected=0x868F63BD;
                    strcpy(selected_private, "2446684864137654178351500875025920");
                    strcpy(selected_public, "943901380,5852893500978533104296112875516146,6976869334719132365903529482995757");
                    SendMessageA(hwndDlg, WM_COMMAND, IDC_BTN_GENERATE, 0);
                    break;
                }
            }
            return TRUE;
            }
        }
        return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
