
// G910_SAMPLEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "G910_SAMPLE.h"
#include "G910_SAMPLEDlg.h"
#include "afxdialogex.h"

#pragma comment(lib, "LogitechLEDLib.lib")

#include "LogitechLEDLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ULONG_PTR g_gdiplusToken;

#define RGB_MIN 0
#define RGB_MAX 255
#define RGB_PERCENT_MAX 100
#define ALPHA_MAX 255

// CG910_SAMPLEDlg dialog

CG910_SAMPLEDlg::CG910_SAMPLEDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CG910_SAMPLEDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CG910_SAMPLEDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHECK3, m_perKeyCheckBox);
    DDX_Control(pDX, IDC_CHECK1, m_rgbCheckBox);
    DDX_Control(pDX, IDC_CHECK2, m_monochromeCheckBox);
    DDX_Control(pDX, IDC_SINGLEKEY_START_COLOR, m_singleKeyStartColorPicker);
    DDX_Control(pDX, IDC_SINGLEKEY_FINISH_COLOR, m_singleKeyFinishColorPicker);
    DDX_Control(pDX, IDC_CHECK_PERKEY_PULSE_LOOP, m_perKeyPulseLoop);
    DDX_Control(pDX, IDC_CHECK_EXCLUDE_NUMPAD, m_disableNumpad);
}

BEGIN_MESSAGE_MAP(CG910_SAMPLEDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_BUTTON_SETLIGHTING, &CG910_SAMPLEDlg::OnBnClickedButtonSetlighting)
    ON_BN_CLICKED(IDC_BUTTON_D, &CG910_SAMPLEDlg::OnBnClickedButtonNumLock)
    ON_BN_CLICKED(IDC_BUTTON_SAVE_LIGHTING, &CG910_SAMPLEDlg::OnBnClickedButtonSaveLighting)
    ON_BN_CLICKED(IDC_BUTTON_RESTORE_LIGHTING, &CG910_SAMPLEDlg::OnBnClickedButtonRestoreLighting)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR_LIGHTING, &CG910_SAMPLEDlg::OnBnClickedButtonClearLighting)
    ON_BN_CLICKED(IDC_BUTTON_STOP_EFFECTS, &CG910_SAMPLEDlg::OnBnClickedButtonStopEffects)
    ON_WM_SYSCOMMAND()
    ON_BN_CLICKED(IDC_CHECK1, &CG910_SAMPLEDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_CHECK2, &CG910_SAMPLEDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_CHECK3, &CG910_SAMPLEDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_GET_VERSION, &CG910_SAMPLEDlg::OnBnClickedGetVersion)
    ON_BN_CLICKED(IDC_BUTTON_INITIALIZE, &CG910_SAMPLEDlg::OnBnClickedButtonInitialize)
    ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, &CG910_SAMPLEDlg::OnBnClickedButtonShutdown)
    ON_BN_CLICKED(IDC_BUTTON_FLASH_R, &CG910_SAMPLEDlg::OnBnClickedButtonFlashR)
    ON_BN_CLICKED(IDC_BUTTON_PULSE_1, &CG910_SAMPLEDlg::OnBnClickedButtonPulse1)
    ON_BN_CLICKED(IDC_BUTTON_FKEYS, &CG910_SAMPLEDlg::OnBnClickedButtonFkeys)
    ON_BN_CLICKED(IDC_CHECK_EXCLUDE_NUMPAD, &CG910_SAMPLEDlg::OnBnClickedCheckExcludeNumpad)
    ON_BN_CLICKED(IDC_BUTTON_SET_GKEYS, &CG910_SAMPLEDlg::OnBnClickedButtonSetGkeys)
END_MESSAGE_MAP()


// CG910_SAMPLEDlg message handlers

BOOL CG910_SAMPLEDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
    if(!LogiLedInit())
    {
            MessageBox(L"Couldn't initialize LED SDK",L"ERROR",0);
    }
    m_targetDevice = LOGI_DEVICETYPE_ALL;

    // set the edit boxes to have valid numbers
    SetDlgItemInt(IDC_EDIT_RED, 100,0);
    SetDlgItemInt(IDC_EDIT_GREEN, 0,0);
    SetDlgItemInt(IDC_EDIT_BLUE, 50,0);
    SetDlgItemInt(IDC_EDIT_INTERVAL, 300,0);
    SetDlgItemInt(IDC_EDIT_DURATION, 6000,0);

    //setup scrollbars
    m_perKeyCheckBox.SetCheck(1);
    m_perKeyPulseLoop.SetCheck(1);
    m_monochromeCheckBox.SetCheck(1);
    m_rgbCheckBox.SetCheck(1);
    m_singleKeyStartColorPicker.SetColor(RGB(255,0,0));
    m_targetDevice = LOGI_DEVICETYPE_ALL;
    LogiLedSetTargetDevice(m_targetDevice);
    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CG910_SAMPLEDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CG910_SAMPLEDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// change a range from oldMin - oldMax to newMin - newMax
int CG910_SAMPLEDlg::ChangeRange(int oldVal, int oldMin, int oldMax, int newMin, int newMax)
{
    return (((oldVal - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin;
}

// check the bounds of whatever is in the box to make sure nothing goes out of range
void CG910_SAMPLEDlg::CheckBounds()
{
    // get the values to be checked
    int checkRed = GetDlgItemInt(IDC_EDIT_RED, 0,0);
    int checkGreen = GetDlgItemInt(IDC_EDIT_GREEN, 0,0);
    int checkBlue = GetDlgItemInt(IDC_EDIT_BLUE, 0,0);
    int checkInterval = GetDlgItemInt(IDC_EDIT_INTERVAL, 0,0);

    // check to see if any are above max/below min, then set the checked val to the valid value
    checkRed > RGB_PERCENT_MAX ? checkRed = RGB_PERCENT_MAX : checkRed = checkRed;
    checkRed < RGB_MIN ? checkRed = RGB_MIN : checkRed = checkRed;

    checkGreen > RGB_PERCENT_MAX ? checkGreen = RGB_PERCENT_MAX : checkGreen = checkGreen;
    checkGreen < RGB_MIN ? checkGreen = RGB_MIN : checkGreen = checkGreen;

    checkBlue > RGB_PERCENT_MAX ? checkBlue = RGB_PERCENT_MAX : checkBlue = checkBlue;
    checkBlue < RGB_MIN ? checkBlue = RGB_MIN : checkBlue = checkBlue;

    // it wouldn't make sense to have an interval of 0, so make it 50 min (super fast for blinking - good for pulsing)
    checkInterval < 50 ? SetDlgItemInt(IDC_EDIT_INTERVAL, 50,0) : SetDlgItemInt(IDC_EDIT_INTERVAL, checkInterval,0);

    // reflect those changes on the sample
    SetDlgItemInt(IDC_EDIT_RED, checkRed,0);
    SetDlgItemInt(IDC_EDIT_GREEN, checkGreen,0);
    SetDlgItemInt(IDC_EDIT_BLUE, checkBlue,0);
    SetDlgItemInt(IDC_EDIT_INTERVAL, checkInterval,0);
}

void CG910_SAMPLEDlg::OnBnClickedButtonSetlighting()
{
    CheckBounds();

    int redVal = GetDlgItemInt(IDC_EDIT_RED, 0,0);
    int greenVal = GetDlgItemInt(IDC_EDIT_GREEN, 0,0);
    int blueVal = GetDlgItemInt(IDC_EDIT_BLUE, 0,0);


    // TODO: Add your control notification handler code here
    LogiLedSetLighting(redVal, greenVal, blueVal);
}


void CG910_SAMPLEDlg::OnBnClickedButtonNumLock()
{
    // This is an example of how to use the function LogiLedSetLightingForKeyWithKeyName
    CheckBounds();

    int redVal = GetDlgItemInt(IDC_EDIT_RED, 0,0);
    int greenVal = GetDlgItemInt(IDC_EDIT_GREEN, 0,0);
    int blueVal = GetDlgItemInt(IDC_EDIT_BLUE, 0,0);

    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::NUM_LOCK, redVal, greenVal, blueVal);
}


void CG910_SAMPLEDlg::OnBnClickedButtonSaveLighting()
{
    // TODO: Add your control notification handler code here
    LogiLedSaveCurrentLighting();
}


void CG910_SAMPLEDlg::OnBnClickedButtonRestoreLighting()
{
    // TODO: Add your control notification handler code here
    LogiLedRestoreLighting();
}


void CG910_SAMPLEDlg::OnBnClickedButtonClearLighting()
{
    // TODO: Add your control notification handler code here
    LogiLedSetLighting(0,0,0);
}

void CG910_SAMPLEDlg::OnBnClickedButtonStopEffects()
{
    LogiLedStopEffects();
}


void CG910_SAMPLEDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if(nID == SC_CLOSE)
    {
        LogiLedShutdown();
    }

        CDialog::OnSysCommand(nID, lParam);

}




void CG910_SAMPLEDlg::OnBnClickedCheck1()
{
    m_targetDevice = 0;
    UpdateData();
    if(m_rgbCheckBox.GetCheck())
    {
        m_targetDevice |= LOGI_DEVICETYPE_RGB;
    }
    if(m_monochromeCheckBox.GetCheck())
    {
        m_targetDevice |= LOGI_DEVICETYPE_MONOCHROME;
    }
    if(m_perKeyCheckBox.GetCheck())
    {
        m_targetDevice |= LOGI_DEVICETYPE_PERKEY_RGB;
    }
    LogiLedSetTargetDevice(m_targetDevice);
}

void CG910_SAMPLEDlg::OnBnClickedGetVersion()
{
    int major, minor, build = 0;
    if(!LogiLedGetSdkVersion(&major, &minor, & build))
    {
        ::MessageBox(NULL,L"Could not retrieve SDK version",L"error",0);
    }
    else
    {
        wchar_t versionStr[128];
        swprintf_s(versionStr, L"SDK VERSION : %d.%d.%d", major, minor, build);
        ::MessageBox(NULL, versionStr, L"LED SDK VERSION",0);
    }
    
}


void CG910_SAMPLEDlg::OnBnClickedButtonInitialize()
{
    if(!LogiLedInit())
    {
        ::MessageBox(NULL, L"Could not Initialize SDK", L"Init failed",0);
    }
}


void CG910_SAMPLEDlg::OnBnClickedButtonShutdown()
{
    m_disableNumpad.SetCheck(false);
    LogiLedShutdown();
}


void CG910_SAMPLEDlg::OnBnClickedButtonFlashR()
{
    // get the values from the sample
    int duration = GetDlgItemInt(IDC_EDIT_DURATION, 0,0);
    int interval = GetDlgItemInt(IDC_EDIT_INTERVAL, 0,0);
    int redVal = (int)(100 * ((float)GetRValue(m_singleKeyStartColorPicker.GetColor()) / (float)255));
    int greenVal = (int)(100 * ((float)GetGValue(m_singleKeyStartColorPicker.GetColor()) / (float)255));
    int blueVal = (int)(100 * ((float)GetBValue(m_singleKeyStartColorPicker.GetColor()) / (float)255));

    LogiLedFlashSingleKey(LogiLed::KeyName::R, redVal, greenVal, blueVal, duration, interval);
}


void CG910_SAMPLEDlg::OnBnClickedButtonPulse1()
{
    // get the values from the sample
    int duration = GetDlgItemInt(IDC_EDIT_DURATION, 0,0);
    int redVal = 0;
    int greenVal = 0;
    int blueVal = 0;
    GetEffectColorValues(&redVal, &greenVal, &blueVal, true);
   
    int redFinishVal = 0;
    int greenFinishVal = 0;
    int blueFinishVal = 0;
    GetEffectColorValues(&redFinishVal, &greenFinishVal, &blueFinishVal, false);

    LogiLedPulseSingleKey(LogiLed::KeyName::ONE, redFinishVal, greenFinishVal, blueFinishVal, redVal, greenVal, blueVal, duration, m_perKeyPulseLoop.GetCheck());

}

void CG910_SAMPLEDlg::GetEffectColorValues(int *red, int *green, int *blue, bool startPicker)
{
    if(startPicker)
    {
        *red = (int)(100 * ((float)GetRValue(m_singleKeyStartColorPicker.GetColor()) / (float)255));
        *green = (int)(100 * ((float)GetGValue(m_singleKeyStartColorPicker.GetColor()) / (float)255));
        *blue = (int)(100 * ((float)GetBValue(m_singleKeyStartColorPicker.GetColor()) / (float)255));
    }
    else
    {
        *red = (int)(100 * ((float)GetRValue(m_singleKeyFinishColorPicker.GetColor()) / (float)255));
        *green = (int)(100 * ((float)GetGValue(m_singleKeyFinishColorPicker.GetColor()) / (float)255));
        *blue = (int)(100 * ((float)GetBValue(m_singleKeyFinishColorPicker.GetColor()) / (float)255));
    }
        return;
}


void CG910_SAMPLEDlg::OnBnClickedButtonFkeys()
{
    int duration = GetDlgItemInt(IDC_EDIT_DURATION, 0,0);
    int redVal = 0;
    int greenVal = 0;
    int blueVal = 0;
    GetEffectColorValues(&redVal, &greenVal, &blueVal, true);

    int redFinishVal = 0;
    int greenFinishVal = 0;
    int blueFinishVal = 0;
    GetEffectColorValues(&redFinishVal, &greenFinishVal, &blueFinishVal, false);
    int loopChecked = m_perKeyPulseLoop.GetCheck();
    LogiLedPulseSingleKey(LogiLed::KeyName::F1, redFinishVal, greenFinishVal, blueFinishVal, redVal, greenVal, blueVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F2, redFinishVal, greenFinishVal, blueFinishVal, redVal, greenVal, blueVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F3, redFinishVal, greenFinishVal, blueFinishVal, redVal, greenVal, blueVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F4, redFinishVal, greenFinishVal, blueFinishVal, redVal, greenVal, blueVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F5, redFinishVal, greenFinishVal, blueFinishVal, redVal, greenVal, blueVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F6, redFinishVal, greenFinishVal, blueFinishVal, redVal, greenVal, blueVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F7, redVal, greenVal, blueVal, redFinishVal, greenFinishVal, blueFinishVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F8, redVal, greenVal, blueVal, redFinishVal, greenFinishVal, blueFinishVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F9, redVal, greenVal, blueVal, redFinishVal, greenFinishVal, blueFinishVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F10, redVal, greenVal, blueVal, redFinishVal, greenFinishVal, blueFinishVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F11, redVal, greenVal, blueVal, redFinishVal, greenFinishVal, blueFinishVal, duration, loopChecked);
    LogiLedPulseSingleKey(LogiLed::KeyName::F12, redVal, greenVal, blueVal, redFinishVal, greenFinishVal, blueFinishVal, duration, loopChecked);
}


void CG910_SAMPLEDlg::OnBnClickedCheckExcludeNumpad()
{
    LogiLed::KeyName excludedList[17] =
    {
        LogiLed::NUM_LOCK, LogiLed::NUM_SLASH, LogiLed::NUM_ASTERISK, LogiLed::NUM_MINUS,
        LogiLed::NUM_SEVEN, LogiLed::NUM_EIGHT, LogiLed::NUM_NINE,
        LogiLed::NUM_FOUR, LogiLed::NUM_FIVE, LogiLed::NUM_SIX, LogiLed::NUM_PLUS,
        LogiLed::NUM_ONE, LogiLed::NUM_TWO, LogiLed::NUM_THREE,
        LogiLed::NUM_ZERO, LogiLed::NUM_PERIOD, LogiLed::NUM_ENTER
    };
    LogiLed::KeyName *excluded = NULL;
    if (m_disableNumpad.GetCheck())
    {
        LogiLedExcludeKeysFromBitmap(excludedList, 17);
    }
    else
    {
        LogiLedExcludeKeysFromBitmap(NULL, 0);
    }
}


void CG910_SAMPLEDlg::OnBnClickedButtonSetGkeys()
{
    CheckBounds();

    int redVal = GetDlgItemInt(IDC_EDIT_RED, 0, 0);
    int greenVal = GetDlgItemInt(IDC_EDIT_GREEN, 0, 0);
    int blueVal = GetDlgItemInt(IDC_EDIT_BLUE, 0, 0);

    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_1, redVal, greenVal, blueVal);
    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_2, redVal, greenVal, blueVal);
    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_3, redVal, greenVal, blueVal);
    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_4, redVal, greenVal, blueVal);
    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_5, redVal, greenVal, blueVal);
    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_6, redVal, greenVal, blueVal);
    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_7, redVal, greenVal, blueVal);
    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_8, redVal, greenVal, blueVal);
    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_9, redVal, greenVal, blueVal);
    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_LOGO, redVal, greenVal, blueVal);
    LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::G_BADGE, redVal, greenVal, blueVal);
}
