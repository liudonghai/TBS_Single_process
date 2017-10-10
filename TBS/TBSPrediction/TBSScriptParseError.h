#pragma once
#include "stdafx.h"

//define the erroe code for parsing script.
#define SCRIPT_EOF				0	//脚本到达结尾未能读取到结束标志，
#define SCRIPT_CANNOT_OPEN		1	//脚本文件打开失败
#define SCRIPT_LINE				2	//不能匹配该行
#define SCRIPT_EXC_LOOP_END		3	//多余的循环结束
#define SCRIPT_EXC_COND_END		4	//多余的条件结束
#define SCRIPT_LOOP_NOTEND		5	//循环未结束
#define SCRIPT_COND_NOTEND		6	//条件未结束
#define SCRIPT_EXC_FININSH		7	//多余的脚本结束标志
#define SCRIPT_SINGLE			8	//<...>语句错误
#define SCRIPT_KEYWORD			9	//错误的关键字
#define SCRIPT_NOT_MATCH		10	//该行标签开始和结束不匹配
#define SCRIPT_NOT_NUM			11	//数字表达式的中出现非数字字符
#define SCRIPT_FINISH			12	//脚本解析到FINISH标签
#define SCRIPT_CODE_ERROR		13	//脚本编码错误，暂时只支持ANSI和UTF-8
CString cstrErrorInf[] = {L"脚本到达结尾未能读取到结束标志",L"脚本打开失败",L"非法行",L"多余循环结束语句",L"多余的条件结束语句",
							L"循环未结束",L"条件未结束",L"多余的脚本结束标志",L"<***>语句错误",
							L"错误的关键字",L"该行标签开始和结束不匹配", L"数字表达式的中出现非数字字符", L"脚本解析到FINISH标签",L"编码错误"};