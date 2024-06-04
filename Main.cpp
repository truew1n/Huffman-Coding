#include <iostream>
#include <stdint.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <windows.h>

#include "PriorityQueue.hpp"
#include <unordered_map>
#include "BinaryTree.hpp"
#include <algorithm>

typedef struct Dimension {
    int32_t Width;
    int32_t Height;
} Dimension;

LRESULT CALLBACK WinProcedure(HWND HWnd, UINT UMsg, WPARAM WParam, LPARAM LParam);
void DilationMask(void *Display, int32_t X, int32_t Y, uint32_t Color, Dimension DisplayDimension);
void ClearScreen(void *Display, uint32_t Color, Dimension DisplayDimension);
void DrawLine(void *Display, int32_t BeginX, int32_t BeginY, int32_t EndX, int32_t EndY, uint32_t Color, Dimension DisplayDimension);
void DrawCircle(void *Display, int32_t X, int32_t Y, int32_t Radius, uint32_t Color, Dimension DisplayDimension);


void DrawHuffmanTree(void *Display, Node<char>* Root, Node<char>* Parent, int32_t X, int32_t Y, int32_t Level, int32_t OffsetX, uint32_t Color, Dimension DisplayDimension, int32_t ParentX) {
    if (!Root) {
        return;
    }

    int32_t DX = OffsetX / (1 << Level);
    int32_t DY = 50;

    if (Root->Left) {
        int32_t ChildXLeft = X - DX;
        int32_t ChildYLeft = Y + DY;

        DrawLine(Display, X, Y, ChildXLeft, ChildYLeft, 0xFFFF0000, DisplayDimension);
        DrawHuffmanTree(Display, Root->Left, Root, ChildXLeft, ChildYLeft, Level + 1, OffsetX, Color, DisplayDimension, X);
    }
    if (Root->Right) {
        int32_t ChildXRight = X + DX;
        int32_t ChildYRight = Y + DY;

        DrawLine(Display, X, Y, ChildXRight, ChildYRight, 0xFF00FF00, DisplayDimension);
        DrawHuffmanTree(Display, Root->Right, Root, ChildXRight, ChildYRight, Level + 1, OffsetX, Color, DisplayDimension, X);
    }

    int32_t Radius = 10;
    DrawCircle(Display, X, Y, Radius, Color, DisplayDimension);
}

void GetHuffmanCodes(Node<char>* Root, std::unordered_map<char, std::string>& Codes, std::string Code = "") {
    if (!Root) return;

    if (!Root->Left && !Root->Right) {
        Codes[Root->Data] = Code;
        return;
    }

    GetHuffmanCodes(Root->Left, Codes, Code + "0");
    GetHuffmanCodes(Root->Right, Codes, Code + "1");
}

double CalculateEntropy(const std::string& Message, const std::unordered_map<char, std::string>& HuffmanCodes) {
    size_t TotalBits = 0;
    for (char c : Message) {
        TotalBits += HuffmanCodes.at(c).size();
    }

    std::unordered_map<char, double> Probabilities;
    for (const auto& Pair : HuffmanCodes) {
        char Symbol = Pair.first;
        size_t Frequency = std::count_if(Message.begin(), Message.end(), [Symbol](char c) { return c == Symbol; });
        Probabilities[Symbol] = static_cast<double>(Frequency) / Message.size();
    }

    double Entropy = 0.0;
    for (const auto& Pair : Probabilities) {
        double Probability = Pair.second;
        Entropy -= Probability * std::log2(Probability);
    }

    return Entropy;
}

int main(void)
{
    std::string UserInput;
    std::cout << "Input: ";
    std::getline(std::cin, UserInput);

    HANDLE HInstance = GetModuleHandleW(NULL);

    WNDCLASSW WindowClass = {0};
    WindowClass.lpszClassName = L"ClassHuffmanCoding";
    WindowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hInstance = (HINSTANCE) HInstance;
    WindowClass.lpfnWndProc = WinProcedure;

    if(!RegisterClassW(&WindowClass)) return -1;

    int32_t Width = 1920;
    int32_t Height = 1080;

    RECT WindowRect = { 0 };
    WindowRect.right = Width;
    WindowRect.bottom = Height;
    WindowRect.left = 0;
    WindowRect.top = 0;

    AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0);
    HWND Window = CreateWindowW(
        WindowClass.lpszClassName,
        L"Huffman Coding",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WindowRect.right - WindowRect.left,
        WindowRect.bottom - WindowRect.top,
        NULL, NULL,
        NULL, NULL
    );

    GetWindowRect(Window, &WindowRect);

    int32_t BitmapWidth = Width;
    int32_t BitmapHeight = Height;

    uint32_t BytesPerPixel = 4;

    uint32_t BitmapTotalSize = BitmapWidth * BitmapHeight;
    uint32_t DisplayTotalSize = BitmapTotalSize * BytesPerPixel;

    void *Display = malloc(DisplayTotalSize);

    BITMAPINFO BitmapInfo;
    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = BitmapWidth;
    BitmapInfo.bmiHeader.biHeight = -BitmapHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(Window);

    MSG msg = { 0 };
    int32_t running = 1;

    Dimension DisplayDimension = {BitmapWidth, BitmapHeight}; 

    std::unordered_map<char, int32_t> Frequencies;

    for(char InputChar : UserInput) {
        Frequencies[InputChar]++;
    }

    std::vector<std::pair<int32_t, char>> Pairs;

    for (const auto& Frequency : Frequencies) {
        Pairs.push_back(std::make_pair(Frequency.second, Frequency.first));
    }

    std::sort(Pairs.begin(), Pairs.end());

    PriorityQueue<Node<char>*> PQueue;

    for (const auto& Pair : Pairs) {
        PQueue.Push(Pair.first, new Node<char>(Pair.second));
    }

    while(PQueue.Size() != 1) {
        Node<char> *FirstNode = PQueue.Top();
        int32_t FirstPriority = PQueue.TopPriority();
        PQueue.Pop();

        Node<char> *SecondNode = PQueue.Top();
        int32_t SecondPriority = PQueue.TopPriority();
        PQueue.Pop();

        Node<char> *Root = new Node<char>(0);
        if(FirstPriority < SecondPriority) {
            Root->Left = FirstNode;
            Root->Right = SecondNode;
        } else if(FirstPriority == SecondPriority) {
            if(FirstNode->Data <= SecondNode->Data) {
                Root->Left = SecondNode;
                Root->Right = FirstNode;
            } else {
                Root->Left = FirstNode;
                Root->Right = SecondNode;
            }
        } else {
            Root->Left = SecondNode;
            Root->Right = FirstNode;
        }
        PQueue.Push(FirstPriority + SecondPriority, Root);
    }

    BinaryTree<char> HuffmanTree;
    HuffmanTree.SetRoot(PQueue.Top());
    PQueue.Pop();

    int32_t SpacingX = 920;
    DrawHuffmanTree(Display, HuffmanTree.GetRoot(), nullptr, BitmapWidth / 2, 50, 1, SpacingX, RGB(255, 255, 255), DisplayDimension, BitmapWidth / 2);

    std::unordered_map<char, std::string> HuffmanCodes;
    GetHuffmanCodes(HuffmanTree.GetRoot(), HuffmanCodes);

    std::string EncodedMessage;
    for(char InputChar : UserInput) {
        EncodedMessage += HuffmanCodes[InputChar];
    }

    std::cout << "Huffman Codes:" << std::endl;
    for (const auto& pair : HuffmanCodes) {
        std::cout << "\"" << pair.first << "\": " << pair.second << std::endl;
    }

    std::cout << "Entropy: " << CalculateEntropy(UserInput, HuffmanCodes) << std::endl;
    
    while (running) {
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            switch (msg.message) {
                case WM_QUIT: {
                    running = 0;
                    break;
                }
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        
        // Draw Here
        
        StretchDIBits(
            hdc, 0, 0,
            BitmapWidth, BitmapHeight,
            0, 0,
            BitmapWidth, BitmapHeight,
            Display, &BitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY
        );
    }

    return 0;
}

LRESULT CALLBACK WinProcedure(HWND HWnd, UINT UMsg, WPARAM WParam, LPARAM LParam)
{
    switch (UMsg) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default: {
            return DefWindowProcW(HWnd, UMsg, WParam, LParam);
            break;
        }
    }
    return 0;
}

void DilationMask(void *Display, int32_t X, int32_t Y, uint32_t Color, Dimension DisplayDimension)
{
    if(X >= 0 && X < DisplayDimension.Width && Y >= 0 && Y < DisplayDimension.Height) {
        ((uint32_t *) Display)[X + Y * DisplayDimension.Width] = Color;
    }
}

void ClearScreen(void *Display, uint32_t Color, Dimension DisplayDimension)
{
    for(int32_t j = 0; j < DisplayDimension.Height; ++j) {
        for(int32_t i = 0; i < DisplayDimension.Width; ++i) {
            DilationMask(Display, i, j, Color, DisplayDimension);
        }
    }
}

#define ABS(x) (x < 0 ? -x : x)

void DrawLine(void *Display, int32_t BeginX, int32_t BeginY, int32_t EndX, int32_t EndY, uint32_t Color, Dimension DisplayDimension)
{
    int32_t DX = EndX - BeginX;
    int32_t DY = EndY - BeginY;

    int32_t Steps = ABS(DX) > ABS(DY) ? ABS(DX) : ABS(DY);

    float XInc = DX / (float) Steps;
    float YInc = DY / (float) Steps;

    float FX = BeginX;
    float FY = BeginY;
    for(int i = 0; i <= Steps; ++i) {
        DilationMask(Display, FX, FY, Color, DisplayDimension);
        FX += XInc;
        FY += YInc;
    }
}

void DrawCircle(void *Display, int32_t X, int32_t Y, int32_t Radius, uint32_t Color, Dimension DisplayDimension)
{
    for(int32_t j = -Radius; j <= Radius; ++j) {
        for(int32_t i = -Radius; i <= Radius; ++i) {
            int32_t DX = -i;
            int32_t DY = -j;
            if(((DX * DX) + (DY * DY)) <= (Radius * Radius)) {
                DilationMask(Display, DX + X, DY + Y, Color, DisplayDimension);
            }
        }   
    }
}