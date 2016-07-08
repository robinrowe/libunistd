// vcl.h

#ifndef vcl_h
#define vcl_h

#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <vector>

typedef std::exception Exception;

class AnsiString
:	public std::string 
{
public:
	int Length() const
	{	return length();
	}
	AnsiString()
	{}
	AnsiString(const char* s)
	:	std::string(s)
	{}
	AnsiString(const char* s,int offset,int len)
	:	std::string(s,offset,len)
	{}
	AnsiString SubString(int offset,int len) const
	{	return AnsiString(c_str(),offset,len);
	}
	int Pos(const char* s) const
	{	const char* p=c_str();
		const char* offset=strstr(c_str(),s);
		if(!offset)
		{	return 0;
		}
		return offset-p;
	}
	int ToInt() const
	{	return atoi(c_str());
	}
	AnsiString& operator=(const char* s)
	{	assign(s);
		return *this;
	}
	AnsiString& operator=(const std::string& s)
	{	assign(s);
		return *this;
	}
	AnsiString& operator+(const char* s)
	{	append(s);
		return *this;
	}
	operator const char*() const
	{	return c_str();
	}
	void printf(const char* f,int x)
	{}
};

class App
{
public:
	App()
	:	ShowMainForm(false)
	{}
	void Initialize();
	AnsiString Title;
	bool CreateForm(const char* name,const int* id);
	bool ShowMainForm;
	void Run();
	void ShowException(Exception* e);
};

class TStringList
{	
public:
	TStringList()
	:	Count(0)
	{}
	std::vector<AnsiString> Strings;
	int Count;
	bool LoadFromFile(const char* filename);
	void Add(const char* s);
	void Clear();
	void Append(const char* s);
	void Delete(int i);
	const AnsiString& operator[](unsigned i) const;
};

class TObject
{protected:
	const char* name;
public:
	bool Visible;
	int Top;
	int Left;
	int Width;
	int Height;
	TObject()
	:	name("?")
	,	Visible(false)
	{}
	void Show();
	void Hide();
};

class TControl
:	public TObject
{public:
	TControl(TObject* obj);
	TControl()
	{}
};


class TComponent
:	public TObject
{public:
	TComponent(TObject* obj);
};

class TForm
:	public TControl
{public:
	TForm(TObject* obj);
};

struct TText
{	AnsiString Text;
};

class TPanel
:	public TControl
{public:
	TPanel(TObject* obj);
    std::vector<TText*> Items;
};

class TMemo
:	public TControl
{	TStringList lines;
public:
	TMemo(TObject* obj)
	:	Lines(&lines)
	{}
	TStringList* Lines;
	void Clear();
};

class TStatusBar
:	public TControl
{public:
	TStatusBar(TObject* obj);
	TPanel* Panels;
};

class TSplitter
:	public TControl
{public:
	TSplitter(TObject* obj);
};

class TTimer
:	public TObject
{public:
	TTimer(TObject* obj);
	bool Enabled;
};

class TButton
:	public TControl
{public:
	TButton(TObject* obj);
	AnsiString Caption;
};

#define VCL_CLASS(x) \
class x \
:	public TControl \
{public: \
	x(TObject* obj); \
}

VCL_CLASS(TMouseButton);
VCL_CLASS(TActionList);
VCL_CLASS(TAction);
VCL_CLASS(TToolButton);
VCL_CLASS(TToolBar);
VCL_CLASS(TCheckBox);
VCL_CLASS(TRadioGroup);
VCL_CLASS(TEdit);
VCL_CLASS(TScrollBar);
VCL_CLASS(TGroupBox);
VCL_CLASS(TOpenDialog);
VCL_CLASS(TComboBox);
VCL_CLASS(TCloseAction);
VCL_CLASS(TMessage);

struct TShiftState
{	bool Contains(int mask);
};

struct TBrush
{	int Color;
};

enum
{	clBtnFace,
	clWindowText,
	CtlSysCommand,
	ssCtrl
};

struct TFont
{	int Size;
	int Color;
};

typedef AnsiString TCaption;

class TSpeedButton
:	public TControl
{public:
	TSpeedButton(TObject* obj);
	TCaption Caption;	
};


struct TRect
{	int left;
	int right;
	int top;
	int bottom;
	TRect()
	{	left=right=top=bottom=0;
	}
	TRect(int left,int right,int top,int bottom)
	{	this->left=left;
		this->right=right;
		this->top=top;
		this->bottom=bottom;
	}
};

class TLabel
:	public TControl
{public:
	TLabel(TObject* obj);
	AnsiString Caption;
};

class TCanvas
:	public TControl
{public:
	TFont* Font;
	TBrush* Brush;
	TCanvas(TObject* obj);
	void Rectangle(int x,int y,int dx,int dy)
	{}
	void TextOutA(int x, int y, const char* s)
	{}
	void FillRect(TRect& r)
	{}
	void MoveTo(int x,int y)
	{}
	void LineTo(int x,int y)
	{}
};

class TImageList
:	public TControl
{public:
	TImageList(TObject* obj);
	TRect Rectangle;
	void Draw(TCanvas* c, int x, int y, int z, bool isShow);
};

class TPaintBox
:	public TControl
{public:
	TPaintBox(TObject* obj);
	TCanvas* Canvas;
	TFont* Font;
};

#define MESSAGE 
#define BEGIN_MESSAGE_MAP
#define MESSAGE_HANDLER(x, y, z)
#define END_MESSAGE_MAP(x)
#define STATUS_PLAYBACK 0
#define USEFORM(x,y) extern int y
#define MAXPATH 255
#define __classid(x) #x
#define __published public
#define PACKAGE 
#define __fastcall
#define False false
#define True true

inline
void SetWindowsHeader()
{}

typedef AnsiString String;

#endif
