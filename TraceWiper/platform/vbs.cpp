
#include "vbs.h"

#include <string>

std::string& str_trim_left(std::string& str)
{
	std::string::iterator iter;
	for(iter=str.begin(); iter!=str.end(); ++iter)
	{
	#ifdef TRACEWIPER
		if(!iswspace(*iter))
			break;
	#endif
	}
	str.erase(str.begin(), iter);
	return str;
}

std::string& str_trim_right(std::string& str)
{
	std::string::iterator iter;
	iter=str.end();
	for(iter--; iter!=str.begin(); iter--)
	{
	#ifdef TRACEWIPER
		if(!iswspace(*iter))
		{
			break;
		}
	#endif
	}
	iter++;
	str.erase(iter, str.end());
	return str;
}

std::string& str_trim(std::string& str)
{
    return str_trim_right(str_trim_left(str));
}

std::string create_new()
{
    std::string str;
    str="Set Word = CreateObject(\"Word.Application\")\n";
    str+="Word.Documents.Add\n\n\n";
    return str;
}

std::string close_save(const char* filename)
{
    std::string str;
    str="Word.ActiveDocument.SaveAs (\"";
    str+=filename;
    str+="\")\n";
    str+="Word.Quit (False)\n";
    str+="Set Word = Nothing\n\n\n";
    return str;
}

//selection �����ƣ��Լ���������һԪ��
std::string move_down()
{
    std::string str;
    str+="Word.Selection.MoveDown 5, 1000\n";
    str+="Word.Selection.TypeParagraph\n\n";
    return str;
}

std::string put_Paragraph()
{
    std::string str;
    str="\nWord.Selection.TypeParagraph\n\n";
    return str;
}

//���ӱ���
//std::string put_title(const char* title, const char* title_type="���� 1", int align=ALIGN_LEFT)
std::string put_title(const char* title, const char* title_type, int align)
{
    std::string str;
    str+=put_Paragraph();    
    str="\nWord.Selection.MoveDown 5, 1000\n"
        "Word.Selection=\"";
    str+=title;
    str+="\"\nWord.Selection.ParagraphFormat.Alignment = ";
    if(align==ALIGN_CENTER)
        str+="wdAlignParagraphCenter";
    else if(align==ALIGN_LEFT)
        str+="wdAlignParagraphLeft";
    else if(align==ALIGN_RIGHT)
        str+="wdAlignParagraphRight";
    str+="\nWord.Selection.Style = Word.ActiveDocument.Styles(\"";
    str+=title_type;
    str+="\")\n\n";
    str+=move_down();

    return str;
}

//���ӡ�����1��
//std::string put_title1(const char* title, int align=ALIGN_CENTER)
std::string put_title1(const char* title, int align)
{
    return put_title(title, "���� 1", align);
}

//std::string put_title2(const char* title, int align=ALIGN_LEFT)
std::string put_title2(const char* title, int align)
{
    return put_title(title, "���� 2", align);
}

//std::string put_title3(const char* title, int align=ALIGN_LEFT)
std::string put_title3(const char* title, int align)
{
    return put_title(title, "���� 3", align);
}

//���ӱ����һ������
std::string add_grid_ln(const char* line)
{
    std::string str;
    str="\"";
    str+=str_trim(std::string(line) );
    str+="\"";
    str+="+ vbCrLf _\n";
    return str;
}

//content������add_grid_ln���������
std::string put_grid(const char* content)
{
    std::string str;
    str="Word.Selection =";//\"

    //std::string cstr(content);//"\"+ vbCrLf _\n");
    //std::string strtmp("\"+ vbCrLf"); //("\"+ vbCrLf _\n");

    //if( cstr.Right(strtmp.GetLength() )==strtmp)
    //    cstr.Delete(cstr.GetLength()-strtmp.GetLength(), strtmp.GetLength());
    str+=content;    //(LPCSTR)cstr;
    str+="\n\n";    //"\"\n\n";

    str+="Word.Selection.ConvertToTable 100, 100, 0, 0 \n";
    str+="With Word.Selection.Tables(1) \n";
    str+="        If .Style <> \"������\" Then \n";
    str+="            .Style = \"������\" \n";
    str+="        End If \n";
    str+="        .ApplyStyleHeadingRows = True \n";
    str+="        .ApplyStyleLastRow = True \n";
    str+="        .ApplyStyleFirstColumn = True \n";
    str+="        .ApplyStyleLastColumn = True \n";
    str+="End With \n\n\n";
    return str;
}

std::string add_warning_msg(const char* msg)
{
    std::string str;
    str="\nWord.Selection=\"";
    str+=msg;
    str+="\"\n";
    str+="Word.Selection.Font.Color = wdColorLightBlue\n";
    str+=move_down();
    return str;
}

extern "C" void RunScript(const char* script_str);
#pragma comment(lib, "ScriptHost.lib")


// end of file 