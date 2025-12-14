#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <commctrl.h>

#define ID_GROUPBOX 1001
#define ID_SCROLLBAR 1002
#define ID_TERM_COUNT_INPUT 1003
#define ID_SUBMIT_TERM_COUNT 1004
#define ID_COURSE_COUNT_INPUT 1005
#define ID_SUBMIT_COURSE_COUNT 1006
#define ID_COURSE_DATA_BASE_INPUT 2000
#define ID_COURSE_DATA_HOURS_INPUT 2100
#define ID_SUBMIT_COURSE_DATA_BUTTON 2999

#define ID_UPDOWN_CONTROL 3000

const char g_szClassName[] = "myWindowClass";

const int WIDTH = 600;
const int HEIGHT = 600;

char INTEGER_ARRAY[] = {'0','1','2','3','4','5','6','7','8','9'};

int clientWidth;
int clientHeight;


float gpa[100];

int term_count = -1;
int current_term = 0;
int course_count = 0;


BOOL CALLBACK DestroyChildCallback(HWND hwndChild, LPARAM lParam) {
    DestroyWindow(hwndChild);
    return TRUE; 
}


int CreateNumberOfTermsPage(HWND hwnd, LPARAM lParam) {
    EnumChildWindows(hwnd, DestroyChildCallback, 0);

    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

    int label_width = 160;
    int input_width = 40;
    int total_width = label_width + input_width;

    int x_pos = (clientWidth / 2) - (total_width / 2);

    HWND hLabel = CreateWindowEx(
        0,
        "STATIC",
        "Enter Number of Terms:",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        x_pos,
        clientHeight / 2 - 10,
        label_width,
        20,
        hwnd,
        NULL,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hNumericInput = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP |
        ES_CENTER | ES_NUMBER,
        x_pos + label_width,
        clientHeight / 2 - 10,
        input_width,
        20,
        hwnd,
        (HMENU)ID_TERM_COUNT_INPUT,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hUpDown = CreateWindowEx(
        0,
        UPDOWN_CLASS,
        NULL,
        WS_CHILD | WS_VISIBLE |
        UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK | UDS_AUTOBUDDY,
        0, 0, 0, 0,
        hwnd,
        (HMENU)ID_UPDOWN_CONTROL,
        hInst,
        NULL
    );

    SendMessage(hNumericInput, EM_LIMITTEXT, (WPARAM)2, 0);
    SendMessage(hUpDown, UDM_SETRANGE32, 1, 99);
    SendMessage(hUpDown, UDM_SETPOS32, 0, 1);
    SetFocus(hNumericInput);
    

    HWND hButton = CreateWindowEx(
        0,
        "BUTTON",
        "Submit",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD |
        BS_PUSHBUTTON,
        clientWidth / 2 - 30,
        clientHeight / 2 + 13,
        60,
        20,
        hwnd,
        (HMENU)ID_SUBMIT_TERM_COUNT,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );
    return 0;
}

int CreateNumberOfCoursesPage(HWND hwnd, LPARAM lParam) {
    EnumChildWindows(hwnd, DestroyChildCallback, 0);

    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

    int label_width = 320;
    int input_width = 40;
    int total_width = label_width + input_width;

    int x_pos = (clientWidth / 2) - (total_width / 2);


    char final_string[50];
    snprintf(final_string, sizeof(final_string), "Enter How Many Courses Registered in Term %d:", current_term + 1);

    HWND hLabel = CreateWindowEx(
        0,
        "STATIC",
        final_string,
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        x_pos,
        clientHeight / 2 - 10,
        label_width,
        20,
        hwnd,
        NULL,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    HWND hNumericInput = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "1",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP |
        ES_CENTER | ES_NUMBER,
        x_pos + label_width,
        clientHeight / 2 - 10,
        input_width,
        20,
        hwnd,
        (HMENU)ID_COURSE_COUNT_INPUT,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );
    HWND hUpDown = CreateWindowEx(
        0,
        UPDOWN_CLASS,
        NULL,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP |
        UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK | UDS_AUTOBUDDY,
        0, 0, 0, 0,
        hwnd,
        (HMENU)ID_UPDOWN_CONTROL,
        hInst,
        NULL
    );

    SendMessage(hNumericInput, EM_LIMITTEXT, (WPARAM)2, 0);
    SendMessage(hUpDown, UDM_SETRANGE32, 1, 99);
    SendMessage(hUpDown, UDM_SETPOS32, 0, 1);
    SetFocus(hNumericInput);

    HWND hButton = CreateWindowEx(
        0,
        "BUTTON",
        "Submit",
        WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        clientWidth / 2 - 30,
        clientHeight / 2 + 13,
        60,
        20,
        hwnd,
        (HMENU)ID_SUBMIT_COURSE_COUNT,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );
    return 0;
}


int CreateCourseGradesPage(HWND hwnd, LPARAM lParam) {
    EnumChildWindows(hwnd, DestroyChildCallback, 0);

    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
    RECT rect;

    // Get the dimensions of the parent window's client area;
    if (!GetClientRect(hwnd, &rect))
        return 0;

    int scrollbar_width = 0;

    // HWND scrollbar = CreateWindowEx( 
    //     0,                      // no extended styles 
    //     "SCROLLBAR",           // scroll bar control class 
    //     (PTSTR) NULL,           // no window text 
    //     WS_CHILD | WS_VISIBLE |
    //     SBS_VERT,         // horizontal scroll bar style 
    //     clientWidth - scrollbar_width,              // horizontal position 
    //     0, // vertical position 
    //     scrollbar_width,             // width of the scroll bar 
    //     clientHeight,               // height of the scroll bar
    //     hwnd,             // handle to main window 
    //     (HMENU)ID_SCROLLBAR,           // no menu 
    //     hInst,                // instance owning this window 
    //     (PVOID) NULL            // pointer not needed 
    // );


    int margin = 20;
    int padding = 5;

    int x = margin + padding;
    int y = (margin * 2) + padding;

    int label1_width = 170;
    int label2_width = 160;
    int grade_width = 50;

    for(int i=0; i<course_count; i++){
        int x = margin + padding;

        char final_string[30];
        snprintf(final_string, sizeof(final_string), "Course %d Mark or Grade: ", i + 1);

        CreateWindowEx(
            0,
            "STATIC",
            final_string,
            WS_CHILD | WS_VISIBLE | SS_RIGHT,
            x,
            y,
            label1_width,
            20,
            hwnd,
            NULL,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL
        );
        x += label1_width;

        HWND hComboBox = CreateWindowEx(
            0,
            "COMBOBOX",
            NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP |
            CBS_DROPDOWN,
            x,
            y,
            grade_width,
            20 + 200,
            hwnd,
            (HMENU)(ID_COURSE_DATA_BASE_INPUT + i),
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL
        );

        const char *suggestions[] = {
            "A+",
            "A",
            "A-",
            "B+",
            "B",
            "B-",
            "C+",
            "C",
            "C-",
            "D+",
            "D",
            "F"
        };

        int num_suggestions = sizeof(suggestions) / sizeof(suggestions[0]);

        for (int i = 0; i < num_suggestions; i++) {
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)suggestions[i]);
        }
        x += grade_width + 20;

        snprintf(final_string, sizeof(final_string), "Course %d Credit Hours: ", i + 1);

        CreateWindowEx(
            0,
            "STATIC",
            final_string,
            WS_CHILD | WS_VISIBLE | SS_RIGHT,
            x,
            y,
            label2_width,
            20,
            hwnd,
            NULL,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL
        );

        x += label2_width;
        HWND hNumericInput = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            "EDIT",
            "1",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP |
            ES_CENTER | ES_NUMBER,
            x,
            y,
            40,
            20,
            hwnd,
            (HMENU)(ID_COURSE_DATA_HOURS_INPUT + i),
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL
        );
        HWND hUpDown = CreateWindowEx(
            0,
            UPDOWN_CLASS,
            NULL,
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP |
            UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK | UDS_AUTOBUDDY,
            0, 0, 0, 0,
            hwnd,
            (HMENU)ID_UPDOWN_CONTROL,
            hInst,
            NULL
        );
        SendMessage(hNumericInput, EM_LIMITTEXT, (WPARAM)1, 0);
        SendMessage(hUpDown, UDM_SETRANGE32, 1, 9);
        SendMessage(hUpDown, UDM_SETPOS32, 0, 1);

        y += 25;
    }

    HWND hwndGroupBox = CreateWindowEx(
        0,
        "BUTTON",
        "Course Scores",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        margin, margin,
        clientWidth - (margin * 2) - scrollbar_width, y + padding - margin,
        hwnd,
        (HMENU)ID_GROUPBOX,
        hInst,
        NULL
    );

    HWND hButton = CreateWindowEx(
        0,
        "BUTTON",
        "Submit",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD |
        BS_PUSHBUTTON,
        margin, y + margin / 2,
        60,
        20,
        hwnd,
        (HMENU)ID_SUBMIT_COURSE_DATA_BUTTON,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    // SCROLLINFO si;
    // si.cbSize = sizeof(si);
    // si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE; // Mask specifying which fields are valid

    // // The full range of the scroll bar
    // si.nMin = 0;
    // si.nMax = y;

    // // The size of the "thumb" or scroll box relative to the range.
    // // For example, if your window displays 10 lines, and the max is 100 lines, 
    // // then nPage would be 10.
    // si.nPage = (int)(y / course_count);
    // si.nPos = 0;
    // SetScrollInfo(
    //     scrollbar,  // Handle to the scroll bar control
    //     SB_CTL,     // Indicates this is a scroll bar control, not a window scroll bar
    //     &si,        // Pointer to the SCROLLINFO structure
    //     TRUE        // Redraw the scroll bar immediately
    // );
}

int isElementInArray(char arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return 1; // Element found
        }
    }
    return 0; // Element not found
}

float getScore(char str[]) {
    int isGrade = 0;
    int arraySize = sizeof(str) / sizeof(str[0]);
    for(int i=0; i < arraySize; i++) {
        char character = str[i];
        if((character != 0) && !isElementInArray(INTEGER_ARRAY, 10, character)){
            isGrade = 1;
            break;
        }
    }

    if(!isGrade){
        int percentage = atoi(str);
        if(percentage >= 95){
            return 12.0;
        } else if(percentage >= 90) {
            return 11.5;
        } else if(percentage >= 85) {
            return 11.0;
        } else if(percentage >= 80) {
            return 10.0;
        } else if(percentage >= 75) {
            return 9.0;
        } else if(percentage >= 70) {
            return 8.0;
        } else if(percentage >= 65) {
            return 7.0;
        } else if(percentage >= 60) {
            return 6.0;
        } else if(percentage >= 56) {
            return 5.0;
        } else if(percentage >= 53) {
            return 4.0;
        } else if(percentage >= 50) {
            return 3.0;
        } else {
            return 0.0;
        }
    } else {
        if(strcmp(str, "A+") == 0) {
            return 12.0;
        } else if(strcmp(str, "A") == 0) {
            return 11.5;
        } else if(strcmp(str, "A-") == 0) {
            return 11.0;
        } else if(strcmp(str, "B+") == 0) {
            return 10.0;
        } else if(strcmp(str, "B") == 0) {
            return 9.0;
        } else if(strcmp(str, "B-") == 0) {
            return 8.0;
        } else if(strcmp(str, "C+") == 0) {
            return 7.0;
        } else if(strcmp(str, "C") == 0) {
            return 6.0;
        } else if(strcmp(str, "C-") == 0) {
            return 5.0;
        } else if(strcmp(str, "D+") == 0) {
            return 4.0;
        } else if(strcmp(str, "D") == 0) {
            return 3.0;
        } else {
            return 0.0;
        }
    }

    return 0.0;
}

float sum(float arr[], int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}


// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg) {
        // case WM_LBUTTONDOWN:
        // {
        //     char szFileName[MAX_PATH];
        //     HINSTANCE hInstance = GetModuleHandle(NULL);

        //     GetModuleFileName(hInstance, szFileName, MAX_PATH);
        //     MessageBox(hwnd, szFileName, "This program is:", MB_OK | MB_ICONINFORMATION);
        //     break;
        // }
        case WM_CREATE: {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            clientWidth = clientRect.right;
            clientHeight = clientRect.bottom;
            CreateNumberOfTermsPage(hwnd, lParam);
            // HMENU hMenu, hSubMenu;
            // HICON hIcon, hIconSm;

            // hMenu = CreateMenu();

            // hSubMenu = CreatePopupMenu();
            // AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
            // AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

            // hSubMenu = CreatePopupMenu();
            // AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Go");
            // AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Home");
            // AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Stuff");

            // SetMenu(hwnd, hMenu);

            // hIcon = LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
            // if(hIcon)
            //     SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            // else
            //     MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK | MB_ICONERROR);

            // hIconSm = LoadImage(NULL, "menu_two.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
            // if(hIconSm)
            //     SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
            // else
            //     MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK | MB_ICONERROR);
        }
        break;
        case WM_COMMAND:
        {
            if (LOWORD(wParam) == ID_SUBMIT_TERM_COUNT) {
                if (HIWORD(wParam) == BN_CLICKED) {
                    // MessageBox(hwnd, "Button was clicked!", "Button Action", MB_OK);
                    HWND hTextInput = GetDlgItem(hwnd, ID_TERM_COUNT_INPUT);
                    if(hTextInput != NULL){
                        char buffer[3];
                        GetWindowText(hTextInput, buffer, sizeof(buffer));
                        if(buffer[0] != '\0'){
                            term_count = atoi(buffer);
                            current_term = 0;
                            gpa[term_count];
                            CreateNumberOfCoursesPage(hwnd, lParam);
                        } else {
                            MessageBox(hwnd, "The term count field is a required field.", "Input Retrieved", MB_OK | MB_ICONERROR);
                        }
                    }
                }
            } else if(LOWORD(wParam) == ID_SUBMIT_COURSE_COUNT) {
                HWND hTextInput = GetDlgItem(hwnd, ID_COURSE_COUNT_INPUT);
                if(hTextInput != NULL){
                    char buffer[3];
                    GetWindowText(hTextInput, buffer, sizeof(buffer));
                    if(buffer[0] != '\0'){
                        course_count = atoi(buffer);
                        CreateCourseGradesPage(hwnd, lParam);
                    } else {
                        MessageBox(hwnd, "The course count field is a required field.", "Input Retrieved", MB_OK | MB_ICONERROR);
                    }
                }
            } else if(LOWORD(wParam) == ID_SUBMIT_COURSE_DATA_BUTTON) {
                int total_hours = 0;
                float total_score = 0;
                for(int i=0; i<course_count; i++){
                    char grade_buffer[4];
                    char hours_buffer[3];

                    HWND hGradeInput = GetDlgItem(hwnd, ID_COURSE_DATA_BASE_INPUT + i);
                    HWND hHoursInput = GetDlgItem(hwnd, ID_COURSE_DATA_HOURS_INPUT + i);
                    if(hGradeInput == NULL){
                        printf("A> %d", ID_COURSE_DATA_BASE_INPUT + i);
                        MessageBox(hwnd, "Something is wrong...", "Input Retrieved", MB_OK | MB_ICONERROR);
                        return 0;
                    } else if (hHoursInput == NULL){
                        printf("A> %d", ID_COURSE_DATA_HOURS_INPUT + i);
                        MessageBox(hwnd, "Something is wrong... 2", "Input Retrieved", MB_OK | MB_ICONERROR);
                        return 0;
                    }
                    GetWindowText(hGradeInput, grade_buffer, sizeof(grade_buffer));
                    GetWindowText(hHoursInput, hours_buffer, sizeof(hours_buffer));
                    if(grade_buffer[0] == '\0' || hours_buffer[0] == '\0'){
                        char final_string[75];
                        snprintf(final_string, sizeof(final_string), "Missing required data for Course %d!", i + 1);
                        MessageBox(hwnd, final_string, "Input Retrieved", MB_OK | MB_ICONERROR);
                        return 0;
                    } else {
                        total_hours += atoi(hours_buffer);
                        total_score += getScore(grade_buffer);
                    }
                }
                gpa[current_term] = total_score / total_hours;

                current_term += 1;
                if (current_term == term_count){
                    char full_text[1028] = "";
                    for(int i=0;i<term_count;i++){
                        char string[30];
                        snprintf(string, sizeof(string), "Semester %d GPA: %.2f\n", i + 1, gpa[i]);
                        strcat(full_text, string);
                    }
                    float average_gpa = sum(gpa, term_count) / term_count;
                    char string[30];
                    snprintf(string, sizeof(string), "Overall GPA: %.2f\n", average_gpa);
                    strcat(full_text, string);

                    if(average_gpa >= 3.6) {
                        strcat(full_text, "\nExcellent");
                    } else if(average_gpa >= 3.0) {
                        strcat(full_text, "\nVery Good");
                    } else if(average_gpa >= 2.6) {
                        strcat(full_text, "\nGood");
                    } else if(average_gpa >= 2.0) {
                        strcat(full_text, "\nPass");
                    } else {
                        strcat(full_text, "\nFail.");
                    }
                    MessageBox(hwnd, full_text, "Final Results!", MB_OK);
                    CreateNumberOfTermsPage(hwnd, lParam);
                } else {
                    CreateNumberOfCoursesPage(hwnd, lParam);
                }
            }
           break;
        };
    // case WM_VSCROLL:  // HARD TO IMPLEMENT :(
    //     // Get all the vertial scroll bar information.
    //     HDC hdc; 
    //     TEXTMETRIC tm;

    //     hdc = GetDC(hwnd); 
    //     GetTextMetrics (hdc, &tm);
    //     int yChar = tm.tmHeight + tm.tmExternalLeading; 
        
    //     SCROLLINFO si;
    //     GetScrollInfo(hwnd, SB_HORZ, &si);

    //     si.cbSize = sizeof (si);
    //     si.fMask  = SIF_ALL;
    //     GetScrollInfo (hwnd, SB_VERT, &si);

    //     // Save the position for comparison later on.
    //     int yPos = si.nPos;
    //     switch (LOWORD (wParam))
    //     {

    //     // User clicked the HOME keyboard key.
    //     case SB_TOP:
    //         si.nPos = si.nMin;
    //         break;
              
    //     // User clicked the END keyboard key.
    //     case SB_BOTTOM:
    //         si.nPos = si.nMax;
    //         break;
              
    //     // User clicked the top arrow.
    //     case SB_LINEUP:
    //         si.nPos -= 1;
    //         break;
              
    //     // User clicked the bottom arrow.
    //     case SB_LINEDOWN:
    //         si.nPos += 1;
    //         break;
              
    //     // User clicked the scroll bar shaft above the scroll box.
    //     case SB_PAGEUP:
    //         si.nPos -= si.nPage;
    //         break;
              
    //     // User clicked the scroll bar shaft below the scroll box.
    //     case SB_PAGEDOWN:
    //         si.nPos += si.nPage;
    //         break;
              
    //     // User dragged the scroll box.
    //     case SB_THUMBTRACK:
    //         si.nPos = si.nTrackPos;
    //         break;
              
    //     default:
    //         break; 
    //     }

    //     // Set the position and then retrieve it.  Due to adjustments
    //     // by Windows it may not be the same as the value set.
    //     si.fMask = SIF_POS;
    //     SetScrollInfo (hwnd, SB_VERT, &si, TRUE);
    //     GetScrollInfo (hwnd, SB_VERT, &si);

    //     // If the position has changed, scroll window and update it.
    //     if (si.nPos != yPos){                    
    //         ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
    //         UpdateWindow(hwnd);
    //     }
    //     break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    // Specify the class for Up-Down controls
    icex.dwICC = ICC_UPDOWN_CLASS; 
    if (!InitCommonControlsEx(&icex))
    {
        MessageBox(NULL, TEXT("Failed to initialize Common Controls."), TEXT("Error"), MB_ICONERROR);
        return 1;
    }

    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "GPA Calculator",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT | WS_VSCROLL,
        WIDTH,
        HEIGHT,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}